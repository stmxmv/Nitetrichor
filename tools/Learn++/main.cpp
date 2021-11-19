// -*- Nitetrichor -*-
//===--------------------------- Learn ++ ---------------------------------===//
//
// main.cpp
//
// Created by Molybdenum on 8/14/21.
//
//===----------------------------------------------------------------------===//

#if defined(__GNUG__) && !defined(__clang__)
#include <bits/stdc++.h>
#else

#include <algorithm>
#include <array>
#include <complex>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#endif


//#define SIMULATE_OJ


#ifdef WN_NOT_OJ
#ifndef SIMULATE_OJ
//        #include "LearnKit/LearnKit.hpp"
#endif


#endif// WN_NOT_OJ

#ifdef SIMULATE_OJ
namespace NC {


}

#endif

#ifndef __asm_volatile_goto
#define __asm_volatile_goto(x...) asm goto(x)
#endif
#ifndef __always_inline
#define __always_inline inline __attribute__((always_inline))
#endif
#ifndef __noinline
#define __noinline __attribute__((noinline))
#endif
#ifndef __stringify_1
#define __stringify_1(x...) #x
#endif
#ifndef __stringify
#define __stringify(x...) __stringify_1(x)
#endif
#ifndef __same_type
#define __same_type(a, b) std::is_same<typeof(a), typeof(b)>::value
#endif


#define None ((void)0)

//#define FASTIO

#if (!defined(WN_NOT_OJ) || defined(SIMULATE_OJ)) && defined(FASTIO)
using NC::cin;
using NC::cout;
using NC::endl;
using NC::getline;
using ostream = NC::std_IO;
using istream = NC::std_IO;
#else
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::istream;
using std::ostream;
#endif

namespace NC {
#define NC_EXPECT(exp, c) __builtin_expect(exp, c)
#define NC_LIKELY(x)      __builtin_expect(!!(x), 1)
#define NC_UNLIKELY(x)    __builtin_expect(!!(x), 0)
#define POP_COUNT(x)      __builtin_popcount(x)
//static __always_inline bool IsLower(char ch) { return ch >= 'a' && ch <= 'z'; }
//static __always_inline bool IsAlpha(char ch) { return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'); }
//static __always_inline bool IsDigit(char ch) { return ch >= '0' && ch <= '9'; }
//static __always_inline char ToLower(char ch) { return IsAlpha(ch) ? IsLower(ch) ? ch : (char) ('a' + ch - 'A') : ch; }
//struct PairHash {
//    template<typename T, typename U>
//    __always_inline std::size_t operator()(const std::pair<T, U> &x) const {
//        return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
//    }
//};

}// namespace NC


template<typename T = int>
class UnionFindSet {
    std::vector<T> parent;
    std::vector<T> rank;
    static_assert(std::is_integral<T>::value, "integral require");

public:
    constexpr UnionFindSet() noexcept : parent(), rank() {}

    void init(T n) noexcept {
        parent.resize(n);
        rank.resize(n);
        for (T i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    T find(T x) noexcept {
        while (x != parent[x]) {
            x = parent[x] = parent[parent[x]];
        }
        return x;
        //        if (parent[x] == x) {
        //            return x;
        //        } else {
        //            return parent[x] = find(parent[x]);
        //        }
    }

    void unite(T x, T y) noexcept {
        x = find(x);
        y = find(y);
        if (x == y) { return; }
        if (rank[x] < rank[y]) {
            parent[x] = y;
        } else {
            parent[y] = x;
            if (rank[x] == rank[y]) {
                ++rank[x];
            }
        }
    }

    inline bool isSame(T x, T y) noexcept { return find(x) == find(y); }

    bool isAllSame() {
        int x = find(0);
        for (int i = 1; i < (int)parent.size(); ++i) {
            if (x != find(i)) {
                return false;
            }
        }
        return true;
    }

    int kindCount() {
        int ret = 0;
        std::unordered_set<T> set;
        for (int i = 0; i < (int)parent.size(); ++i) {
            int kind;
            if (set.find(kind = find(i)) == set.end()) {
                set.insert(kind);
                ++ret;
            }
        }
        return ret;
    }
};

template<typename T>
struct Edge0 {
    T from, to, cost;
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


template<typename T>
struct Edge1 {
    T to, cost, next;
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


/// \brief use floyd algorithm
/// \tparam Matrix must point to a matrix, don't use VLA, it will compile error
/// \param mtx
template<typename Matrix>
void m_transform_adjacency_to_reachability(int n, Matrix& mtx) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                if (mtx[j][k] < mtx[j][i] * mtx[i][k]) {
                    mtx[j][k] = mtx[j][i] * mtx[i][k];
                }
            }
        }
    }
}


int main() {
    // main entry here

    //#define REDIRECT


#ifdef WN_NOT_OJ
#ifdef REDIRECT
    freopen("OJ/in.txt", "r", stdin);
    freopen("OJ/out.txt", "w", stdout);
#else
#ifndef FASTIO
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
#endif
#endif// REDIRECT
#else
#ifndef FASTIO
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
#endif
#endif// WN_NOT_OJ


    typedef Basic_graph1<int> prim_graph;
    typedef Basic_graph0<int> Kruskal_graph;


    int t;
    cin >> t;
    while (t--) {
        int vtx_num, edge_num;
        cin >> vtx_num >> edge_num;
        std::string vtx_names[vtx_num];
        std::unordered_map<std::string, int> map;
        for (int i = 0; i < vtx_num; ++i) {
            cin >> vtx_names[i];
            map[vtx_names[i]] = i;
        }

        std::vector<std::vector<double>> mtx(vtx_num, std::vector<double>(vtx_num));

        for (int i = 0; i < edge_num; ++i) {
            double rate;
            std::string from_str, to_str;
            cin >> from_str >> rate >> to_str;
            int from = map[from_str], to = map[to_str];
            mtx[from][to] = rate;
        }

        m_transform_adjacency_to_reachability(vtx_num, mtx);

        for (int i = 0; i < vtx_num; ++i) {
            if (mtx[i][i] > 1) {
                cout << "YES" << endl;
                goto next_test;
            }
        }

        cout << "NO" << endl;

    next_test:
        None;
    }



    return 0;
}
