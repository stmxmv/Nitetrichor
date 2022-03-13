// -*- Nitetrichor -*-
//===--------------------------- `target name` ---------------------------------===//
//
// Trie.hpp
// include/Nitetrichor/LearnKit
// Created by Molybdenum on 12/14/21.
// Copyright (c) 2021 Nitetrichor. All rights reserved.
//===----------------------------------------------------------------------===//
#ifndef NITETRICHOR_TRIE_HPP
#define NITETRICHOR_TRIE_HPP
#include <utility>
#include <queue>
namespace AN {


template<typename T = int>
class Trie {
public:
    constexpr static int childrenNum = 26;
    struct Node {
        T cnt;
        T children[childrenNum];
    };
    static_assert(std::is_integral<T>::value, "Integral Required!!");
    static_assert(!std::is_same<T, char>::value, "char type maybe not fit");
private:


    T posCnt;
    std::vector<T> pos;
    T trieCnt;
    std::vector<Node> trie;

public:
    Trie() noexcept: posCnt(), pos(), trieCnt(), trie(1) {}

    void clear() {
        trie.clear();
        trieCnt = 0;
        pos.clear();
        posCnt = 0;
    }

    void insert(const char *str) {
        T cur = 0;
        for (; *str; ++str) {
            int idx = *str - 'a';
            if (!trie[cur].children[idx]) {
                trie[cur].children[idx] = ++trieCnt;
                trie.emplace_back();
            }


            cur = trie[cur].children[idx];
            ++trie[cur].cnt;
        }

        pos.push_back(cur);
        ++posCnt;
    }

//    void levelPrint() {
//        std::queue<std::pair<char, int>> queue;
//        for (int i = 0; i < childrenNum; ++i) {
//            if (trie[0].children[i]) {
//                queue.emplace('a' + i, trie[0].children[i]);
//            }
//        }
//
//        while (!queue.empty()) {
//            auto front = queue.front();
//            queue.pop();
//
//            cout << front.first;
//
//            auto cur = front.second;
//            for (int i = 0; i < childrenNum; ++i) {
//                if (trie[cur].children[i]) {
//                    queue.emplace('a' + i, trie[cur].children[i]);
//                }
//            }
//
//        }
//        cout << endl;
//    }

    T queryPrefix(const char *str) {
        T cur = 0;
        for (; *str; ++str) {
            cur = trie[cur].children[*str - 'a'];
        }
        return trie[cur].cnt;
    }


};







}
#endif//NITETRICHOR_TRIE_HPP
