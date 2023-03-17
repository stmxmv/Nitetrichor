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





template<typename T>
using Vector = std::vector<T>;

using Vectori = Vector<int>;


//#include "exam.hpp"



template<typename Iter, typename T>
int an_low_bound(int n, Iter arr, T val) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (val <= arr[mid]) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return left;
}




template<typename Iter, typename T>
int _an_low_bound(int n, Iter arr, T val) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (val <= arr[mid]) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return left;
}

template<typename T>
struct an_list {
    struct node {
        T data;
        std::weak_ptr<node> parent;
        std::shared_ptr<node> child;
        node() = default;
        node(T data) : data(data) {}
    };


    struct wrap_iter : std::iterator<std::bidirectional_iterator_tag, T> {
        std::shared_ptr<node> ptr;

        wrap_iter(std::shared_ptr<node> &node) : ptr(node) {}


        wrap_iter& operator ++() {
            ptr = ptr->child;
            return *this;
        }

        wrap_iter operator ++(int) {
            auto ret = *this;
            ++(*this);
            return ret;
        }

        wrap_iter& operator --() {
            ptr = ptr->parent.lock();
            return *this;
        }

        wrap_iter operator --(int) {
            auto ret = *this;
            --(*this);
            return ret;
        }

        bool operator==(const wrap_iter &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator!=(const wrap_iter &rhs) const {
            return !(*this == rhs);
        }

        T  &operator*() {
            return ptr->data;
        }


    };

    std::shared_ptr<node> root;

    an_list() : root(std::make_shared<node>()) {
        root->parent = root;
        root->child = root;
    }


    ~an_list() {
        root->child = nullptr;
    }

    void __insert(std::shared_ptr<node> &cur, T data) {

        std::shared_ptr<node> new_node = std::make_shared<node>(data);

        new_node->child = cur;

        new_node->parent = cur->parent;

        cur->parent.lock()->child = new_node;

        cur->parent = new_node;
    }

    void insert(wrap_iter iter, T data) {
        __insert(iter.ptr, data);
    }

    void push_back(T data) {
        __insert(root, data);
    }


    wrap_iter begin() { return wrap_iter(root->child); }

    wrap_iter end() { return wrap_iter(root); }

};

#include "exam.hpp"

template<typename Iter>
void an_insert_sort(int n, Iter arr) {

    for (int i = 1; i < n; ++i) {
        int k = i;
        int tmp = arr[k];
        for ( ; k - 1 >= 0 && arr[k - 1] > tmp; --k) {
            arr[k] = arr[k - 1];
        }

        arr[k] = tmp;
    }
}


template<typename Iter>
void an_shell_sort(int n, Iter arr) {
    int gap = n;
    do {
        gap = gap / 2;

        for (int i = 0; i < gap; ++i) {

            for (int j = i + gap; j < n; j += gap) {
                int k = j;
                int tmp = arr[k];
                for ( ; k - gap >= 0 && arr[k - gap] > tmp; k -= gap) {
                    arr[k] = arr[k - gap];
                }
                arr[k] = tmp;
            }

        }



    } while (gap != 1);

}


template<typename Iter>
void _an_shell_sort(int n, Iter arr) {
    int gap = n;
    do {

        gap = gap / 2;
        for (int i = 0; i < gap; ++i) {
            for (int j = i + gap; j < n; j += gap) {
                int k = j;
                int tmp = arr[k];

                for (; k - gap >= 0 && arr[k - gap] > tmp; k -= gap) {

                    arr[k] = arr[k - gap];
                }

                arr[k] = tmp;

            }
        }


    } while (gap != 1);

}


template<typename Iter>
void _an_insert_sort(int n, Iter arr) {
    for (int i = 1; i < n; ++i) {
        int k = i;
        int tmp = arr[k];
        for (; k - 1 >= 0 && arr[k - 1] > tmp; --k) {
            arr[k] = arr[k - 1];
        }
        arr[k] = tmp;
    }
}


template<typename Iter>
void an_quick_sort(int n, Iter arr) {

    if (n == 0 || n == 1) { return ; }

    int left = 0;
    int right = n - 1;

    int pivot = arr[0];

    while (left < right) {

        while (left < right && arr[right] >= pivot) {
            --right;
        }

        arr[left] = arr[right];


        while (left < right && arr[left] <= pivot) {
            ++left;
        }

        arr[right] = arr[left];

    }

    assert(left == right);


    arr[left] = pivot;

    an_quick_sort(left, arr);

    an_quick_sort(n - left - 1, arr + left + 1);

}

template<typename Iter>
void _an_quick_sort(int n, Iter arr) {
    if (n == 0 || n == 1) { return ; }


    int left = 0, right = n - 1;
    int pivot = arr[0];
    while (left < right) {
        while (left < right && arr[right] >= pivot) {
            --right;
        }

        arr[left] = arr[right];

        while (left < right && arr[left] <= pivot) {
            ++left;
        }

        arr[right] = arr[left];
    }

    arr[left] = pivot;

    _an_quick_sort(left, arr);

    _an_quick_sort(n - left - 1, arr + left + 1);



}

template<typename Iter>
void an_sift_down(int n, Iter arr, int start) {
    auto val = arr[start];
    int cur = start;

    /// has child
    while (cur * 2 + 1 < n) {
        int child = cur * 2 + 1;
        if (child + 1 < n && arr[child] < arr[child + 1]) {
            ++child;
        }
        if (arr[child] < val) {
            break;
        }
        // let the child up
        arr[cur] = arr[child];
        cur = child;
    }
    arr[cur] = val;
}


template<typename Iter>
void an_sift_up(int n, Iter arr, int start) {
    auto val = arr[start];
    int cur = start;

    /// has parent
    while (cur > 0) {
        int parent = (cur - 1) / 2;

        if (arr[parent] > val) {
            break;
        }
        // let the parent up
        arr[cur] = arr[parent];
        cur = parent;
    }
    arr[cur] = val;
}

template<typename Iter>
void _an_sift_up(int n, Iter arr, int start) {
    int val = arr[start];
    int cur = start;

    while (cur > 0) {
        int parent = (cur - 1) / 2;

        if (val < arr[parent]) { break; }

        arr[cur] = arr[parent];
        cur = parent;
    }


    arr[cur] = val;
}
template<typename Iter>
void an_heap_push(int n, Iter arr) {
    _an_sift_up(n, arr, n - 1);
}

template<typename Iter>
void an_heap_make(int n, Iter arr) {
    if (n <= 1) { return; }
    for (int i = (n - 2) / 2; i >= 0; --i) {
        an_sift_down(n ,arr, i);
    }
}


template<typename Iter>
void an_heap_pop(int n, Iter arr) {
    std::iter_swap(arr, arr + n - 1);
    an_sift_down(n - 1, arr, 0);
}


template<typename Iter>
void _an_sift_down(int n, Iter arr, int start) {
    int val = arr[start];
    int cur = start;

    while (cur * 2 + 1 < n) {
        int child = cur * 2 + 1;
        if (child + 1 < n && arr[child + 1] > arr[child] ) {
            ++child;
        }


        if (val > arr[child]) { break; }

        arr[cur] = arr[child];

        cur = child;

    }


    arr[cur] = val;
}





template<typename Iter>
void _an_heap_make(int n, Iter arr) {
    if (n <= 1) { return; }
    for (int i = (n - 2) / 2; i >= 0; --i) {
        _an_sift_down(n, arr, i);
    }
}


template<typename Iter, typename Callback>
void an_floyd_path(int from,  int to, Iter path, Callback callback) {

    if (from == to) { return; }

    if (path[from][to] == 0) {
        callback(to);
        return;
    }

    an_floyd_path(from, path[from][to], callback);

    an_floyd_path(path[from][to], to, callback);



}


template<typename String>
struct an_kmp {

    int length;
    const String &string;

    Vectori next = {-1, 0};

    void make_next() {
       for (int i = 0, j = 1; j < length;){
           if (string[i] == string[j]) {
               ++i, ++j;
               next.push_back(i);
           } else {
               int fail = next[i];
               if (fail < 0) {
                   i = 0;
                   ++j;
                   next.push_back(0);
               } else {
                   i = fail;
               }
           }
       }
    }

    template<typename ComString>
    int findFirstOne(const ComString &otherString, int otherLength) noexcept {
        if (otherLength > length) { return -1; }

        for (int i = 0, j = 0; i < otherLength; ) {
            if (otherString[i] == string[j]) {
                ++i, ++j;
                if (j == length) {
                    return i - length;
                }
            } else {
                int fail = next[j];
                if (fail < 0) {
                    j = 0;
                    ++i;

                } else {
                    j = fail;
                }

            }
        }

        return -1;
    }


    template<typename ComString, typename OutIter>
    OutIter findAll(const ComString &otherString, int otherLength, OutIter out) noexcept {
        if (otherLength > length) { return -1; }

        for (int i = 0, j = 0; i < otherLength; ) {
            if (otherString[i] == string[j]) {
                ++i, ++j;
                if (j == length) {
                    *out++ = i - length;
                    j = next[length];
                }
            } else {
                int fail = next[j];
                if (fail < 0) {
                    j = 0;
                    ++i;

                } else {
                    j = fail;
                }

            }
        }

        return out;
    }

};

namespace exam {
class DisjointSet {
    Vectori parent;

public:
    explicit DisjointSet(int n) noexcept : parent(n) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }


    int find(int x) noexcept {
        while (x != parent[x]) {
            x = parent[x] = parent[parent[x]];
        }
        return x;
    }

    void unite(int x, int y) noexcept {
        x         = find(x);
        y         = find(y);
        parent[x] = y;
    }

    inline bool isSame(int x, int y) noexcept { return find(x) == find(y); }
};
}

int main() {
    // main entry here

//    #define REDIRECT


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



    exam::DisjointSet set(10);


    set.unite(2, 3);

    set.unite(3, 7);

    set.unite(1, 2);

    cout << set.isSame(1, 3);



//    Vectori vec, copy;
//
//    for (int i = 0; i < 100; ++i) {
//        int rand = arc4random_uniform(100);
//        vec.push_back(rand);
//        copy.push_back(rand);
//        an_heap_push(i + 1, vec.begin());
//        std::push_heap(copy.begin(), copy.end());
//
//        cout << rand <<  ' ';
//    }
//    cout << endl;
//
//
////    std::push_heap(copy.begin(), copy.end());
//
//    v_print(copy);
//
////    an_heap_push(100, vec.begin());
//
////    for (int i = 0; i < 100 - 1; ++i) {
////        an_heap_pop(100 - i, vec.begin());
////    }
//
//    v_print(vec);
//
//    assert(copy == vec);



    return 0;
}

