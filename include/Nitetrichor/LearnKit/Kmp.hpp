//
// Created by Molybdenum on 8/4/21.
// Copyright (c) 2021 Nitetrichor. All rights reserved.
//

#ifndef LEARNKIT_KMP_H
#define LEARNKIT_KMP_H


#include <vector>
#include <optional>


namespace WN::String {



///\brief
///\tparam Iterator must conform to std::random_access_iterator
///\tparam U the size type
template<typename Iterator, int Size, typename U = int>
class Kmp {
    Iterator begin;
    Iterator end;

    typedef typename std::iterator_traits<Iterator>::iterator_category IteratorCategory;
    using WNSize = U;

    WNSize length;
    WNSize next_size;
    WNSize next[Size] = {-1, 0};

    void _generateNextArray(std::random_access_iterator_tag) {
        // generate `next` array base on 'begin' and 'end' ivars.

        auto iter1 = begin;
        auto iter2 = iter1 + 1;
//            auto endIter = end - 1;
        while (iter2 != end) {
            if (*iter2 == *iter1) {
                ++iter1;
                ++iter2;

//                // like abcxxabc, if the second 'b' doesn't match, we don't need to compare prefix 'ab' again.
//                if (iter2 != end && *iter2 == *iter1) {
//                    next[next_size++] = next[iter1 - begin];
//                } else {
                next[next_size++] = static_cast<WNSize>(iter1 - begin);
//                }


            } else {
                auto nextIndex = next[iter1 - begin];

                if (nextIndex == -1) {
                    iter1 = begin;
                    ++iter2;
                    next[next_size++] = 0;

                } else {

                    iter1 = begin + nextIndex;
                }
            }
        }
    }

public:


    Kmp(Iterator begin, Iterator end) noexcept
            : begin(std::move(begin)), end(std::move(end)), length(static_cast<WNSize>(end - begin)), next_size(2) {
        _generateNextArray(IteratorCategory());
    }

    const WNSize * getNextArray() const noexcept { return next; }

    WNSize getNextSize() const noexcept { return next_size; }

    template<typename T>
    WNSize findFirstOne(const T TBegin, const T TEnd) noexcept {
        if (length > TEnd - TBegin) {
            return -1;
        }
        auto TIter = TBegin;
        auto iter = begin;

        while (TIter != TEnd) {

            if (*TIter != *iter) {

                auto nextIndex = next[iter - begin];

                if (-1 == nextIndex) {
                    iter = begin;

                    ++TIter;

                } else {
                    iter = begin + nextIndex;

                }

            } else {

                ++iter;
                ++TIter;

                if (iter == end) {
                    return static_cast<WNSize>(TIter - TBegin - length);
                }
            }
        }

        return -1;
    }

    /// \brief
    /// \result end of result
    template<typename T, typename OutIter>
    OutIter findAll(const T TBegin, const T TEnd, OutIter out) noexcept {
        if (length > TEnd - TBegin) {
            return out;
        }
        auto TIter = TBegin;
        auto iter = begin;

        while (TIter != TEnd) {

            if (*TIter != *iter) {

                auto nextIndex = next[iter - begin];

                if (-1 == nextIndex) {
                    iter = begin;

                    ++TIter;

                } else {
                    iter = begin + nextIndex;

                }

            } else {

                ++iter;
                ++TIter;

                if (iter == end) {
                    *out++ = static_cast<WNSize>(TIter - TBegin - length);

                    iter = begin + next[next_size - 1];

                }
            }
        }

        return out;
    }

};

namespace dynamic {
template<typename Iter, typename T = int>
class Kmp {
    Iter begin;
    Iter end;

    typedef typename std::iterator_traits<Iter>::iterator_category IteratorCategory;

    T length;
    T next_size;
    std::vector<T> next/* = {-1, 0}*/;

    void _generateNextArray(std::random_access_iterator_tag) {

        auto iter1 = begin;
        auto iter2 = iter1 + 1;
        while (iter2 != end) {
            if (*iter2 == *iter1) {
                ++iter1;
                ++iter2;
//////                // like abcxxabc, if the second 'b' doesn't match, we don't need to compare prefix 'ab' again.
////                if (iter2 != end && *iter2 == *iter1) {
////                    next[next_size++] = next[iter1 - begin];
////                } else {
                next[next_size++] = static_cast<T>(iter1 - begin);
//                }

            } else {
                auto nextIndex = next[iter1 - begin];

                if (nextIndex == -1) {
                    iter1 = begin;
                    ++iter2;
                    next[next_size++] = 0;

                } else {

                    iter1 = begin + nextIndex;
                }
            }
        }
    }

public:


    Kmp(Iter begin, Iter end) noexcept
            : begin(std::move(begin)), end(std::move(end)), length(static_cast<T>(end - begin)), next_size(2), next(length + 1) {
        next[0] = -1;
        _generateNextArray(IteratorCategory());
    }

    const auto& getNextArray() const noexcept { return next; }
    auto& getNextArray() noexcept { return next; }

    T getNextSize() const noexcept { return next_size; }

    template<typename InIter>
    T findFirstOne(const InIter TBegin, const InIter TEnd) noexcept {
        if (length > TEnd - TBegin) {
            return -1;
        }
        auto TIter = TBegin;
        auto iter = begin;

        while (TIter != TEnd) {

            if (*TIter != *iter) {

                auto nextIndex = next[iter - begin];

                if (-1 == nextIndex) {
                    iter = begin;

                    ++TIter;

                } else {
                    iter = begin + nextIndex;

                }

            } else {

                ++iter;
                ++TIter;

                if (iter == end) {
                    return static_cast<T>(TIter - TBegin - length);
                }
            }
        }

        return -1;
    }

    template<typename InIter, typename OutIter>
    OutIter findAll(const InIter TBegin, const InIter TEnd, OutIter out) noexcept {
        if (length > TEnd - TBegin) {
            return out;
        }
        auto TIter = TBegin;
        auto iter = begin;

        while (TIter != TEnd) {

            if (*TIter != *iter) {

                auto nextIndex = next[iter - begin];

                if (-1 == nextIndex) {
                    iter = begin;

                    ++TIter;

                } else {
                    iter = begin + nextIndex;

                }

            } else {

                ++iter;
                ++TIter;

                if (iter == end) {
                    *out++ = static_cast<T>(TIter - TBegin - length);

                    iter = begin + next[next_size - 1];

                }
            }
        }

        return out;
    }

};

}


namespace old {
template<typename Iterator, typename U = int>
class Kmp {
    Iterator begin;
    Iterator end;

    typedef typename std::iterator_traits<Iterator>::iterator_category IteratorCategory;
    using WNSize = U;

    WNSize length;
    std::vector<WNSize> next;

    void _generateNextArray(std::random_access_iterator_tag) {
        // generate `next` array base on 'begin' and 'end' ivars.

        next.assign({-1, 0});
        auto iter1 = begin;
        auto iter2 = iter1 + 1;
//            auto endIter = end - 1;
        while (iter2 != end) {
            if (*iter2 == *iter1) {
                ++iter1;
                ++iter2;

                // like abcxxabc, if the second 'b' doesn't match, we don't need to compare prefix 'ab' again.
                if (iter2 != end && *iter2 == *iter1) {
                    next.push_back(next[iter1 - begin]);
                } else {
                    next.push_back(static_cast<WNSize>(iter1 - begin));
                }


            } else {
                auto nextIndex = next[iter1 - begin];

                if (nextIndex == -1) {
                    iter1 = begin;
                    ++iter2;
                    next.push_back(static_cast<WNSize>(0));

                } else {

                    iter1 = begin + nextIndex;
                }
            }
        }
    }

public:


    Kmp(Iterator begin, Iterator end) noexcept
            : begin(std::move(begin)), end(std::move(end)), length(static_cast<WNSize>(end - begin)) {
        _generateNextArray(IteratorCategory());
    }

    const std::vector<WNSize> &getNextArray() const noexcept {
        return next;
    }

    template<typename T>
    std::optional<WNSize> findFirstOne(const T TBegin, const T TEnd) noexcept {
        if (length > TEnd - TBegin) {
            return {};
        }
        auto TIter = TBegin;
        auto iter = begin;

        while (TIter != TEnd) {

            if (*TIter != *iter) {

                auto nextIndex = next[iter - begin];

                if (-1 == nextIndex) {
                    iter = begin;

                    ++TIter;

                } else {
                    iter = begin + nextIndex;

                }

            } else {

                ++iter;
                ++TIter;

                if (iter == end) {
                    return static_cast<WNSize>(TIter - TBegin - length);
                }
            }
        }

        return {};
    }

    template<typename T>
    std::optional<std::vector<WNSize> > findAll(const T TBegin, const T TEnd) noexcept {
        if (length > TEnd - TBegin) {
            return {};
        }
        auto TIter = TBegin;
        auto iter = begin;

        std::vector<WNSize> ret;

        while (TIter != TEnd) {

            if (*TIter != *iter) {

                auto nextIndex = next[iter - begin];

                if (-1 == nextIndex) {
                    iter = begin;

                    ++TIter;

                } else {
                    iter = begin + nextIndex;

                }

            } else {

                ++iter;
                ++TIter;

                if (iter == end) {
                    ret.push_back(static_cast<WNSize>(TIter - TBegin - length));

                    iter = begin + next.back();

                }
            }
        }

        if (!ret.empty()) {
            return ret;
        }

        return {};
    }

};

}


//test

//string str;
//cin >> str; // ababaaaba
//
//WN::String::Kmp kmp(str.begin(), str.end());
//
//
//auto &next = kmp.getNextArray();
//
//for (auto &i : next) {
//cout << i << ' ' ;
//}
//cout << endl;
//
//string str2;
//cin >> str2; //aabbaabbaabbababaaaba
//
//auto one = kmp.findFirstOne(str2.begin(), str2.end());
//
//if (one) {
//WNLog(*one);
//} else {
//WNLog("no match");
//}
//
//
//auto all = kmp.findAll(str2.begin(), str2.end());
//
//if (all) {
//Print(*all);
//} else {
//WNLog("no match");
//}


}




#endif //LEARNKIT_KMP_H
