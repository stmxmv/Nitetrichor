/*!
* Created by Molybdenum on 9/2/21.
*
*/
#ifndef NITETRICHOR_SORT_HPP
#define NITETRICHOR_SORT_HPP

#include <functional>

namespace WN {

/*!
 * @brief
 */
template<typename Iter, typename Comp>
[[deprecated]]
Iter Partition(Iter begin,
               Iter end,
               Iter pivot,
               Comp comp = std::less<typename std::iterator_traits<Iter>::value_type>()) {
    // save pivot as last element
    auto pivotValue = *pivot;
    std::iter_swap(pivot, end - 1);

    // find first comp() return false
    while (true) {
        if (begin == end) {
            return begin;
        }
        if (!comp(*begin, pivotValue)) {
            break;
        }
        ++begin;
    }

    // put item to the end
    for (auto iter = std::next(begin) ; iter != end; ++iter) {
        if (comp(*iter, pivotValue)) {
            std::iter_swap(iter, begin);
            ++begin;
        }
    }

    // let return value point to pivot
    std::iter_swap(begin, end - 1);

    return begin;
}

/*!
 * @brief quick sort using recursion
 * @tparam Iter: Random access iterator
 */
template<typename Iter, typename Comp>
inline void QuickSort(Iter begin,
               Iter end,
               Comp comp) {
    // has one or none
    if (begin == end || begin + 1 == end) {
        return;
    }

    // use central element as pivot
    auto pivot = *std::next(begin, std::distance(begin, end) / 2);
    auto lCur = begin;
    auto rCur = end - 1;

    while (lCur <= rCur) {
        // assume comp is std::less<>(), then find first *lCur < pivot is false
        while (comp(*lCur, pivot)) {
            ++lCur;
        }
        // find last *rCur > pivot is false
        while (comp(pivot, *rCur)) {
            --rCur;
        }

        if (lCur <= rCur) {
            std::iter_swap(lCur, rCur);
            ++lCur;
            --rCur;
        }
    }
    // now rCur < lCur
    // include *rCur, so use ++rCur as terminal
    WN::QuickSort(begin, ++rCur, comp);
    WN::QuickSort(lCur, end, comp);
}

/*!
 * @brief NCQuickSort with default comp std::less<>()
 * @tparam Iter: Random access iterator
 */
template<typename Iter>
inline void QuickSort(Iter begin, Iter end) {
    WN::QuickSort(begin, end, std::less<typename std::iterator_traits<Iter>::value_type>());
}


template<typename Iter, typename Comp = std::less<typename std::iterator_traits<Iter>::value_type>>
inline void LocateNthElement(Iter begin,
                             Iter nth,
                             Iter end,
                             Comp comp = std::less<typename std::iterator_traits<Iter>::value_type>()) {
    if (begin == end || begin + 1 == end) {
        return;
    }
    // use central element as pivot
    auto pivot = *std::next(begin, std::distance(begin, end) / 2);
    auto lCur = begin;
    auto rCur = end - 1;

    while (true) {
        while (comp(*lCur, pivot)) {
            ++lCur;
        }
        // find last *rCur > pivot is false
        while (comp(pivot, *rCur)) {
            --rCur;
        }

        if (lCur <= rCur) {
            std::iter_swap(lCur, rCur);
            ++lCur;
            --rCur;
        } else {
            break;
        }
    }

    // now rCur < lCur
    if (nth <= rCur) {
        WN::LocateNthElement(begin, nth, ++rCur, comp);
    } else if (nth >= lCur) {
        WN::LocateNthElement(lCur, nth, end, comp);
    } else {
        return;
    }
}







}

#endif //NITETRICHOR_SORT_HPP
