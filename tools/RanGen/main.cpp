// -*- Nitetrichor -*-
//===----------------------------- RanGen ---------------------------------===//
//
// main.cpp
//
// Created by Molybdenum on 11/16/21.
// Copyright (c) 2021 Nitetrichor. All rights reserved.
//===----------------------------------------------------------------------===//

#include "LearnKit/Random.hpp"
#include "LearnKit/UnionFindSet.hpp"
#include <iostream>
#include <unordered_set>
using std::cout, std::endl;

struct hash_for_pair {
    size_t operator()(std::pair<int, int> const &p) const {
        return ((size_t)p.first << 32) | p.second;//NOLINT
    }
};


void test(NC::Random &random) {


    int vtx_num  = random.get(5, 10);
    int dis = random.get(10, 90);

    cout << vtx_num << ' ' << dis << endl;

    for (int i = 0; i < vtx_num; ++i) {
        int x = random.get(0, 100), y = random.get(0, 100);
        cout << x << ' ' << y << endl;
    }

    int o = random.get(vtx_num / 5, vtx_num);

    for (int i = 0; i < o; ++i) {
        cout << 'O' << ' ' << i + 1 << endl;
    }

    int test_num = random.get(5, 10);

    for (int i = 0; i < test_num; ++i) {
        int from = random.get(1, vtx_num);
        int to = random.get(1, vtx_num);
        while (to == from) {
            to = random.get(1, vtx_num);
        }

        cout << "S " << from << ' ' << to << endl;
    }
}

extern "C" {

void shell_sort(int n, int arr[n]) {
    int gap = n;

    do {
        gap = gap / 2;
        for (int i = 0; i < gap; ++i) {

            for (int j = i + gap; j < n; j += gap) {
                int k;
                int tmp = arr[j];
                for (k = j; k - gap >= 0 && arr[k - gap] < tmp ; k -= gap) {
                    arr[k] = arr[k - gap];
                }

                arr[k] = tmp;
            }

        }

//        for (int i = 0; i < n; ++i) {
//            printf(i == 0 ? "%d" : " %d", arr[i]);
//        }
//        puts("");


    } while (gap != 1);
}
#define swap(a, b) \
	do { __auto_type __tmp = (a); (a) = (b); (b) = __tmp; } while (0)



void quick_sort(int size, int arr[size], int n, int outArr[n]) {
    // has one or none
    if (size == 0 || size == 1) {
        return;
    }

    // use central element as pivot
    int  pivot = arr[0];
    int * lCur = arr;
    int * rCur = arr + size - 1;

    while (lCur < rCur) {
        while (lCur < rCur && *rCur >= pivot) {
            --rCur;
        }
        *lCur = *rCur;
        while (lCur < rCur && *lCur <= pivot) {
            ++lCur;
        }
        *rCur = *lCur;
    }
    *lCur = pivot;

//    for (int i = 0; i < n; ++i) {
//        printf(i == 0 ? "%d" : " %d", outArr[i]);
//    }
//    puts("");

    // now rCur < lCur
    // include *rCur, so add extra 1
    quick_sort((int) (rCur - arr), arr, n, outArr);
    quick_sort((int) (size - (lCur - arr) - 1), lCur + 1, n, outArr);
}


void insert_sort(int n, int arr[n]) {


    for (int i = 1; i < n; ++i) {
        int k;
        int tmp = arr[i];
        for (k = i; k - 1 >= 0 && arr[k - 1] > tmp ; --k) {
            arr[k] = arr[k - 1];
        }

        arr[k] = tmp;

    }

}

}

template<typename Func>
class Method {};
template<typename Ret, typename ...Params>
class Method<Ret(Params...)> {

    class Undefined_class; // here is the hack

    typedef Ret(Undefined_class::*method_type)(Params...);
    method_type method;

public:
    template<typename Class>
    constexpr explicit Method(Ret(Class::*a_method)(Params...)) : method((Ret(Undefined_class::*)(Params...))a_method) {}

    inline Ret operator() (void *self, Params... params) {
        return (((Undefined_class*)(self))->*method)(params...);
    }
};


template<typename Iter1 , typename Iter2, typename Iter3, typename Comp>
inline void merge_sort_merge(Iter1 &&begin1, Iter1 &&end1, Iter2 &&begin2, Iter2 &&end2, Iter3 &&out, Comp &&comp) {
    while (begin1 < end1 && begin2 < end2) {
        *out++ = comp(*begin1, *begin2) ? std::move(*begin1++) : std::move(*begin2++);
    }
    while (begin1 < end1) {
        *out++ = std::move(*begin1++);
    }
    while (begin2 < end2) {
        *out++ = std::move(*begin2++);
    }
}

template<typename Iter, typename Comp = std::less<typename std::iterator_traits<Iter>::value_type>>
void merge_sort(Iter &&begin, Iter &&end, Comp &&comp = Comp()) {
    typedef typename std::iterator_traits<Iter>::value_type value_type;
    int len = end - begin;
    std::vector<value_type> temp(len);
    bool inTemp = false;
    for (int seg = 1; seg < len; seg += seg) {
        for (int start = 0; start < len; start += seg + seg) {
            int low = start, mid = std::min(start + seg, len), high = std::min(start + seg + seg, len);

            if (inTemp) {
                merge_sort_merge(temp.begin() + low, temp.begin() + mid, temp.begin() + mid, temp.begin() + high, begin + low, comp);
            } else {
                merge_sort_merge(begin + low, begin + mid, begin + mid, begin + high, temp.begin() + low, comp);
            }
        }

        inTemp = !inTemp;

    }
    if (inTemp) {
        for (int i = 0; i < len; i++) {
            begin[i] = std::move(temp[i]);
        }
    }
}


template<typename Iter, typename Comp = std::less<typename std::iterator_traits<Iter>::value_type>>
void sift_down(Iter begin, Iter end, Iter start, Comp comp = Comp()) {
    --end;
    auto val = *start;
    Iter cur = start;
    while (begin + (cur - begin) * 2 + 1 <= end) {
        Iter child = begin + (cur - begin) * 2 + 1;
        if (child + 1 <= end && comp(*child, *(child + 1))) {
            ++child;
        }
        if (comp(*child, val)) {
            break;
        }
        // let the child up
        *cur = *child;
        cur = child;
    }
    *cur = val;
}


template<typename Iter, typename Comp = std::less<typename std::iterator_traits<Iter>::value_type>>
inline void heap_pop(Iter begin, Iter end, Comp comp = Comp()) {
    std::iter_swap(begin, --end);
    sift_down(begin, end, begin, comp);
}




template<typename Iter, typename Comp = std::less<typename std::iterator_traits<Iter>::value_type>>
void heap_make(Iter begin, Iter end, Comp comp = Comp()) {
    int len = end - begin;
    if (len > 1) {
        for (int i = (len - 2) / 2; i >= 0; --i) {
            sift_down(begin, end, begin + i, comp);
        }
    }
}


bool test2(NC::Random &random) {
    std::vector<int> vec;

    std::generate_n(std::back_inserter(vec), 100, [&]{ return random.get(0, 100); });

    auto origin = vec;

    auto copy = vec;


    heap_make(vec.begin(), vec.end());

    for (int i = 0; i < (int)vec.size() - 1; ++i) {
        heap_pop(vec.begin(), vec.end() - i);
    }

    //    merge_sort(vec.begin(), vec.end());

    std::sort(copy.begin(), copy.end());
    bool good = vec == copy;

    if (!good) {
        for (int i : origin) {
            cout << i << ' ';
        }
        cout << endl;

        for (int i : vec) {
            cout << i << ' ';
        }
        cout << endl;

        for (int i : copy) {
            cout << i << ' ';
        }
        cout << endl;
    }

    cout << std::boolalpha << good << endl;
    return good;
}

int main(int  /*argc*/, const char * /*argv*/[]) {

    NC::Random random;
    using std::cin;
    int time;
    cin >> time;
    for (int i = 0; i < time; ++i) {
        if (!test2(random)) {
            break;
        }
    }

    //    int round = random.get(5, 20);
//    cout << round << endl;
//    for (int i = 0; i < round; ++i) {
//        test(random);
//    }

}