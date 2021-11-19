/*!
* Created by Molybdenum on 9/28/21.
*
*/
#ifndef NITETRICHOR_UNIONFINDSET_HPP
#define NITETRICHOR_UNIONFINDSET_HPP

#include <vector>
#include <unordered_set>
namespace NC {

namespace dyn {


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
        while(x != parent[x]) {
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




}

template<int Size, typename T = int>
class UnionFindSet {
    T parent[Size];
    T rank[Size];
    static_assert(std::is_integral<T>::value, "integral require");
public:
    constexpr UnionFindSet() noexcept : parent{}, rank{} {}

    void init(T n) noexcept {
        for (T i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    T find(T x) noexcept {
        while(x != parent[x]) {
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

};



}

#endif //NITETRICHOR_UNIONFINDSET_HPP
