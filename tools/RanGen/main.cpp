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

    cout << vtx_num << endl;

    for (int i = 0; i < vtx_num; ++i) {
        for (int j = 0; j < vtx_num; ++j) {
            if (j == 0) {
                cout << random.getBool();
            } else {
                cout << ' ' << random.getBool();
            }
        }
        cout << endl;
    }
}


int main(int argc, const char *argv[]) {

    NC::Random random;

    int round = random.get(5, 20);
    cout << round << endl;
    for (int i = 0; i < round; ++i) {
        test(random);
    }

}