// -*- Nitetrichor -*-
//===--------------------------- `target name` ---------------------------------===//
//
// exam.hpp
// tools/Learn++
// Created by Molybdenum on 12/21/21.
// Copyright (c) 2021 Nitetrichor. All rights reserved.
//===----------------------------------------------------------------------===//
#ifndef NITETRICHOR_EXAM_HPP
#define NITETRICHOR_EXAM_HPP

#include <vector>
#include <queue>
#include <iostream>
using std::cout;
using std::endl;

template<typename T>
using Vector = std::vector<T>;

using Vectori = Vector<int>;

using Matrix = Vector<Vectori>;
namespace AN {

struct Edge1 {
    int cost;
    int to;
    int next;
    Edge1() = default;
    Edge1(int cost, int to, int next) : cost(cost), to(to), next(next) {}
};

struct Edge0 {
    int from, cost, to;
    Edge0() = default;
    Edge0(int from, int cost, int to) : from(from), cost(cost), to(to) {}
    bool operator<(const Edge0 &rhs) const {
        return std::tie(cost, from, to) < std::tie(rhs.cost, rhs.from, rhs.to);
    }
    bool operator>(const Edge0 &rhs) const {
        return rhs < *this;
    }
};

/// pass
class DisjointSet {
    Vectori parent;
    Vectori rank;
public:
    explicit DisjointSet(int n) noexcept : parent(n), rank(n) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }


    int find(int x) noexcept {
        while(x != parent[x]) {
            x = parent[x] = parent[parent[x]];
        }
        return x;
        //        if (parent[x] == x) {
        //            return x;
        //        } else {
        //            return parent[x] = find(parent[x]);
        //        }
    }

    void unite(int x, int y) noexcept {
        x = find(x);
        y = find(y);
        if (x == y) { return; }
        if (rank[x] < rank[y]) {
            parent[x] = y;
        } else {
            parent[y] = x;
            if (rank[x] == rank[y]) {
                ++rank[x];
            }
        }
    }

    inline bool isSame(int x, int y) noexcept { return find(x) == find(y); }

    bool isAllSame() {
        int x = find(0);
        for (int i = 1; i < (int)parent.size(); ++i) {
            if (x != find(i)) {
                return false;
            }
        }
        return true;
    }

    int kindCount() {
        int ret = 0;
        Vector<bool> set(parent.size());
        for (int i = 0; i < (int)parent.size(); ++i) {
            int kind = find(i);
            if (!set[kind]) {
                set[kind] = true;
                ++ret;
            }
        }
        return ret;
    }

};



struct Graph0 {
    Vector<Edge0> edges;
    int numOfVertexes;

    explicit Graph0(int numOfVertexes) : numOfVertexes(numOfVertexes) {}

    void insert(int from, int cost, int to) {
        edges.emplace_back(from, cost, to);
    }

    Vector<Edge0> spanningTree;
    int Kruskal() {
        DisjointSet findSet(numOfVertexes);
        int ret = 0, cnt = 0;
        std::sort(edges.begin(), edges.end());

        for (int i = 0; i < edges.size(); ++i) {
            if (findSet.isSame(edges[i].from, edges[i].to)) {
                // bridged
                continue;
            }
            ret += edges[i].cost;
            spanningTree.emplace_back(edges[i].from, edges[i].cost, edges[i].to);

            findSet.unite(edges[i].from, edges[i].to);

            ++cnt;
            if (cnt == numOfVertexes - 1) {
                return ret;
            }

        }
        if (cnt == numOfVertexes - 1) {
            return ret;
        }
        return 0;
    }

};

struct Graph1 {



    Vectori head;
    Vector<Edge1> table;

    struct path_elem {
        int cost;
        int from;
        path_elem(int cost, int last) : cost(cost), from(last) {}
        bool operator<(const path_elem &rhs) const {
            return std::tie(cost, from) < std::tie(rhs.cost, rhs.from);
        }
    };

    constexpr static int INF = 0x3f3f3f3f;

    Vector<path_elem> path;

    explicit Graph1(int num) : head(num), table(2) {}

    void insert(int from, int cost, int to) {
        table.emplace_back(cost,  to,  head[from]);
        head[from] = table.size() - 1;
    }

    void dijkstra(int start) {
        Vector<bool> vis(head.size());
        path.resize(head.size(), {INF, 0});

        path[start] = {0, start};

        std::priority_queue<path_elem, std::vector<path_elem>> queue;
        queue.emplace(0, start);

        while (!queue.empty()) {
            int from = queue.top().from;
            queue.pop();

            if (vis[from]) { continue; }
            vis[from] = true;

            for (int cur = head[from]; cur; cur = table[cur].next) {
                auto edge = table[cur];
                if (!vis[edge.to] && path[edge.to].cost > path[from].cost + edge.cost) {

                    path[edge.to].cost = path[from].cost + edge.cost;
                    path[edge.to].from = from;

                    queue.emplace(path[edge.to].cost, edge.to);

                }
            }
        }
    }


    Vector<Edge0> spanningTree;

    int prim(int start) {
        Vectori distance(head.size(), INF);
        Vector<bool> vis(head.size());
        int ret   = 0;
        int count = 0;

        struct queue_elem {
            int from, cost, to;
            queue_elem(int from, int cost, int to) : from(from), cost(cost), to(to) {}
            bool operator<(const queue_elem &rhs) const {
                return std::tie(cost, from, to) < std::tie(rhs.cost, rhs.from, rhs.to);
            }
            bool operator>(const queue_elem &rhs) const {
                return rhs < *this;
            }
        };

        std::priority_queue<queue_elem,
                            std::vector<queue_elem>,
                            std::greater<>>
                queue;
        queue.emplace(start, 0, start);
        while (!queue.empty() && count < head.size()) {

            auto front = queue.top();
            queue.pop();

            if (vis[front.to]) { continue; }
            vis[front.to] = true;

            ++count;
            ret += front.cost;
            spanningTree.emplace_back(front.from,  front.to, front.cost);
            // slack
            for (auto cur = head[front.to]; cur; cur = table[cur].next) {
                if (!vis[table[cur].to] && table[cur].cost < distance[table[cur].to]) {
                    distance[table[cur].to] = table[cur].cost;
                    queue.emplace(front.to, table[cur].cost, table[cur].to);
                }
            }
        }

        if (count == head.size()) {
            return ret;
        }
        return 0;
    }
};



}


static constexpr int INF = 0x3f3f3f3f;


void m_transform_adjacency_to_reachability(int n, Matrix& mtx) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                mtx[j][k] = std::min(mtx[j][k], mtx[j][i] + mtx[i][k]);
            }
        }
    }
}

void m_print(int n, const Matrix& m) {
    for (int i = 0; i < n; ++i) {
        bool space = false;
        for (int j = 0; j < n; ++j) {
            if (std::exchange(space, true)) {
                cout << ' ';
            }
            if (m[i][j] == INF) {
                cout << "N";
            } else {
                cout << m[i][j];
            }
        }
        cout << endl;
    }
}

void v_print(const Vectori &vec) {
    for (int i = 0; i < vec.size(); ++i) {
        if (i != 0) {
            cout << ' ';
        }
        cout << vec[i];
    }
    cout << endl;
}


/// pass
template<typename Iter>
void __quick_sort(int size, Iter arr, int n, Iter outArr) {
    // has one or none
    if (size == 0 || size == 1) {
        return;
    }

    // use central element as pivot
    int  pivot = arr[0];
    Iter lCur = arr;
    Iter rCur = arr + size - 1;

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


    /// verbose print

    for (int i = 0; i < n; ++i) {
        if (i != 0) {
            cout << ' ';
        }
        cout << outArr[i];
    }
    cout << endl;

    // now rCur < lCur
    // include *rCur, so add extra 1
    quick_sort(rCur - arr, arr, n, outArr);
    quick_sort(size - (lCur - arr) - 1, lCur + 1, n, outArr);
}

/// pass
template<typename Iter>
void quick_sort(int n, Iter arr) {

    if (n == 0 || n == 1) { return ; }

    int left = 0;
    int right = n - 1;

    int pivot = arr[0];

    while (left < right) {

        while (left < right && arr[right] >= pivot) {
            --right;
        }

        arr[left] = arr[right];


        while (left < right && arr[left] <= pivot) {
            ++left;
        }

        arr[right] = arr[left];

    }

    assert(left == right);


    arr[left] = pivot;

    an_quick_sort(left, arr);

    an_quick_sort(n - left - 1, arr + left + 1);

}

/// pass
template<typename Iter>
void _quick_sort(int size, Iter arr) {
    // has one or none
    if (size == 0 || size == 1) {
        return;
    }

    // use first element as pivot
    int  pivot = arr[0];
    Iter lCur = arr;
    Iter rCur = arr + size - 1;

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


    // now rCur == lCur
    // include *rCur, so add extra 1
    quick_sort(rCur - arr, arr);
    quick_sort(size - (lCur - arr) - 1, lCur + 1);
}

/// pass
template<typename Iter>
void insert_sort(int n, Iter arr) {

    for (int i = 1; i < n; ++i) {
        int k;
        int tmp = arr[i];
        for (k = i; k - 1 >= 0 && arr[k - 1] > tmp ; --k) {
            arr[k] = arr[k - 1];
        }

        arr[k] = tmp;

//        /// verbose print
//        for (int j = 0; j < n; ++j) {
//            if (i != 0) {
//                cout << ' ';
//            }
//            cout << arr[i];
//        }
//        cout << endl;
    }

}


/// pass
template<typename Iter>
void shell_sort(int n, Iter arr) {
    int gap = n;

    do {
        gap = gap / 2;
        for (int i = 0; i < gap; ++i) {

            for (int j = i + gap; j < n; j += gap) {
                int k;
                int tmp = arr[j];
                for (k = j; k - gap >= 0 && tmp < arr[k - gap] ; k -= gap) {
                    arr[k] = arr[k - gap];
                }

                arr[k] = tmp;
            }

        }

//        /// verbose print
//        for (int i = 0; i < n; ++i) {
//            if (i != 0) {
//                cout << ' ';
//            }
//            cout << arr[i];
//        }
//        cout << endl;


    } while (gap != 1);
}


/// pass
template<typename Iter>
int arr_upper_bound(int n, Iter arr, int val) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = (right + left) / 2;
        if (val < arr[mid]) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return left;
}


/// pass
template<typename Iter>
int arr_lower_bound(int n, Iter arr, int val) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = (right + left) / 2;
        if (val <= arr[mid]) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return left;
}


/// pass
template<typename Iter>
void heap_push(Iter begin, Iter end) {
    --end;
    auto val = *end;
    while (end > begin) {
        Iter parent = begin + (end - begin - 1) / 2; // the parent node
        if (val < *parent) {
            break;
        }
        *end = *parent; // let the parent down
        end = parent;
    }
    *end = val;
}

/// pass
template<typename Iter>
void sift_down(Iter begin, Iter end, Iter start) {
    --end;
    auto val = *start;
    Iter cur = start;
    while (begin + (cur - begin) * 2 + 1 <= end) {
        Iter child = begin + (cur - begin) * 2 + 1;
        if (child + 1 <= end && *child < *(child + 1)) {
            ++child;
        }
        if (*child < val) {
            break;
        }
        // let the child up
        *cur = *child;
        cur = child;
    }
    *cur = val;
}

/// pass
template<typename Iter>
inline void heap_pop(Iter begin, Iter end) {
    std::iter_swap(begin, --end);
    sift_down(begin, end, begin);
}



/// pass
template<typename Iter>
void heap_make(Iter begin, Iter end) {
    int len = end - begin;
    if (len > 1) {
        for (int i = (len - 2) / 2; i >= 0; --i) {
            sift_down(begin, end, begin + i);
        }
    }
}

/// pass
template<typename Iter1, typename Iter2>
void floyd(int n, Iter1 mtx, Iter2 from) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                if (mtx[j][k] < mtx[j][i] + mtx[i][k]) {
                    mtx[j][k] = mtx[j][i] + mtx[i][k];
                    from[j][k] = i;
                }
            }
        }
    }
}

/// pass
template<typename Iter, typename Callback>
void floyd_path(int from,int to, Iter path, Callback callback) {
    if (from == to) return;
    if(path[from][to] == 0 ) {
        callback(to);
    }
    else{
        floyd_path(from, path[from][to], path, callback);
        floyd_path(path[from][to], to, path, callback);
    }
}

template<typename Iter>
void radix_sort(int n, Iter arr) {
    Vectori result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = i;
    }
    Vectori rank(n);
    Vectori bucket(10);
    int length = 0;


    int max = *std::max_element(arr, arr + n);
    while (max) {
        ++length;
        max /= 10;
    }

    int base = 1;
    for (int i = 0; i < length; ++i, base *= 10) {

        std::fill(bucket.begin(), bucket.end(), 0);

        for (int j = 0; j < n; ++j) {
            int val = arr[j] / base;
            val %= 10;
            rank[j] = val;
            ++bucket[rank[j]];
        }

        std::partial_sum(bucket.begin(), bucket.end(), bucket.begin());

        auto last_result = result;


        for (int j = n - 1; j >= 0; --j) {
            result[--bucket[rank[last_result[j]]]] = last_result[j];
        }


    }

    /// do sort
    Vector<typename std::iterator_traits<Iter>::value_type> copy(arr, arr + n);
    for (int i = 0; i < n; ++i) {
        arr[i] = std::move(copy[result[i]]);
    }
}



/// pass
template<typename String>
struct Kmp {
    const String &string;

    int length;
    Vectori next/* = {-1, 0}*/;

    void make_next() {
        for (int i = 0, j = 1; j < length; ) {
            if (string[i] == string[j]) {
                ++i, ++j;
                next.push_back(i);
            } else {

                int fail = next[i];

                if (fail < 0) {
                    i = 0;
                    ++j;
                    next.push_back(0);
                } else {
                    i = fail;
                }

            }

        }
    }


    Kmp(const String &string, int length) noexcept
        :  string(string), length(length), next({-1, 0}) {
        make_next();
    }


    template<typename ComString>
    int findFirstOne(const ComString &otherString, int otherLength) noexcept {
        if (length > otherLength) {
            return -1;
        }

        for (int i = 0, j = 0; i < otherLength; ) {
            if (otherString[i] != string[j]) {
                int fail = next[j];
                if (fail < 0) {
                    j = 0;
                    ++i;
                } else {
                    j = fail;
                }
            } else {
                ++i, ++j;

                if (j == length) {
                    return i - j;
                }
            }
        }

        return -1;
    }

    template<typename ComString, typename OutIter>
    OutIter findAll(const ComString &otherString, int otherLength, OutIter out) noexcept {
        if (length > otherLength) {
            return out;
        }
        for (int i = 0, j = 0; i < otherLength; ) {
            if (otherString[i] != string[j]) {
                int fail = next[j];
                if (fail < 0) {
                    j = 0;
                    ++i;
                } else {
                    j = fail;
                }
            } else {
                ++i, ++j;

                if (j == length) {
                    *out++ = i - j;
                    j = next[length];
                }
            }
        }

        return out;
    }

};



template<typename T>
struct BTree {

    struct Leaf {
        T data;
        std::unique_ptr<Leaf> lChild, rChild;
        Leaf(T data) : data(data) {}
    };


    template<typename Callback>
    void __inorder(std::unique_ptr<Leaf> &cur, const Callback &callback) {
        if (!cur) { return; }

        __inorder(cur->lChild, callback);

        callback(cur->data);

        __inorder(cur->rChild, callback);

    }


    template<typename Callback>
    void __postorder(std::unique_ptr<Leaf> &cur, const Callback &callback) {
        if (!cur) { return; }

        __postorder(cur->lChild, callback);

        __postorder(cur->rChild, callback);
        callback(cur->data);

    }

    bool __exist(std::unique_ptr<Leaf> &cur, T val) {

        if (!cur) { return false; }

        if (cur->data == val) { return true; }

        if (cur->data > val) {
            return __exist(cur->lChild, val);
        }

        return __exist(cur->rChild, val);
    }

    template<typename Iter>
    void __build_tree(std::unique_ptr<Leaf> &cur, Iter &begin, const Iter &end) {
        if (begin >= end || *begin == '0') {
            begin++;
            return;
        }
        cur = std::make_unique<Leaf>(*begin++);
        __build_tree(cur->lChild, begin, end);
        __build_tree(cur->rChild, begin, end);
    }


    std::unique_ptr<Leaf> root;
    int cnt;

    template<typename Iter>
    BTree(Iter begin, Iter end) : cnt() {
        __build_tree(root, begin, end);
    }



    template<typename Callback>
    void inorder(const Callback &callback) {
        __inorder(root, callback);
    }

    bool exist(T val) {
        return __exist(root, val);
    }

    template<typename Callback>
    void postorder(const Callback &callback) {
        __postorder(root, callback);
    }


};


template<typename T>
class BST {

    struct Leaf {
        T data;
        std::unique_ptr<Leaf> lChild, rChild;
    };


    void __insert(std::unique_ptr<Leaf> &cur, const T &data) {

        if (!cur) {
            cur = std::make_unique<Leaf>();
            cur->data = data;
            return;
        }

        if (cur->data == data) {
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



/// pass
/// I don't wanna mess with destructor !!!
struct List {
    struct Node {
        int data;
        std::weak_ptr<Node> parent;
        std::shared_ptr<Node> child;
        Node() = default;
        Node(int data) : data(data) {}
    };

    struct wrap_iter : std::iterator<std::bidirectional_iterator_tag, int> {
        std::shared_ptr<Node> node;

        explicit wrap_iter(std::shared_ptr<Node> node) : node(std::move(node)) {}

        wrap_iter &operator--() {
            node = node->parent.lock();
            return *this;
        };

        wrap_iter operator--(int) {
            wrap_iter ret = *this;
            --(*this);
            return ret;
        };

        wrap_iter &operator++() {
            node = node->child;
            return *this;
        };

        wrap_iter operator++(int) {
            wrap_iter ret = *this;
            ++(*this);
            return ret;
        };

        bool operator==(const wrap_iter &rhs) const {
            return node == rhs.node;
        }
        bool operator!=(const wrap_iter &rhs) const {
            return !(rhs == *this);
        }

        int operator*() const {
            return node->data;
        }
    };

    std::shared_ptr<Node> root;

    List() {
        root = std::make_shared<Node>();
        root->parent = root;
        root->child = root;
    }

    ~List() {
        root->child = nullptr;
    }

    void insert(std::shared_ptr<Node> &node, int data) {
        std::shared_ptr<Node> newNode = std::make_shared<Node>(data);

        newNode->parent = node->parent;
        newNode->child = node;

        node->parent.lock()->child = newNode;
        node->parent = newNode;
    }

    void push_back(int data) {
        insert(root, data);
    }

    wrap_iter begin() const { return wrap_iter(root->child); }

    wrap_iter end() const { return wrap_iter(root); }



};








#endif//NITETRICHOR_EXAM_HPP
