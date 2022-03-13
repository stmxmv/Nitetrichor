//
// Created by Molybdenum on 8/8/21.
// Copyright (c) 2021 Nitetrichor. All rights reserved.
//

#ifndef LEARNKIT_BTREE_H
#define LEARNKIT_BTREE_H

#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
namespace NC {



namespace dyn {


namespace list_version {


template<typename T>
class BST {

    struct Leaf {
        T data;
        std::unique_ptr<Leaf> lChild, rChild;
        int cnt, size;
    };


    void __insert(std::unique_ptr<Leaf> &cur, const T &data) {

        if (!cur) {
            cur = std::make_unique<Leaf>();
            cur->data = data;
            cur->size = cur->cnt = 1;
            return;
        }

        ++cur->size;

        if (cur->data == data) {
            ++cur->cnt;
            return;
        }

        if (cur->data > data) {
            __insert(cur->lChild, data);
        } else {
            __insert(cur->rChild, data);
        }


    }


    template<typename Callback>
    void __inorder(std::unique_ptr<Leaf> &cur, const Callback &callback) {
        if (!cur) { return; }

        __inorder(cur->lChild, callback);

        callback(cur->data);

        __inorder(cur->rChild, callback);

    }

    bool __exist(std::unique_ptr<Leaf> &cur, const T &val) {

        if (!cur) { return false; }

        if (cur->data == val) { return true; }

        if (cur->data > val) {
            return __exist(cur->lChild, val);
        }

        return __exist(cur->rChild, val);
    }

    void __do_remove(std::unique_ptr<Leaf> &node) {


        /// just leave cnt and size away :)
        if (!node->lChild) {

            node = std::move(node->rChild);

        } else if (!node->rChild) {

            node = std::move(node->lChild);

        } else {

            Leaf *to_swap_parent = node.get();
            Leaf *to_swap = node->lChild.get();

            while (to_swap->rChild) {
                /// go to right most
                to_swap_parent = to_swap;
                to_swap = to_swap->rChild.get();
            }

            node->data = to_swap->data;

            if (to_swap_parent != node.get()) {
                to_swap_parent->rChild = std::move(to_swap->lChild);
            } else {
                to_swap_parent->lChild = std::move(to_swap->lChild);
            }

        }



    }

    bool __remove(std::unique_ptr<Leaf> &cur, const T &val) {
        if (!cur) { return false; }

        if (cur->data == val) {
            __do_remove(cur);
            return true;
        }

        if (cur->data > val) {
            return __remove(cur->lChild, val);
        }

        return __remove(cur->rChild, val);

    }
public:

    std::unique_ptr<Leaf> root;
    int cnt;

    BST() : root(), cnt() {}


    void insert(const T &data) {
        __insert(root, data);
    }

    template<typename Callback>
    void inorder(const Callback &callback) {
        __inorder(root, callback);
    }

    bool exist(const T &val) {
        return __exist(root, val);
    }

    bool remove(const T &val) {
        return __remove(root, val);
    }

};




}

template<typename T>
class BST {
    struct leaf {
        T data;
        int lChild, rChild;
        int cnt, size;
    };

    int cnt;
    std::vector<leaf> tree;


    void _insert(int cur, const T &data) {
        ++tree[cur].size;
        if (tree[cur].data == data) {
            ++tree[cur].cnt;
            return;
        }
        if (tree[cur].data > data) {

            if (tree[cur].lChild) {
                _insert(tree[cur].lChild, data);
            } else {
                tree.emplace_back();
                tree[++cnt].data = data;
                tree[cnt].size = tree[cnt].cnt = 1;
                tree[cur].lChild               = cnt;
            }

        } else {

            if (tree[cur].rChild) {
                _insert(tree[cur].rChild, data);
            } else {
                tree.emplace_back();
                tree[++cnt].data = data;
                tree[cnt].size = tree[cnt].cnt = 1;
                tree[cur].rChild               = cnt;
            }
        }
    }

    T _queryFront(int cur, const T &data, const T &found) {
        if (tree[cur].data >= data) {
            if (tree[cur].lChild) {
                return _queryFront(tree[cur].lChild, data, found);
            }
            return found;
        }
        if (tree[cur].rChild) {

            if (tree[cur].cnt) {

                return _queryFront(tree[cur].rChild, data, tree[cur].data);
            }
            return _queryFront(tree[cur].rChild, data, found);
        }
        return tree[cur].data;
    }

    T _queryBack(int cur, const T &data, const T &found) {
        if (tree[cur].data <= data) {
            if (tree[cur].rChild) {
                return _queryBack(tree[cur].rChild, data, found);
            }
            return found;
        }
        if (tree[cur].lChild) {

            if (tree[cur].cnt) {

                return _queryBack(tree[cur].lChild, data, tree[cur].data);
            }
            return _queryBack(tree[cur].lChild, data, found);
        }
        return tree[cur].data;
    }

    int _index(int cur, const T &data) {
        if (cur == 0) {
            return 0;
        }
        if (tree[cur].data == data) {
            return tree[tree[cur].lChild].size;
        }

        if (data < tree[cur].data) {
            return _index(tree[cur].lChild, data);
        }

        return _index(tree[cur].rChild, data) + tree[tree[cur].lChild].size + tree[cur].cnt;
    }

    T _atIndex(int cur, int idx) {
        if (cur == 0) {
            return invalid;
        }
        if (tree[tree[cur].lChild].size > idx) {
            return _atIndex(tree[cur].lChild, idx);
        }

        if (tree[tree[cur].lChild].size + tree[cur].cnt > idx) {
            return tree[cur].data;
        }
        return _atIndex(tree[cur].rChild, idx - (tree[tree[cur].lChild].size + tree[cur].cnt));
    }

public:
    T invalid;

    BST() : cnt(), tree(2), invalid() {}

    void insert(const T &data) {
        if (!cnt) {
            [[unlikely]]
            ++cnt;
            tree[1].data = data;
            tree[1].cnt = tree[1].size = 1;
        } else {
            _insert(1, data);
        }
    }

    T front(const T &data) {
        return _queryFront(1, data, invalid);
    }

    T back(const T &data) {
        return _queryBack(1, data, invalid);
    }

    int index(const T &data) {
        return _index(1, data);
    }

    int atIndex(int idx) {
        return _atIndex(1, idx);
    }
};


}


template<int Size, typename T>
class BST {
    struct leaf {
        T data;
        int lChild, rChild;
        int cnt, size;
    };

    int cnt;
    leaf tree[Size];


    void _insert(int cur, const T &data) {
        ++tree[cur].size;
        if (tree[cur].data == data) {
            ++tree[cur].cnt;
            return;
        }
        if (tree[cur].data > data) {

            if (tree[cur].lChild) {
                _insert(tree[cur].lChild, data);
            } else {
                tree[++cnt].data = data;
                tree[cnt].size = tree[cnt].cnt = 1;
                tree[cur].lChild               = cnt;
            }

        } else {

            if (tree[cur].rChild) {
                _insert(tree[cur].rChild, data);
            } else {
                tree[++cnt].data = data;
                tree[cnt].size = tree[cnt].cnt = 1;
                tree[cur].rChild               = cnt;
            }
        }
    }

    T _queryFront(int cur, const T &data, const T &found) {
        if (tree[cur].data >= data) {
            if (tree[cur].lChild) {
                return _queryFront(tree[cur].lChild, data, found);
            }
            return found;
        }
        if (tree[cur].rChild) {

            if (tree[cur].cnt) {

                return _queryFront(tree[cur].rChild, data, tree[cur].data);
            }
            return _queryFront(tree[cur].rChild, data, found);
        }
        return tree[cur].data;
    }

    T _queryBack(int cur, const T &data, const T &found) {
        if (tree[cur].data <= data) {
            if (tree[cur].rChild) {
                return _queryBack(tree[cur].rChild, data, found);
            }
            return found;
        }
        if (tree[cur].lChild) {

            if (tree[cur].cnt) {

                return _queryBack(tree[cur].lChild, data, tree[cur].data);
            }
            return _queryBack(tree[cur].lChild, data, found);
        }
        return tree[cur].data;
    }

    int _index(int cur, const T &data) {
        if (cur == 0) {
            return 0;
        }
        if (tree[cur].data == data) {
            return tree[tree[cur].lChild].size;
        }

        if (data < tree[cur].data) {
            return _index(tree[cur].lChild, data);
        }

        return _index(tree[cur].rChild, data) + tree[tree[cur].lChild].size + tree[cur].cnt;
    }

    T _atIndex(int cur, int idx) {
        if (cur == 0) {
            return invalid;
        }
        if (tree[tree[cur].lChild].size > idx) {
            return _atIndex(tree[cur].lChild, idx);
        }

        if (tree[tree[cur].lChild].size + tree[cur].cnt > idx) {
            return tree[cur].data;
        }
        return _atIndex(tree[cur].rChild, idx - (tree[tree[cur].lChild].size + tree[cur].cnt));
    }

public:
    T invalid;

    BST() : cnt(), tree(), invalid() {}

    void insert(const T &data) {
        if (!cnt) {
            [[unlikely]]
            ++cnt;
            tree[1].data = data;
            tree[1].cnt = tree[1].size = 1;
        } else {
            _insert(1, data);
        }
    }

    T front(const T &data) {
        return _queryFront(1, data, invalid);
    }

    T back(const T &data) {
        return _queryBack(1, data, invalid);
    }

    int index(const T &data) {
        return _index(1, data);
    }

    int atIndex(int idx) {
        return _atIndex(1, idx);
    }
};


template<typename Iter, typename Iter1>
void PreOrderWeight(const Iter &begin, Iter cur, const Iter &end, Iter1 &weight, int height, int &res) {
    if (cur >= end || *cur == 0) { return; }
    auto child0 = begin + (cur - begin) * 2 + 1;
    auto child1 = child0 + 1;
    if ((child0 >= end || *child0 == '0') && (child1 >= end || *child1 == '0')) {
        // is leaf
        res += *weight++ * height;
    }

    PreOrderWeight(begin, begin + (cur - begin) * 2 + 1, end, weight, height + 1, res);
    PreOrderWeight(begin, begin + (cur - begin) * 2 + 2, end, weight, height + 1, res);
}


template<typename T>
class basic_btree {
    struct TreeNode {
        T val;
        TreeNode *left, *right;
        TreeNode() : val(), left(), right() {}
        explicit TreeNode(T x) : val(std::move(x)), left(), right() {}
    };

    TreeNode *__root;

    template<typename Iter>
    void __build_tree(TreeNode **root, Iter &begin, const Iter &end) {
        if (begin >= end || *begin == '0') {
            begin++;
            return;
        }
        *root = new TreeNode(*begin++);
        __build_tree(&(*root)->left, begin, end);
        __build_tree(&(*root)->right, begin, end);
    }

    template<typename Iter>
    TreeNode *TreeAllocate(Iter &begin, const Iter &end) {
        TreeNode *ret;
        __build_tree(&ret, begin, end);
        return ret;
    }

    template<typename Iter>
    void __build_tree(TreeNode **root, Iter &begin, const Iter &end, int branch) {
        if (begin >= end || *begin == '0') {
            ++begin;
            return;
        }

        *root = new TreeNode(*begin++);
        __build_tree(&(*root)->left, begin, end, branch);

        auto *cur = (*root)->left;

        for (int i = 1; i < branch; ++i) {
            if (!cur) {
                ++begin;
                continue;
            }

            __build_tree(&cur->right, begin, end, branch);
            cur = cur->right;
        }
    }


    template<typename Iter>
    TreeNode *TreeAllocate(Iter &begin, const Iter &end, int branch) {
        TreeNode *ret = nullptr;
        __build_tree(&ret, begin, end, branch);
        return ret;
    }


    void TreeDeallocate(TreeNode *node) {
        if (!node) { return; }
        TreeDeallocate(node->left);
        TreeDeallocate(node->right);
        delete node;
    }


    void BTreePrintCode(TreeNode *root, std::string &buff) {
        if (!root) { return; }
        using std::cout, std::endl;
        if (!root->left && !root->right) {
            for (int i = 0; i < (int) buff.length(); ++i) {
                if (i == 0) {
                    cout << buff[0];
                } else {
                    cout << ' ' << buff[i];
                }
            }
            cout << endl;
        }

        buff.push_back('0');
        BTreePrintCode(root->left, buff);
        buff.pop_back();

        buff.push_back('1');
        BTreePrintCode(root->right, buff);
        buff.pop_back();
    }

public:
    template<typename Iter>
    basic_btree(Iter begin, Iter end) {
        __root = TreeAllocate(begin, end);
    }

    template<typename Iter>
    basic_btree(Iter begin, Iter end, int branch) {
        __root = TreeAllocate(begin, end, branch);
    }

    ~basic_btree() {
        TreeDeallocate(__root);
    }

    void printCode() {
        std::string buff;
        BTreePrintCode(__root->left, buff);
    }
};


template<typename T>
class Huffman {
    struct Node {
        T value;
        int children[2];
        explicit Node(T value) : value(value), children{-1, -1} {}
        Node(T value, int c1, int c2) : value(value), children{c1, c2} {}
        bool operator<(const Node &other) const {
            return value < other.value;
        }
    };
    std::unordered_map<int, std::string> map;

    std::vector<Node> tree;
    int cnt;

    void __generate_map(int cur, std::string &buff) {
        if (cur < 0) { return; }

        if (tree[cur].children[0] < 0 && tree[cur].children[1] < 0) {
            // leaf
            map[cur] = buff;
            return;
        }

        buff.push_back('0');
        __generate_map(tree[cur].children[0], buff);
        buff.pop_back();
        buff.push_back('1');
        __generate_map(tree[cur].children[1], buff);
        buff.pop_back();
    }

public:
    Huffman() : tree(), cnt() {}

    /// \brief
    /// \tparam Iter pointer to array of weights
    /// \param begin
    /// \param end
    template<typename Iter>
    void init(Iter begin, Iter end) {
        struct queue_elem {
            int no;
            T value;
            queue_elem(int no, T value) : no(no), value(value) {}
            bool operator<(const queue_elem &other) const {
                if (value == other.value) {
                    return no > other.no;
                }
                return value > other.value;
            }
        };
        std::priority_queue<queue_elem> queue;
        while (begin != end) {
            queue.emplace(cnt++, *begin);
            tree.emplace_back(*begin++);
        }


        while (!queue.empty()) {
            auto first = queue.top();
            queue.pop();
            if (queue.empty()) { break; }
            auto second = queue.top();
            queue.pop();

            if (first.value > second.value) {
                std::swap(first, second);
            }
            // first < second
            tree.emplace_back(first.value + second.value, first.no, second.no);
            queue.emplace(cnt++, first.value + second.value);
        }


        std::string buff;
        __generate_map(cnt - 1, buff);
    }


    //    const std::string& getCode(T value) {
    //        return map.at(value);
    //    }

    [[nodiscard]] const std::string &getCode(int idx) const { return map.at(idx); }

    /// \brief
    /// \tparam OutIter put in the index of weights
    /// \param code
    /// \param out
    /// \return
    template<typename OutIter>
    bool decode(const std::string &code, OutIter out) const {
        int cur = cnt - 1;
        for (auto i : code) {
            if (i != '0' && i != '1') {
                goto error;
            }
            cur = tree[cur].children[i - '0'];
            if (tree[cur].children[0] < 0 && tree[cur].children[1] < 0) {
                *out++ = cur;
                cur    = cnt - 1;
            }
        }
        if (cur != cnt - 1) {
            goto error;
        }
        return true;
    error:
        return false;
    }

    /// \brief
    /// \tparam Iter
    /// \tparam OutIter put the refers' according value in
    /// \param code
    /// \param refer each weight's according value
    /// \param out
    /// \return
    template<typename Iter, typename OutIter>
    bool decode(const std::string &code, Iter refer, OutIter out) const {
        int cur = cnt - 1;
        for (auto i : code) {
            if (i != '0' && i != '1') {
                goto error;
            }
            cur = tree[cur].children[i - '0'];
            if (tree[cur].children[0] < 0 && tree[cur].children[1] < 0) {
                *out++ = *(refer + cur);
                cur    = cnt - 1;
            }
        }
        if (cur != cnt - 1) {
            goto error;
        }
        return true;
    error:
        return false;
    }
};


namespace __ridiculous {

struct TreeNode {
    char val;
    std::vector<TreeNode *> children;// Oops!!!
    TreeNode() : val() {}
    explicit TreeNode(char x) : val(x) {}
};


template<typename Iter>
void __build_tree(TreeNode **root, Iter &begin, const Iter &end, int branch) {
    if (begin >= end || *begin == '0') {
        begin++;
        return;
    }
    *root = new TreeNode(*begin++);
    (*root)->children.resize(branch);
    for (int i = 0; i < branch; ++i) {
        __build_tree(&(*root)->children[i], begin, end, branch);
    }
}

template<typename Iter>
TreeNode *TreeAllocate(Iter begin, Iter end, int branch) {
    TreeNode *ret;
    __build_tree(&ret, begin, end, branch);
    return ret;
}

void TreeDeallocate(TreeNode *node) {
    if (node) {
        for (auto &i : node->children) {
            TreeDeallocate(i);
        }
        delete node;
    }
}

TreeNode *TreeMerge(TreeNode *root) {
    if (!root) { return nullptr; }
    auto *ret = new TreeNode;
    ret->children.resize(2);
    ret->val = root->val;

    if (!root->children[0]) { return ret; }
    ret->children[0] = TreeMerge(root->children[0]);

    auto *child = ret->children[0];
    for (int i = 1; i < (int) root->children.size(); ++i) {

        if (!root->children[i]) { return ret; }

        child->children[1] = TreeMerge(root->children[i]);

        child = child->children[1];
    }
    return ret;
}

template<typename Iter>
TreeNode *TreeMergeMany(Iter begin, Iter end) {
    auto *ret = new TreeNode;
    ret->children.resize(2);
    ret->children[0] = TreeMerge(*begin++);

    auto *cur = ret->children[0];

    while (begin != end) {

        cur->children[1] = TreeMerge(*begin);

        cur = cur->children[1];

        ++begin;
    }
    return ret;
}


void PrintCode(TreeNode *root, std::string &buff) {
    if (!root) { return; }
    using std::cout, std::endl;
    if (!root->children[0] && !root->children[1]) {
        for (int i = 1; i < (int) buff.length(); ++i) {
            if (i == 1) {
                cout << buff[1];
            } else {
                cout << ' ' << buff[i];
            }
        }
        cout << endl;
    }

    buff.push_back('0');
    PrintCode(root->children[0], buff);
    buff.pop_back();

    buff.push_back('1');
    PrintCode(root->children[1], buff);
    buff.pop_back();
}


}// namespace __ridiculous


namespace __old {
template<typename T>
struct BitNode {
    T data;
    BitNode *lChild, *rChild;

public:
    explicit BitNode(const T &data) noexcept : data(data), lChild(nullptr), rChild(nullptr) {}
};

template<typename T>
struct BitThrNode : BitNode<T> {
    enum class Tag : char {
        Link   = 0x0,
        Thread = 0x1
    };
    Tag lTag : 1, rTag : 1;

public:
    using BitNode<T>::BitNode;
};

template<typename T>
class BTree {
    BitNode<T> *root;
    bool bIsThreaded;
    template<typename InIter>
    inline InIter _createBTreeUsingPreorder(BitNode<T> **tree,
                                            InIter begin,
                                            InIter end,
                                            T terminal,
                                            bool threaded) noexcept {
        if (begin == end || *begin == terminal) {
            *tree = nullptr;
            return begin;
        }
        if (threaded) {
            *tree       = new BitThrNode<T>(*begin);
            auto tTree  = static_cast<BitThrNode<T> *>(*tree);
            tTree->lTag = BitThrNode<T>::Tag::Link;
            tTree->rTag = BitThrNode<T>::Tag::Link;
        } else {
            *tree = new BitNode<T>(*begin);
        }
        auto next = _createBTreeUsingPreorder(&(*tree)->lChild, ++begin, end, terminal, threaded);
        if (next == end) {
            return end;
        }
        return _createBTreeUsingPreorder(&(*tree)->rChild, ++next, end, terminal, threaded);
    }


    inline void dealloc(BitNode<T> *node) {
        if (bIsThreaded) {
            auto tNode = static_cast<BitThrNode<T> *>(node);
            if (tNode->lChild && tNode->lTag == BitThrNode<T>::Tag::Link) {
                dealloc(node->lChild);
            }
            if (tNode->rChild && tNode->rTag == BitThrNode<T>::Tag::Link) {
                dealloc(node->rChild);
            }
        } else {
            if (node->lChild) {
                dealloc(node->lChild);
            }
            if (node->rChild) {
                dealloc(node->rChild);
            }
        }
        delete node;
    }


    template<typename Handler>
    inline void _preorderRead(BitNode<T> *node,
                              Handler handler,
                              int level = 0) noexcept {
        if (!node) { return; }
        handler(node->data, level);
        if (!bIsThreaded) {
            _preorderRead(node->lChild, handler, level + 1);
            _preorderRead(node->rChild, handler, level + 1);
        } else {

            auto tNode = static_cast<BitThrNode<T> *>(node);
            if (tNode->lTag == BitThrNode<T>::Tag::Link) {
                _preorderRead(node->lChild, handler, level + 1);
            }
            if (tNode->rTag == BitThrNode<T>::Tag::Link) {
                _preorderRead(node->rChild, handler, level + 1);
            }
        }
    }
    template<typename Handler>
    inline void _inorderRead(BitNode<T> *node,
                             Handler handler,
                             int level = 0) noexcept {
        if (!node) { return; }
        if (!bIsThreaded) {
            _preorderRead(node->lChild, handler, level + 1);
            handler(node->data, level);
            _preorderRead(node->rChild, handler, level + 1);
        } else {

            auto tNode = static_cast<BitThrNode<T> *>(node);
            if (tNode->lTag == BitThrNode<T>::Tag::Link) {
                _preorderRead(node->lChild, handler, level + 1);
            }
            handler(node->data, level);
            if (tNode->rTag == BitThrNode<T>::Tag::Link) {
                _preorderRead(node->rChild, handler, level + 1);
            }
        }
    }

    template<typename Handler>
    inline void _inorderReadNode(BitNode<T> *node, Handler handler) noexcept {
        if (node) {
            _inorderReadNode(node->lChild, handler);
            handler(node);
            _inorderReadNode(node->rChild, handler);
        }
    }

    template<typename Handler>
    inline void _postorderRead(BitNode<T> *node,
                               Handler handler,
                               int level = 0) noexcept {
        if (!node) { return; }
        if (!bIsThreaded) {
            _preorderRead(node->lChild, handler, level + 1);
            _preorderRead(node->rChild, handler, level + 1);
        } else {

            auto tNode = static_cast<BitThrNode<T> *>(node);
            if (tNode->lTag == BitThrNode<T>::Tag::Link) {
                _preorderRead(node->lChild, handler, level + 1);
            }
            if (tNode->rTag == BitThrNode<T>::Tag::Link) {
                _preorderRead(node->rChild, handler, level + 1);
            }
        }
        handler(node->data, level);
    }

public:
    enum class Order : char {
        Preorder,
        Inorder,
        Postorder
    };

    BTree() = default;

    /*!
     * @brief constructor, invoke method init(...)
     */
    template<typename InIter>
    BTree(InIter begin, InIter end, T terminal, bool threaded = false) noexcept : bIsThreaded(threaded) {
        init(begin, end, terminal, threaded);
    }


    ~BTree() noexcept {
        dealloc(root);
    }

    template<typename InIter>
    void init(InIter begin, InIter end, T terminal, bool threaded = false) noexcept {
        _createBTreeUsingPreorder(&root, begin, end, terminal, threaded);
        if (threaded) {
            BitNode<T> *pre = nullptr;
            _inorderReadNode(root, [&pre](BitNode<T> *node) {
                auto tNode = static_cast<BitThrNode<T> *>(node);
                if (!tNode->lChild) {
                    tNode->lTag   = BitThrNode<T>::Tag::Thread;
                    tNode->lChild = pre;
                }

                if (pre) {
                    auto tPre = static_cast<BitThrNode<T> *>(pre);
                    if (!tPre->rChild) {
                        tPre->rTag   = BitThrNode<T>::Tag::Thread;
                        tPre->rChild = node;
                    }
                }

                pre = node;
            });
        }
    }


    /*!
     * @brief
     * @param handler require signature void aHandler(const T& data, int level);
     */
    template<typename Handler>
    void preorderRead(Handler handler) noexcept {
        _preorderRead(root, handler);
    }

    /*!
     * @brief
     * @param handler require signature void aHandler(const T& data, int level);
     */
    template<typename Handler>
    void inorderRead(Handler handler) noexcept {
        _inorderRead(root, handler);
    }

    /*!
     * @brief
     * @param handler require signature void aHandler(const T& data, int level);
     */
    template<typename Handler>
    void postorderRead(Handler handler) noexcept {
        _postorderRead(root, handler);
    }
};


}// namespace __old

}// namespace NC


#endif//LEARNKIT_BTREE_H
