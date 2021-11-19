// -*- Nitetrichor -*-
//===--------------------------- `target name` ---------------------------------===//
//
// temp.hpp
//
// Created by Molybdenum on 11/2/21.
// Copyright (c) 2021 Nitetrichor. All rights reserved.
//===----------------------------------------------------------------------===//
#ifndef NITETRICHOR_TEMP_CPP
#define NITETRICHOR_TEMP_CPP
#include <iostream>
#include <queue>
#include <sstream>

using std::cin;
using std::cout;
using std::endl;


namespace __sch {
class BTree {
    struct TreeNode {
        char data;
        TreeNode *left;
        TreeNode *right;

        TreeNode() : data(), left(nullptr), right(nullptr) {}

        explicit TreeNode(char x) : data(x), left(nullptr), right(nullptr) {}

        TreeNode(char x, TreeNode *left, TreeNode *right) : data(x), left(left), right(right) {}
    };

    TreeNode *root;

    void _deallocTree(TreeNode *node) {
        if (node) {
            _deallocTree(node->left);
            _deallocTree(node->right);
            delete node;
        }
    }

    template<typename Iter>
    void _preorderInit(TreeNode **node, Iter &begin, const Iter &end) {
        if (begin != end && *begin != '0') {
            *node = new TreeNode(*begin);
            _preorderInit(&(*node)->left, ++begin, end);
            _preorderInit(&(*node)->right, ++begin, end);
        }
    }

    void _preorderPrint(TreeNode *node) {
        if (node) {
            cout << node->data;
            _preorderPrint(node->left);
            _preorderPrint(node->right);
        }
    }

    void _inorderPrint(TreeNode *node) {
        if (node) {
            _inorderPrint(node->left);
            cout << node->data;
            _inorderPrint(node->right);
        }
    }

    void _postorderPrint(TreeNode *node) {
        if (node) {
            _postorderPrint(node->left);
            _postorderPrint(node->right);
            cout << node->data;
        }
    }

    static void _leaveCount(TreeNode *node, int &cnt) {
        if (!node) { return; }
        if (!node->left && !node->right) {
            ++cnt;
        }
        _leaveCount(node->left, cnt);
        _leaveCount(node->right, cnt);
    }

    void _printLeafAndItsParent(TreeNode *parent, TreeNode *node, std::vector<char> &parents) {
        if (!node) { return; }
        if (!node->left && !node->right) {
            // leaf
            parents.push_back(parent->data);
            cout << node->data << ' ';
        }
        _printLeafAndItsParent(node, node->left, parents);
        _printLeafAndItsParent(node, node->right, parents);
    }

    void _height(TreeNode *node, int height, int &res) {
        if (!node) {
            res = std::max(height, res);
            return;
        }
        _height(node->left, height + 1, res);
        _height(node->right, height + 1, res);
    }

public:
    BTree() : root() {}

    ~BTree() {
        _deallocTree(root);
    }

    template<typename Iter>
    void initWithInorder(Iter begin, Iter end) {
        _preorderInit(&root, begin, end);
    }

    void preorderPrint() {
        _preorderPrint(root);
    }

    void inorderPrint() {
        _inorderPrint(root);
    }

    void postorderPrint() {
        _postorderPrint(root);
    }

    int getLeaveCount() {
        int ret = 0;
        _leaveCount(root, ret);
        return ret;
    }

    void printLeafAndItsParent() {
        std::vector<char> parents;
        _printLeafAndItsParent(nullptr, root, parents);
        cout << endl;
        for (char i : parents) {
            cout << i << ' ';
        }
        cout << endl;
    }

    int getHeight() {
        int ret = 0;
        _height(root, 0, ret);
        return ret;
    }
};

template<typename Iter>
void PreOrderPrint(Iter begin, Iter cur, Iter end) {
    if (cur >= end || *cur == 0) { return; }
    cout << *cur << ' ';
    PreOrderPrint(begin, begin + (cur - begin) * 2 + 1, end);
    PreOrderPrint(begin, begin + (cur - begin) * 2 + 2, end);
}

}// namespace __sch


namespace leetCode {
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    explicit TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

template<typename Iter>
TreeNode *BuildTree(Iter begin, Iter end) {
    auto *root = new TreeNode();
    std::queue<TreeNode *> queue;
    queue.push(root);
    root->val = *begin++;
    while (begin != end) {
        auto *front = queue.front();
        queue.pop();
        if (*begin != 0) {
            front->left = new TreeNode(*begin);
            queue.push(front->left);
        }
        ++begin;
        if (*begin != 0) {
            front->right = new TreeNode(*begin);
            queue.push(front->right);
        }
        ++begin;
    }
    return root;
}

void DeallocTree(TreeNode *node) {
    if (node) {
        DeallocTree(node->left);
        DeallocTree(node->right);
        delete node;
    }
}


TreeNode *TreeInput() {
    std::string in;
    getline(cin, in);

    std::stringstream ss(in);

    std::vector<int> vec;
    while (std::getline(ss, in, ',')) {
        if (in == "null") {
            vec.push_back(0);
            continue;
        }
        int val = std::stoi(in);
        vec.push_back(val);
    }
    return BuildTree(vec.begin(), vec.end());
}

template<typename Iter, typename T = int>
class Kmp {
    Iter begin;
    Iter end;

    typedef typename std::iterator_traits<Iter>::iterator_category IteratorCategory;

    T length;
    T next_size;
    std::vector<T> next /* = {-1, 0}*/;

    void _generateNextArray(std::random_access_iterator_tag) {

        auto iter1 = begin;
        auto iter2 = iter1 + 1;
        while (iter2 != end) {
            if (*iter2 == *iter1) {
                ++iter1;
                ++iter2;
                // like abcxxabc, if the second 'b' doesn't match, we don't need to compare prefix 'ab' again.
                if (iter2 != end && *iter2 == *iter1) {
                    next[next_size++] = next[iter1 - begin];
                } else {
                    next[next_size++] = static_cast<T>(iter1 - begin);
                }

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

    const auto &getNextArray() const noexcept { return next; }
    auto &getNextArray() noexcept { return next; }

    T getNextSize() const noexcept { return next_size; }

    template<typename InIter>
    T findFirstOne(const InIter TBegin, const InIter TEnd) noexcept {
        if (length > TEnd - TBegin) {
            return -1;
        }
        auto TIter = TBegin;
        auto iter  = begin;

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
        auto iter  = begin;

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

class Solution {
public:
    int dfs(TreeNode *root, int &pre) {
        if (!root) { return INF; }
        int left  = dfs(root->left, pre);
        int ret   = std::min(std::abs(root->val - pre), left);
        pre       = root->val;
        int right = dfs(root->right, pre);
        return std::min(ret, right);
    }

    constexpr static int INF = 0x3f3f'3f3f;

    int minDiffInBST(TreeNode *root) {
        int inf = INF;
        return dfs(root, inf);
    }

    void dfs(TreeNode *root, std::vector<int> &vec) {
        if (!root) { return; }
        dfs(root->left, vec);
        dfs(root->right, vec);
        if (!root->left && !root->right) {
            vec.push_back(root->val);
        }
    }

    bool leafSimilar(TreeNode *root1, TreeNode *root2) {
        std::vector<int> vec1, vec2;
        dfs(root1, vec1);
        dfs(root2, vec2);
        return vec1 == vec2;
    }


    void dp(TreeNode *root, int &max, int &res) {
        if (!root) { return; }

        int cur_max = max;

        dp(root->left, cur_max, res);

        int left_max = cur_max;

        dp(root->right, max, res);

        res = std::max(res, left_max + max + 2);
        max = std::max(left_max + 1, max + 1);
    }

    int diameterOfBinaryTree(TreeNode *root) {
        int max = 0, res = 0;
        dp(root, max, res);
        return res - 2;
    }

    void find_tilt_dfs(TreeNode *root, int &sum, int &res) {
        if (!root) { return; }

        int left = 0, right = 0;
        find_tilt_dfs(root->left, left, res);
        find_tilt_dfs(root->right, right, res);

        sum = left + right + root->val;

        res += std::abs(left - right);
    }

    int findTilt(TreeNode *root) {
        int sum = 0, res = 0;
        find_tilt_dfs(root, sum, res);
        return res;
    }

    void to_post_order(TreeNode *root, std::vector<int> &vec) {
        if (!root) {
            vec.push_back(INT_MAX);
            return;
        }
        to_post_order(root->left, vec);
        to_post_order(root->right, vec);
        vec.push_back(root->val);
    }

    void is_sub_tree(TreeNode *root, std::vector<int> &post, const std::vector<int> &sub, bool &res) {
        if (!root || res) {
            if (!root) {
                post.push_back(INT_MAX);
            }
            return;
        }

        std::vector<int> left, right;
        is_sub_tree(root->left, left, sub, res);
        is_sub_tree(root->right, right, sub, res);

        post = std::move(left);
        post.insert(post.end(), right.begin(), right.end());
        post.push_back(root->val);

        if (post == sub) {
            res = true;
        }
    }

    bool isSubtree(TreeNode *root, TreeNode *subRoot) {
        std::vector<int> sub, post;
        to_post_order(root, post);
        to_post_order(subRoot, sub);
        Kmp<decltype(sub.begin()), int> kmp(sub.begin(), sub.end());

        return kmp.findFirstOne(post.begin(), post.end()) >= 0;
    }
};


}// namespace leetCode


//int main() {
//    using namespace leetCode;
//
//    auto *tree1 = TreeInput();
//    auto *tree2 = TreeInput();
//    Solution solution;
//    cout << solution.isSubtree(tree1, tree2) << endl;
//
//    DeallocTree(tree1);
//
//
//    return 0;
//}

#endif//NITETRICHOR_TEMP_CPP
