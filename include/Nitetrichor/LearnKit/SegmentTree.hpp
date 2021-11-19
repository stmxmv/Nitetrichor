/*!
* Created by Molybdenum on 10/10/21.
*
*/
#ifndef NITETRICHOR_SEGMENTTREE_HPP
#define NITETRICHOR_SEGMENTTREE_HPP
namespace NC {


namespace _example {

template<typename Opera0>
struct _segmentTree_default_opera2 {
    template<typename T, typename Delta>
    T operator()(const T &value, const Delta &delta, int left, int right) const {
        return Opera0()(value, (left - right + 1) * delta);
    }
};

}


/// \brief
/// \tparam Size
/// \tparam Opera0 signature T opera(const T&, const T&)
/// \tparam Opera1 signature Delta opera1(const T& child, const T& otherChild, const Delta&, const Delta&, bool force)
/// \tparam Opera2 signature T opera2(const T& child, const T& otherChild, const Delta&, int n, bool force)
/// \tparam PushDownCon0 signature bool con(const T&, const Delta&, int n)
/// \tparam T
/// \discussion left begin at 0
template<int Size,
        typename Opera0,
        typename Opera1,
        typename Opera2,
        typename PushDownCon0,
        typename PushDownCon1,
        typename T = int,
        typename Delta = T>
class SegmentTree {

    static_assert(std::is_invocable_v<Opera0, T, T>, "Opera0 must be callable");
    static_assert(std::is_invocable_v<Opera1, T, T, Delta, Delta&, bool>, "Opera1 must be callable");
    static_assert(std::is_invocable_v<Opera2, T, T, Delta&, int, bool>, "Opera2 must be callable");
    static_assert(std::is_invocable_v<PushDownCon0, T, Delta&, int>, "PushDownCon must be callable");
    static_assert(std::is_invocable_v<PushDownCon1, T, Delta&, int>, "PushDownCon must be callable");
    struct Leaf {
        T    value;
        Delta delta;
        Leaf() : value{}, delta{} {}
        Leaf(T value, Delta delta) : value(value), delta(delta) {}
    };


    constexpr static int treeSize() {
        int ret = 1;
        while (ret < Size) {
            ret <<= 1;
        }
        ret <<= 1;
        return ret;
    }

    int size;

    Opera0 opera0;
    Opera1 opera1;
    Opera2 opera2;
    PushDownCon0 pushDownCon0;
    PushDownCon1 pushDownCon1;

    Leaf tree[treeSize()]; // size before tree to avoid tree overflowing size

    inline void _push_up(int parent) {
        tree[parent].value = opera0(tree[2 * parent].value, tree[2 * parent + 1].value);
    }

    inline void _push_down(int parent, int left, int right) {
        if (left == right || tree[parent].delta == Delta()) {
            return;
        }
        auto mid = (left + right) / 2;
        auto child = 2 * parent;
        auto temp = tree[child].value;
        tree[child].delta = opera1(temp, tree[child + 1].value, tree[child].delta, tree[parent].delta, false);
        tree[child].value = opera2(temp, tree[child + 1].value, tree[parent].delta, mid - left + 1, false);
        ++child;
        tree[child].delta = opera1(tree[child].value, temp, tree[child].delta, tree[parent].delta, false);
        tree[child].value = opera2(tree[child].value, temp, tree[parent].delta, right - mid, false);

        tree[parent].delta = Delta();
    }

    template<typename Iter>
    inline void _init(const Iter begin, int parent, int left, int right) {
        if (left == right) {
            tree[parent].value = *(begin + left);
            return;
        }
        auto mid = (left + right) / 2;
        _init(begin, 2 * parent, left, mid);
        _init(begin, 2 * parent + 1, mid + 1, right);
        _push_up(parent);
    }


    inline void _update(int parent, int cLeft, int cRight, int left, int right, Delta delta) {
        if (right < cLeft || cRight < left || pushDownCon0(tree[parent].value, delta, right - left + 1)) { return; }
        if (cLeft <= left && right <= cRight && pushDownCon1(tree[parent].value, delta, right - left + 1)) {
            const auto temp = tree[parent].value;
            tree[parent].value = opera2(temp, {}, delta, right - left + 1, true);
            tree[parent].delta = opera1(temp, {}, tree[parent].delta, delta, true);
            return;
        }
        _push_down(parent, left, right);
        auto mid = (left + right) / 2;
        if (cLeft <= mid) {
            _update(2 * parent, cLeft, cRight, left, mid, delta);
        }
        if (mid + 1 <= cRight) {
            _update(2 * parent + 1, cLeft, cRight, mid + 1, right, delta);
        }
        _push_up(parent);
    }

    inline T _query(int parent, int qLeft, int qRight, int left, int right) {
        if (right < qLeft || qRight < left) { return {}; }
        if (qLeft <= left && right <= qRight) {
            return tree[parent].value;
        }
        auto mid = (left + right) / 2;
        T ret{};
        _push_down(parent, left, right);
        if (qLeft <= mid) {
            ret = opera0(ret, _query(2 * parent, qLeft, qRight, left, mid));
        }
        if (mid + 1 <= qRight) {
            ret = opera0(ret, _query(2 * parent + 1, qLeft, qRight, mid + 1, right));
        }
        return ret;
    }

public:
    SegmentTree() : size(), opera0(), opera1(), opera2(), tree() {}


    SegmentTree(Opera0 opera0, Opera1 opera1, Opera2 opera2, PushDownCon0 pushDownCon0, PushDownCon1 pushDownCon1)
            : size(), opera0(opera0), opera1(opera1), opera2(opera2), pushDownCon0(pushDownCon0),
              pushDownCon1(pushDownCon1), tree() {}

    template<typename Iter>
    void init(Iter begin, Iter end) {
        size = end - begin;
        _init(begin, 1, 0, size - 1);
    }

    void update(int left, int right, Delta delta) {
        _update(1, left, right, 0, size - 1, delta);
    }

    T query(int left, int right) {
        return _query(1, left, right, 0, size - 1);
    }


};






}
#endif //NITETRICHOR_SEGMENTTREE_HPP
