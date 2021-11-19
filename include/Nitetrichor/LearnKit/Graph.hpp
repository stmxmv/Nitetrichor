/*!
* Created by Molybdenum on 9/28/21.
*
*/
#ifndef NITETRICHOR_GRAPH_HPP
#define NITETRICHOR_GRAPH_HPP

#include "LearnKit/UnionFindSet.hpp"
#include <stack>
namespace NC {




template<typename T>
struct Edge0 {
    T from, to, cost;
};


template<typename T>
struct Edge1 {
    T to, cost, next;
};



namespace dyn {



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


template<typename Matrix>
class m_gabow_impl {
    typedef matrix_value_t<Matrix> value_type;

    template<typename T>
    using Vector = std::vector<T>;

    Matrix& matrix;
public:
    typedef Vector<Vector<value_type>> component_type;
    explicit m_gabow_impl(Matrix& m) : matrix(m), time(std::size(m)), stk1(), stk2(), scc(std::size(m)), scc_num(), tick() {}

    Vector<value_type> time;
    std::stack<value_type> stk1;
    std::stack<value_type> stk2;
    Vector<value_type> scc;
    int scc_num;
    int tick;
    Vector<Vector<value_type>> component;
    void doit(int cur) {
        int i;
        time[cur] = ++tick;
        stk1.push(cur);
        stk2.push(cur);
        for(i = 0; i < (int) std::size(matrix); ++i) {
            if (matrix[cur][i]) {
                if(!time[i]) {
                    doit(i);
                } else if (0 == scc[i]) {
                    while(time[stk2.top()] > time[i]) {
                        stk2.pop();
                    }
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

template<typename Matrix>
class m_tarjan_impl {
    typedef matrix_value_t<Matrix> value_type;

    template<typename T>
    using Vector = std::vector<T>;
    Matrix& matrix;
public:
    typedef Vector<Vector<value_type>> component_type;
    Vector<Vector<value_type>> component;
    Vector<value_type> dfn;
    Vector<value_type> low;
    Vector<value_type> scc;
    std::stack<value_type> stk;
    int scc_num;
    int tick;

    explicit m_tarjan_impl(Matrix& m) : matrix(m), dfn(std::size(m)), low(std::size(m)), scc(std::size(m)), stk(), scc_num(), tick() {}

    void doit(int n) {
        dfn[n] = low[n] = ++tick;
        stk.push(n);
        for (int i = 0; i < (int) std::size(matrix); ++i) {
            if (matrix[n][i]) {
                if (dfn[i] == 0) {
                    doit(i);
                    low[n] = std::min(low[n], low[i]);
                } else if (scc[i] == 0) {
                    low[n] = std::min(low[n], dfn[i]);// can be Low[i]=min(Low[i],Low[j]) only in strong connected
                }
            }
        }
        if (dfn[n] == low[n]) {
            value_type top;
            ++scc_num;
            component.emplace_back();
            do {
                top = stk.top();
                stk.pop();
                scc[top] = scc_num;
                component.back().push_back(top);
                //            InComponent[j] = ComponentNumber;
            } while (top != n);
        }
    }

};

template<typename T>
class Basic_Graph2 {

    template<typename U>
    using Vector = std::vector<U>;

    Vector<Vector<T>> matrix; //adjacency matrix

public:
    Basic_Graph2() = default;
    explicit Basic_Graph2(int size) : matrix(size, Vector<T>(size)) {}
    Basic_Graph2(std::initializer_list<Vector<T>> list) : matrix(list) {}

    auto &operator[](int idx) { return matrix[idx]; }

    //    auto evaluateConnectedComponent() -> typename m_tarjan_impl<Vector<Vector<T>>>::component_type {
    //        m_tarjan_impl<Vector<Vector<T>>> tarjanImpl(matrix);
    //        for (int i = 0; i < (int) matrix.size(); ++i) {
    //            if (!tarjanImpl.dfn[i]) {
    //                tarjanImpl.doit(i);
    //            }
    //        }
    //        return std::move(tarjanImpl.component);
    //    }

    auto evaluateConnectedComponent() -> typename m_gabow_impl<Vector<Vector<T>>>::component_type {
        m_gabow_impl<Vector<Vector<T>>> gabowImpl(matrix);
        for (int i = 0; i < (int) matrix.size(); ++i) {
            if (!gabowImpl.time[i]) {
                gabowImpl.doit(i);
            }
        }
        return std::move(gabowImpl.component);
    }
};


/// \brief store edges in a array
template<typename T = int>
class Basic_graph0 {
    std::vector<Edge0<T>> edges;
    T numOfVertexes, numOfEdges;


    constexpr static T INF = 0x3f3f3f3f3f3f3f3f;

public:
    std::vector<Edge0<T>> spanningTree;

    constexpr Basic_graph0() noexcept : edges(), numOfVertexes(), numOfEdges() {}

    explicit Basic_graph0(T vertex) : edges(), numOfVertexes(vertex) {}

    void clear() noexcept {
        edges.clear();
        numOfVertexes = 0;
        numOfEdges = 0;
    }

    inline void setNumOfVertexes(T vertexes) { numOfVertexes = vertexes; }

    inline void insert(T from, T to, T cost) {
        edges.push_back({.from = from, .to = to, .cost = cost});
        ++numOfEdges;
    }


    /// \brief Kruskal, require findSet
    /// \param start
    /// \return the sum of the path
    T evaluateMinimumSpanningTree() {
        UnionFindSet<T> findSet;
        T ret = 0, cnt = 0;
        findSet.init(numOfVertexes);
        std::sort(edges.begin(), edges.end(), [](const Edge0<T> &lhs, const Edge0<T> &rhs) {
            return lhs.cost < rhs.cost;
        });

        for (T i = 0; i < numOfEdges; ++i) {
            if (findSet.isSame(edges[i].from, edges[i].to)) {
                // bridged
                continue;
            }
            ret += edges[i].cost;
            spanningTree.push_back({.from = edges[i].from, .to = edges[i].to, .cost = edges[i].cost});

            findSet.unite(edges[i].from, edges[i].to);

            ++cnt;
            if (cnt == numOfVertexes - 1) {
                break;
            }

        }
        if (cnt == numOfVertexes - 1) {
            return ret;
        }
        return 0;
    }
};

/// \brief store edges in a adjacency table
template<typename T = int>
class Basic_graph1 {
    std::vector<Edge1<T>> table;// Adjacency table
    T cnt;
    std::vector<T> head;
    std::vector<bool> vis;

    T numOfVertexes, numOfEdges;

public:

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
        : table(1), cnt(), head(), numOfVertexes(vertexes), numOfEdges() {}

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
};


}


/// \brief store edges in a adjacency table
/// \tparam MaxVertexes
/// \tparam MaxEdges
/// \tparam T
template<int MaxVertexes, int MaxEdges, typename T = int>
class Basic_graph1 {
    Edge1<T> table[MaxEdges]; // Adjacency table
    T cnt, head[MaxVertexes];
    bool vis[MaxVertexes];

    T numOfVertexes, numOfEdges;
    T shortestPath[MaxVertexes];

    T cache[MaxVertexes];
    T cache1[MaxVertexes];



public:
    constexpr static T INF = 0x3f3f3f3f3f3f3f3f;

    Basic_graph1() noexcept : table{}, cnt{}, head{}, vis{}, numOfVertexes{}, numOfEdges{}, shortestPath{} {}

    void clear() noexcept { memset(this, 0, sizeof *this); }

    inline void setNumOfVertexes(T vertexes) { numOfVertexes = vertexes; }

    explicit Basic_graph1(T vertexes) noexcept
            :table{}, cnt{}, head{}, vis{}, numOfVertexes(vertexes), numOfEdges{}, shortestPath{}  {}

    inline void insert(T from, T to, T cost) {
        table[++cnt].next = head[from];
        table[cnt].to = to;
        table[cnt].cost = cost;
        head[from] = cnt;
    }

    /// \brief default invoke version 1
    /// \param start
    inline void evaluateShortestPath(T start) { evaluateShortestPath1(start); }

    /// \brief SPFA
    /// \param start
    void evaluateShortestPath0(T start) {
        memset(shortestPath, (int)INF, sizeof shortestPath);
        memset(vis, 0, sizeof vis);
        shortestPath[start] = 0;
        std::queue<T> queue;
        queue.push(start);
        while (!queue.empty()) {
            auto front = queue.front();
            queue.pop();
            vis[front] = false;
            for (T cur = head[front]; cur; cur = table[cur].next) {
                auto edge = table[cur];
                if (shortestPath[edge.to] > shortestPath[front] + edge.cost) {
                    shortestPath[edge.to] = shortestPath[front] + edge.cost;
                    if (!vis[edge.to]) {
                        vis[edge.to] = true;
                        queue.push(edge.to);
                    }
                }
            }
        }
    }

    /// \brief dijkstra
    /// \param start
    void evaluateShortestPath1(T start) {
        memset(shortestPath, (int)INF, sizeof shortestPath);
        memset(vis, 0, sizeof vis);
        shortestPath[start] = 0;
        std::priority_queue<std::pair<T/* shortest path */, T/* vertex */>,
                std::vector<std::pair<T, T>>,
                std::greater<>> queue;
        queue.emplace(0, start);
        while (!queue.empty()) {
            auto front = queue.top();
            queue.pop();
            if (vis[front.second]) {
                continue;
            }
            vis[front.second] = true;
            for (T cur = head[front.second]; cur; cur = table[cur].next) {
                auto edge = table[cur];
                if (shortestPath[edge.to] > shortestPath[front.second] + edge.cost) {
                    shortestPath[edge.to] = shortestPath[front.second] + edge.cost;
                    if (!vis[edge.to]) {
                        queue.emplace(shortestPath[edge.to], edge.to);
                    }
                }
            }
        }
    }

    /// \brief dijkstra, put result in cache1 instead of shortestPath, access it via getShortestPath1(vertex)
    /// \param start vertex
    void evaluateShortestPath2(T start) {
        memset(cache1, (int)INF, sizeof cache1);
        memset(vis, 0, sizeof vis);
        cache1[start] = 0;
        std::priority_queue<std::pair<T/* shortest path */, T/* vertex */>,
                std::vector<std::pair<T, T>>,
                std::greater<>> queue;
        queue.emplace(0, start);
        while (!queue.empty()) {
            auto front = queue.top();
            queue.pop();
            if (vis[front.second]) {
                continue;
            }
            vis[front.second] = true;
            for (T cur = head[front.second]; cur; cur = table[cur].next) {
                auto edge = table[cur];
                if (cache1[edge.to] > cache1[front.second] + edge.cost) {
                    cache1[edge.to] = cache1[front.second] + edge.cost;
                    if (!vis[edge.to]) {
                        queue.emplace(cache1[edge.to], edge.to);
                    }
                }
            }
        }
    }


    // see https://www.luogu.com.cn/problem/P5905
    // think about `potential energy`


    /// \brief adjust cost according to shortestPath
    /// \param vertex
    void adjustCost(T vertex) {
        for (T cur = head[vertex]; cur; cur = table[cur].next) {
            table[cur].cost += shortestPath[vertex] - shortestPath[table[cur].to];
        }
    }

    /// \brief use SPFA to find, use cache.
    /// \param start
    /// \return
    bool findNegativeLoop(T start) {
        // SPFA
        memset(shortestPath, (int)INF, sizeof shortestPath);
        memset(cache, 0, sizeof cache);
        shortestPath[start] = 0;
        std::queue<T> queue;
        queue.push(start);
        while (!queue.empty()) {
            auto front = queue.front();
            queue.pop();
            vis[front] = false;
            for (T cur = head[front]; cur; cur = table[cur].next) {
                auto edge = table[cur];
                if (shortestPath[edge.to] > shortestPath[front] + edge.cost) {
                    shortestPath[edge.to] = shortestPath[front] + edge.cost;
                    if (!vis[edge.to]) {
                        vis[edge.to] = true;
                        queue.push(edge.to);
                        ++cache[edge.to];
                        if (cache[edge.to] == numOfVertexes) {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    /// \brief access cache1, call evaluateShortestPath2(vertex) first
    /// \param to
    /// \return
    inline T getShortestPath1(T to) { return cache1[to]; }

    inline T getShortestPath(T to) { return shortestPath[to]; }

    /// \brief prim, use cache to store distance
    /// \param start
    /// \return the sum of the path
    T evaluateMinimumSpanningTree(T start) {
        memset(cache, INF, sizeof cache);
        memset(vis, 0, sizeof vis);
        T ret = 0;
        T count = 0;
        std::priority_queue<std::pair<T/* shortest cost */, T/* vertex */>,
                std::vector<std::pair<T, T>>,
                std::greater<>> queue;
        queue.emplace(0, start);
        while (!queue.empty() && count < numOfVertexes) {

            auto front = queue.top();
            queue.pop();
            if (vis[front.second]) {
                // already use this node
                continue;
            }

            ++count;
            ret += front.first;
            vis[front.second] = true;
            // slack
            for (auto cur = head[front.second]; cur; cur = table[cur].next) {
                if (table[cur].cost < cache[table[cur].to]) {
                    cache[table[cur].to] = table[cur].cost;
                    queue.emplace(table[cur].cost, table[cur].to);
                }
            }
        }

        if (count == numOfVertexes) {
            return ret;
        }
        return 0;
    }


};


/// \brief store edges in a array
/// \tparam MaxVertexes
/// \tparam MaxEdges
/// \tparam T
template<int MaxVertexes, int MaxEdges, typename T = int>
class Basic_graph0 {
    Edge0<T> edges[MaxEdges];
    T numOfVertexes, numOfEdges;

    T shortestPath[MaxVertexes];

    constexpr static T INF = 0x3f3f3f3f3f3f3f3f;

public:
    constexpr Basic_graph0() noexcept : edges{}, numOfVertexes{}, numOfEdges{}, shortestPath{} {}

    explicit Basic_graph0(T vertex) : edges{}, numOfVertexes(vertex), shortestPath{} {}

    void clear() noexcept {
        memset(this, 0, sizeof *this);
    }

    inline void setNumOfVertexes(T vertexes) { numOfVertexes = vertexes; }

    inline void insert(T from, T to, T cost) { edges[numOfEdges++] = {from, to, cost}; }

    /// \brief Ford
    /// \param start
    void evaluateShortestPath(T start) {
        memset(shortestPath, (int)INF, sizeof shortestPath);
        shortestPath[start] = 0;

        for (;;) {
            bool update = false;
            for (int i = 0; i < numOfEdges; ++i) {
                auto edge = edges[i];
                if (shortestPath[edge.from] != INF && shortestPath[edge.to] > shortestPath[edge.from] + edge.cost) {
                    shortestPath[edge.to] = shortestPath[edge.from] + edge.cost;
                    update = true;
                }
            }
            if (!update) {
                break;
            }
        }
    }

    inline T getShortestPath(T to) { return shortestPath[to]; }



    bool findNegativeLoop(T start) {
        // Ford
        memset(shortestPath, (int)INF, sizeof shortestPath);
        shortestPath[start] = 0;

        for (int i = 0; i < numOfVertexes; ++i) {
            for (int j = 0; j < numOfEdges; ++j) {
                auto edge = edges[j];
                if (shortestPath[edge.from] != INF && shortestPath[edge.to] > shortestPath[edge.from] + edge.cost) {
                    shortestPath[edge.to] = shortestPath[edge.from] + edge.cost;
                    if (i == numOfVertexes - 1) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool findAllNegativeLoop() {
        // Ford
//        memset(shortestPath, 0, sizeof shortestPath);
        std::fill(shortestPath, shortestPath + numOfVertexes + 1, 0);

        for (int i = 0; i < numOfVertexes; ++i) {
            for (int j = 0; j < numOfEdges; ++j) {
                auto edge = edges[j];
                if (shortestPath[edge.to] > shortestPath[edge.from] + edge.cost) {
                    shortestPath[edge.to] = shortestPath[edge.from] + edge.cost;
                    if (i == numOfVertexes - 1) {
                        return true;
                    }
                }
            }
        }
        return false;
    }



    UnionFindSet<MaxVertexes> findSet;
    /// \brief Kruskal, use findSet
    /// \param start
    /// \return the sum of the path
    T evaluateMinimumSpanningTree() {
        T ret = 0, cnt = 0;
        findSet.init(numOfVertexes);
        std::sort(edges, edges + numOfEdges, [](const Edge0<T> &lhs, const Edge0<T> &rhs) {
            return lhs.cost < rhs.cost;
        });

        for (T i = 0; i < numOfEdges; ++i) {
            if (findSet.isSame(edges[i].from, edges[i].to)) {
                // bridged
                continue;
            }
            ret += edges[i].cost;

            findSet.unite(edges[i].from, edges[i].to);

            ++cnt;
            if (cnt == numOfVertexes - 1) {
                break;
            }

        }
        if (cnt == numOfVertexes - 1) {
            return ret;
        }
        return 0;
    }
};









}

#endif //NITETRICHOR_GRAPH_HPP
