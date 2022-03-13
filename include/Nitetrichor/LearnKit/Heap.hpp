/*!
* Created by Molybdenum on 9/28/21.
*
*/
#ifndef NITETRICHOR_HEAP_HPP
#define NITETRICHOR_HEAP_HPP
#include <functional>
#include "wrap_array.hpp"

namespace AN {

template<typename Iter, typename Comp = std::less<typename std::iterator_traits<Iter>::value_type>>
void heap_push(Iter &&begin, Iter &&end, Comp &&comp = Comp()) {
    --end;
    auto val = *end;
    while (end > begin) {
        Iter parent = begin + (end - begin - 1) / 2; // the parent node
        if (comp(val, *parent)) {
            break;
        }
        *end = *parent; // let the parent down
        end = parent;
    }
    *end = val;
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



}

namespace NC {




template<typename Iter, typename Comp>
void Heap_push(Iter begin, Iter end, Comp comp = std::less<>()) {
    --end;
    auto val = *end;
    while (end > begin) {
        Iter parent = begin + (end - begin - 1) / 2; // the parent node
        if (comp(val, *parent)) {
            break;
        }
        *end = *parent; // let the parent down
        end = parent;
    }
    *end = val;
}


template<typename Iter, typename Comp>
void Heap_pop(Iter begin, Iter end, Comp comp = std::less<>()) {
    --end;
    auto val = *end;
    Iter cur = begin;
    while (begin + (cur - begin) * 2 + 1 < end) {
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



//template<typename Iter>
//void Heap_make(Iter begin, Iter end) {
//
//
//
//}

template<typename T, int Size, typename Comp = std::less<>, typename Container = NC::Array<T, Size>>
class PriorityQueue {
    Container container;
public:

    constexpr PriorityQueue() noexcept : container{} {}

    void push(const T& item) noexcept {
        container.push_back(item);
        Heap_push(container.begin(), container.end(), Comp());
    }

    void push(T&& item) noexcept {
        container.push_back(item);
        Heap_push(container.begin(), container.end(), Comp());
    }

    [[nodiscard]] const T& top() const noexcept { return container.front(); }

    void pop() noexcept {
        Heap_pop(container.begin(), container.end(), Comp());
        container.pop_back();
    }

};







}

#endif //NITETRICHOR_HEAP_HPP
