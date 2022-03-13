/*!
* Created by Molybdenum on 9/2/21.
*
*/
#ifndef NITETRICHOR_SORT_HPP
#define NITETRICHOR_SORT_HPP

#include <functional>
#include <algorithm>
namespace AN {



template<typename T, typename Comp>
void __r_merge_sort(T arr[], T reg[], int start, int end, const Comp &comp, const int n) {
    if (start >= end) { return; }

    int len = end - start, mid = len / 2 + start;
    int start1 = start, end1 = mid;
    int start2 = mid + 1, end2 = end;
    __r_merge_sort(arr, reg, start1, end1, comp, n);
    __r_merge_sort(arr, reg, start2, end2, comp, n);

    int k = start;
    while (start1 <= end1 && start2 <= end2) {
        reg[k++] = comp(arr[start1], arr[start2]) ? arr[start1++] : arr[start2++];
    }
    while (start1 <= end1) {
        reg[k++] = arr[start1++];
    }
    while (start2 <= end2) {
        reg[k++] = arr[start2++];
    }
    for (k = start; k <= end; k++) {
        arr[k] = reg[k];
    }


//    for (int i = 0; i < n; ++i) {
//        cout << (i == 0 ? "" : " ") << arr[i];
//    }
//    cout << endl;
}

template<typename T, typename Comp = std::less<T>>
void r_merge_sort(T arr[], const int len, Comp &&comp = Comp()) {
    T reg[len];
    __r_merge_sort(arr, reg, 0, len - 1, std::forward<Comp>(comp), len);
}


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

//        /// verbose print
//        for (int i = 0; i < len; ++i) {
//            if (i) {
//                cout << ' ';
//            }
//            if (inTemp) {
//                cout << temp[i];
//            } else {
//                cout << begin[i];
//            }
//        }
//        cout << endl;

    }
    if (inTemp) {
        for (int i = 0; i < len; i++) {
            begin[i] = std::move(temp[i]);
        }
    }
}









}



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
