#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <queue>
#include <stack>
#include <unordered_map>
#include <utility>
#include <vector>
#define HAS_CONCEPT __cplusplus >= 202002L


using std::cin, std::cout, std::endl;

template<typename T>
using matrix_value_t = typename std::remove_reference_t<decltype(std::declval<T>()[0][0])>;


template<typename T>
struct Edge1 {
    T to, cost, next;
};


#if HAS_CONCEPT
template<typename Head, typename ATable>
concept AGraph = requires(Head head, ATable atable, int idx) {
    atable[head[idx]].to;
    atable[head[idx]].cost;
    atable[atable[head[idx]].next];
};
#endif

template<typename T>
using aedge_value_t = typename std::remove_reference_t<decltype(std::declval<T>()[0].to)>;


template<typename Head, typename Table, typename Comp = std::greater<>>
#if HAS_CONCEPT
requires AGraph<Head, Table>
#endif
class a_dijkstra_impl {
    typedef aedge_value_t<Table> value_type;


    template<typename T>
    using Vector = std::vector<T>;

    Head &head;
    Table &table;
    constexpr static Comp comp;
    constexpr static value_type inf = 0x3f3f'3f3f'3f3f'3f3f;

public:
    constexpr static value_type INF = comp(inf, 0) ? inf : -inf;

    struct path_elem {
        value_type path;
        value_type last;
    };

    Vector<path_elem> path;

    a_dijkstra_impl(Head &h, Table &t, value_type size) : head(h), table(t), path(size, {.path = INF, .last = 0}) {}
    a_dijkstra_impl(Head &h, Table &t) : a_dijkstra_impl(h, t, std::size(h)) {}

    void doit(value_type start) {
        Vector<bool> vis(path.size());
        path[start] = {.path = 0, .last = start};
        std::priority_queue<std::pair<value_type /* shortest path */, value_type /* vertex */>,
                            std::vector<std::pair<value_type, value_type>>,
                            Comp>
                queue;
        queue.emplace(0, start);
        while (!queue.empty()) {
            auto front = queue.top();
            queue.pop();
            if (vis[front.second]) { continue; }
            vis[front.second] = true;
            for (value_type cur = head[front.second]; cur; cur = table[cur].next) {
                auto edge = table[cur];
                if (!vis[edge.to] && comp(path[edge.to].cost, path[front.second].cost + edge.cost)) {
                    path[edge.to].cost = path[front.second].cost + edge.cost;
                    // the last
                    path[edge.to].from = front.second;
                    queue.emplace(path[edge.to].cost, edge.to);
                }
            }
        }
    }
};

template<typename Head, typename Table>
#if HAS_CONCEPT
requires AGraph<Head, Table>
#endif
class a_gabow_impl {
    typedef aedge_value_t<Table> value_type;

    template<typename T>
    using Vector = std::vector<T>;

    Head &head;
    Table &table;

public:
    typedef Vector<Vector<value_type>> component_type;
    a_gabow_impl(Head &h, Table &t) : head(h), table(t), time(std::size(h)), stk1(), stk2(), scc(std::size(h)), scc_num(), tick() {}

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
        for (auto i = head[cur]; i; i = table[i].next) {
            auto vtx = table[i].to;
            if (!time[vtx]) {
                doit(vtx);
            } else if (0 == scc[vtx]) {
                while (time[stk2.top()] > time[vtx]) {
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
            } while (top != cur);
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


    T cnt;

    std::vector<bool> vis;

    T numOfVertexes, numOfEdges;

public:
    typedef std::vector<T> head_type;
    typedef std::vector<Edge1<T>> table_type;

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
            inline bool operator>(const queue_elem &other) const {
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
            spanningTree.push_back({
                    .from = front.from,
                    .to   = front.to,
                    .cost = front.shortest_cost,
            });
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


template<typename T, bool = std::is_enum<T>::value>
struct is_flag;

template<typename T>
struct is_flag<T, true> : std::false_type {};

template<typename T, typename std::enable_if<is_flag<T>::value>::type * = nullptr>
T operator|(T lhs, T rhs) {
    using u_t = typename std::underlying_type<T>::type;
    return static_cast<T>(static_cast<u_t>(lhs) | static_cast<u_t>(rhs));
}


int fact_n(int num, int end) {
    return num == end ? 1 : num * fact_n(num - 1, end);
}

int Combination(int up, int down) {
    return fact_n(down, down - up) / fact_n(up, 0);
}

int BellNumber(int num) {
    if (num == 0) { return 1; }
    int ret = 0;
    for (int i = 0; i < num; ++i) {
        ret += Combination(i, num - 1) * BellNumber(i);
    }
    return ret;
}

struct alignas(long) S {
};

#if HAS_CONCEPT
template<typename T, typename Index>
concept has_subscript = requires(T t, Index idx) { t[idx]; };

namespace detail {
    template<class T, class U>
    concept SameHelper = std::is_same_v<T, U>;
}

template<class T, class U>
concept same_as = detail::SameHelper<T, U> && detail::SameHelper<U, T>;

#endif


/// \brief a_aoe_latest_start_time_dfs for adjacency table graph
/// \attention need to manual put the whole time in the sink vertex in OutIter,
///            if the graph is not connected, need to call this function multiple times.
template<typename Head, typename Table, typename OutIter>
#if HAS_CONCEPT
requires AGraph<Head, Table> && same_as<typename std::iterator_traits<OutIter>::iterator_category, std::random_access_iterator_tag>
#endif
void a_aoe_latest_start_time_dfs(aedge_value_t<Table> cur, const Head &head, const Table &table, const OutIter &out) {
    typedef aedge_value_t<Table> value_type;

    if (out[cur] || !head[cur]) { return; }

    value_type min = std::numeric_limits<value_type>::max();

    for (value_type e = head[cur]; e; e = table[e].next) {
        a_aoe_latest_start_time_dfs(table[e].to, head, table, out);

        min = std::min(min, out[table[e].to] - table[e].cost);
    }

    out[cur] = min;
}

template<typename T>
using Vector = std::vector<T>;

int main() {

    int vtx_num, edge_num;
    cin >> vtx_num >> edge_num;

    Vector<bool> ins(vtx_num), outs(vtx_num);

    typedef Basic_graph1<int> Graph;

    Graph graph(vtx_num + 2);

    for (int i = 0; i < edge_num; ++i) {
        int from, to, cost;
        cin >> from >> to >> cost;
        outs[from] = true;
        ins[to]    = true;
        graph.insert(from, to, cost);
    }

    int start = vtx_num;

    for (int i = 0; i < (int) ins.size(); ++i) {
        if (!ins[i]) {
            graph.insert(start, i, 0);
        }
    }

    int end = start + 1;

    for (int i = 0; i < (int) outs.size(); ++i) {
        if (!outs[i]) {
            graph.insert(i, end, 0);
        }
    }

    typedef a_dijkstra_impl<Graph::head_type, Graph::table_type, std::less<>> A_dijkstra_impl;
    A_dijkstra_impl aDijkstraImpl(graph.head, graph.table);

    aDijkstraImpl.doit(start);

    auto &path = aDijkstraImpl.path;

    for (int i = 0; i < vtx_num; ++i) {
        cout << path[i].path << ' ';
    }

    cout << endl;

    Vector<int> late_times(vtx_num + 2);

    late_times[end] = path[end].path;
    a_aoe_latest_start_time_dfs(start, graph.head, graph.table, late_times.begin());


    for (int i = 0; i < vtx_num; ++i) {
        cout << late_times[i] << ' ';
    }


}
