// -*- Nitetrichor -*-
//===--------------------------- Learn ++ ---------------------------------===//
//
// SuffixArray.hpp
// 
// Created by Molybdenum on 11/2/21.
// Copyright (c) 2021 Nitetrichor. All rights reserved.
//===----------------------------------------------------------------------===//
#ifndef NITETRICHOR_SUFFIXARRAY_HPP
#define NITETRICHOR_SUFFIXARRAY_HPP
namespace NC {
template<int Size, typename T = int>
class SuffixArray {
    T sa[Size];
    T rank[Size];
    T temp_arr[Size];


public:
    template<typename InIter>
    SuffixArray(InIter begin, InIter end) {

        int n = 0;
        while (begin != end) {
            sa[n] = n;
            rank[n] = (T)*begin;
            ++n;
            ++begin;
        }


        for (int i = 1; i <= n; i <<= 1) {

            auto sa_comp = [this, i, n](T lhs, T rhs) -> bool {
                if (rank[lhs] != rank[rhs]) { return rank[lhs] < rank[rhs]; }
                int r_lhs = lhs + i <= n ? rank[lhs + i] : -1;
                int r_rhs = rhs + i <= n ? rank[rhs + i] : -1;
                return r_lhs < r_rhs;
            };

            std::sort(sa, sa + n, sa_comp);

            // evaluate rank in temp, and store back to rank[]
            temp_arr[sa[0]] = 0;
            for (int j = 1; j < n; ++j) {
                temp_arr[sa[j]] = temp_arr[sa[j - 1]] + (sa_comp(sa[j - 1], sa[j]) ? 1 : 0);
            }

            memcpy(rank, temp_arr, n * sizeof(T));
        }


    }

    const auto& getArray() const { return sa; }
};


template<int Size, int ElemSize, typename T = int>
class SuffixArray1 {
    int n;
    T sa[Size];
    T height[Size];
    template<typename InIter>
    inline void _build_sa(InIter begin, InIter end) {
        n = end - begin;
        int bucket_size = std::max(n, ElemSize);
        std::vector<int> bucket(bucket_size);
        std::vector<int> rank(n);
        std::vector<int> sec_sa(n);

        {
            // radix sort the one-len suffix
            int i = 0;
            for (auto iter = begin; iter != end; ++iter, ++i) {
                rank[i] = (T) *iter;
                ++bucket[rank[i]];
            }

            // if C++ >= 17, prefer std::inclusive_scan
            std::partial_sum(bucket.begin(), bucket.begin() + bucket_size, bucket.begin());

            for (int j = n - 1; j >= 0; --j) {
                sa[--bucket[rank[j]]] = j;
            }
        }

        for (int i = 1; i <= n; i <<= 1) {
            // evaluate the second part of current suffix
            int cnt = 0;
            for (int j = n - i; j < n; ++j) {
                sec_sa[cnt++] = j;
            }
            for (int j = 0; j < n; ++j) {
                if (sa[j] >= i) {
                    sec_sa[cnt++] = sa[j] - i;
                }
            }

            // radix sort
            std::fill(bucket.begin(), bucket.begin() + bucket_size, 0);
            for (int j = 0; j < n; ++j) {
                ++bucket[rank[j]];
            }

            std::partial_sum(bucket.begin(), bucket.begin() + bucket_size, bucket.begin());

            for (int j = n - 1; j >= 0; j--) {
                // adjust sa according to the second part
                sa[--bucket[rank[sec_sa[j]]]] = sec_sa[j];
            }

            // evaluate next rank
            std::swap(rank, sec_sa);

            cnt = 1;
            rank[sa[0]] = 0;
            for (int j = 1; j < n; ++j) {
                int r = sa[j] + i >= n ? -1 : sec_sa[sa[j] + i];
                int r_1 = sa[j - 1] + i >= n ? -1 : sec_sa[sa[j - 1] + i];
                rank[sa[j]] = sec_sa[sa[j - 1]] == sec_sa[sa[j]] && r_1 == r ? cnt - 1 : cnt++;
            }

            if (cnt > n) { break; } // now numbers in sa are different from each other

            bucket_size = cnt;

        }

    }

    template<typename InIter>
    inline void _build_height(InIter begin) {
        // height[rank[i]] >= height[rank[i - 1]] - 1
        std::vector<T> rank(n);
        for (int i = 0; i < n; ++i) {
            rank[sa[i]] = i;
        }
        height[0] = 0;
        int lcp = 0;
        for (int i = 0; i < n; ++i) {
            if (rank[i] == 0) { continue; }
            if (lcp) { --lcp; }
            int j = sa[rank[i] - 1];
            while (i + lcp < n && j + lcp < n && *(begin + i + lcp) == *(begin + j + lcp)) {
                ++lcp;
            }
            height[rank[i]] = lcp;
        }
    }
public:
    SuffixArray1(): n(), sa(), height() {}

    template<typename InIter>
    SuffixArray1(InIter begin, InIter end) : n() {
        init(begin, end);
    }

    template<typename InIter>
    void init(const InIter& begin, const InIter& end) {
        _build_sa(begin, end);
        _build_height(begin);
    }

    const auto& getArray() const { return sa; }

    const auto& getHeight() const { return height; }

    bool checkHasRepeatedSubstringNonOverlap(int k) {
        for (int i = 1;i <= n; i++) {
            if (height[i] >= k) {
                for(int j = i - 1; j >= 0; j--) {
                    if (abs(sa[i] - sa[j]) >= k) { return true; }
                    if (height[j] < k) { break; }
                }
            }
        }
        return false;
    }
};

}


#endif //NITETRICHOR_SUFFIXARRAY_HPP
