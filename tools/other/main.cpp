#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <numeric>
#include <utility>
#include <queue>
#define HAS_CONCEPT __cplusplus >= 202002L


using std::cin, std::cout, std::endl;

template<typename T>
using matrix_value_t = typename std::remove_reference_t<decltype(std::declval<T>()[0][0])>;


template<typename T>
struct Edge1 {
    T to, cost, next;
};



#if HAS_CONCEPT
template<typename T>
concept ATable = requires (T e) {
    e[0].to;
    e[0].cost;
    e[0].next;
};
#endif

template<typename T>
using aedge_value_t = typename std::remove_reference_t<decltype(std::declval<T>()[0].to)>;


template<typename Head, typename Table>
#if HAS_CONCEPT
requires ATable<Table>
#endif
class a_gabow_impl {
    typedef aedge_value_t<Table> value_type;

    template<typename T>
    using Vector = std::vector<T>;

    Head& head;
    Table& table;
public:
    typedef Vector<Vector<value_type>> component_type;
    explicit a_gabow_impl(Head &h, Table& t) : head(h), table(t), time(std::size(h)), stk1(), stk2(), scc(std::size(h)), scc_num(), tick() {}

    Vector<value_type> time;
    std::stack<value_type> stk1;
    std::stack<value_type> stk2;
    Vector<value_type> scc;
    int scc_num;
    int tick;
    Vector<Vector<value_type>> component;
    void doit(int cur) {
        time[cur] = ++tick;
        stk1.push(cur);
        stk2.push(cur);
        for(auto i = head[cur]; i; i = table[i].next) {
            auto vtx = table[i].to;
            if(!time[vtx]) {
                doit(vtx);
            } else if (0 == scc[vtx]) {
                while(time[stk2.top()] > time[vtx]) {
                    stk2.pop();
                }
            }
        }
        if (stk2.top() == cur) {
            stk2.pop();
            ++scc_num;
            component.emplace_back();
            value_type top;
            do {
                top = stk1.top();
                stk1.pop();
                scc[top] = scc_num;
                component.back().push_back(top);
            } while(top != cur);
        }
    }
};



template<typename T>
struct Edge0 {
    T from, to, cost;
};




/// \brief store edges in a adjacency table
template<typename T = int>
class Basic_graph1 {
    typedef std::vector<T> head_type;
    typedef std::vector<Edge1<T>> table_type;

    T cnt;

    std::vector<bool> vis;

    T numOfVertexes, numOfEdges;

public:

    std::vector<Edge1<T>> table;// Adjacency table
    std::vector<T> head;

    std::vector<Edge0<T>> spanningTree;

    constexpr static T INF = 0x3f3f3f3f3f3f3f3f;

    Basic_graph1() noexcept : table(1), cnt(), head(), numOfVertexes(), numOfEdges() {}

    void clear() noexcept {
        table.clear();
        head.clear();
        vis.clear();
        spanningTree.clear();
        cnt           = 0;
        numOfVertexes = 0;
    }

    inline void setNumOfVertexes(T vertexes) {
        numOfVertexes = vertexes;
        head.resize(vertexes + 2);
    }

    explicit Basic_graph1(T vertexes) noexcept
        : cnt(), numOfVertexes(vertexes), numOfEdges(), table(1), head(vertexes) {}

    inline void insert(T from, T to, T cost) {
        table.push_back({.to = to, .cost = cost, .next = head[from]});
        head[from] = ++cnt;
    }

    /// \brief prim, store tree's edge in spanningTree
    /// \param start
    /// \return the sum of the path, 0 if the graph is not connected.
    T evaluateMinimumSpanningTree(T start) {
        constexpr T inf = INF;
        std::vector<T> distance(numOfVertexes + 2, inf);
        vis.clear();
        vis.resize(numOfVertexes + 2);
        T ret   = 0;
        T count = 0;

        struct queue_elem {
            T from, shortest_cost, to;
            queue_elem(T from, T cost, T to) : from(from), shortest_cost(cost), to(to) {}
            inline bool operator>(const queue_elem& other) const {
                return shortest_cost > other.shortest_cost;
            }
        };

        std::priority_queue<queue_elem,
                            std::vector<queue_elem>,
                            std::greater<>>
                queue;
        queue.emplace(start, 0, start);
        while (!queue.empty() && count < numOfVertexes) {

            auto front = queue.top();
            queue.pop();
            if (vis[front.to]) {
                // already use this node
                continue;
            }

            ++count;
            ret += front.shortest_cost;
            vis[front.to] = true;
            spanningTree.push_back({.from = front.from, .to = front.to, .cost = front.shortest_cost,});
            // slack
            for (auto cur = head[front.to]; cur; cur = table[cur].next) {
                if (table[cur].cost < distance[table[cur].to]) {
                    distance[table[cur].to] = table[cur].cost;
                    queue.emplace(front.to, table[cur].cost, table[cur].to);
                }
            }
        }

        if (count == numOfVertexes) {
            return ret;
        }
        return 0;
    }

    auto evaluateConnectedComponent() -> typename a_gabow_impl<head_type, table_type>::component_type {
        a_gabow_impl<head_type, table_type> gabowImpl(head, table);
        for (int i = 0; i < (int) head.size(); ++i) {
            if (!gabowImpl.time[i]) {
                gabowImpl.doit(i);
            }
        }
        return std::move(gabowImpl.component);
    }
};



template <typename T, bool = std::is_enum<T>::value>
struct is_flag;

template <typename T>
struct is_flag<T, true> : std::false_type { };

template <typename T, typename std::enable_if<is_flag<T>::value>::type* = nullptr>
T operator |(T lhs, T rhs) {
    using u_t = typename std::underlying_type<T>::type;
    return static_cast<T>(static_cast<u_t>(lhs) | static_cast<u_t>(rhs));
}



int main() {

    int n, m;
    cin >> n >> m;
    std::vector<int> weights(n);
    for (auto &i : weights) {
        cin >> i;
    }

    Basic_graph1<int> graph(n);

    for (int i = 0; i < m; ++i) {
        int from, to;
        cin >> from >> to;
        graph.insert(from - 1, to - 1, 0);
    }

    auto components = graph.evaluateConnectedComponent();

    int max_weight = 0;

    std::vector<int> dp(n);

    for (const auto &component : components) {
        int weight = 0;
        for (auto vtx : component) {
            weight += weights[vtx];
        }

        int cur_max_route = 0;
        for (auto vtx : component) {
            int cur_route = 0;
            for (auto i = graph.head[vtx]; i; i = graph.table[i].next) {
                cur_route = std::max(cur_route, dp[graph.table[i].to]);
            }
            cur_max_route = std::max(cur_max_route, cur_route);
        }

        weight = cur_max_route + weight;
        for (auto vtx : component) {
            dp[vtx] = weight;
        }
        max_weight = std::max(max_weight, weight);
    }

    cout << max_weight << endl;
}
