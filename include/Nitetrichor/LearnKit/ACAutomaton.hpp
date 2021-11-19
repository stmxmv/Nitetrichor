/*!
* Created by Molybdenum on 9/19/21.
*
*/
#ifndef NITETRICHOR_ACAUTOMATON_HPP
#define NITETRICHOR_ACAUTOMATON_HPP
namespace NC {

namespace String {

template<typename T = int, int Size = 1000000, int NumOfPattern = 200>
class ACAutomaton {
public:
    constexpr static int childrenNum = 26;
    struct Node {
        T fail;
        T cnt;
        T encounter;
        T children[childrenNum];
    };
    static_assert(std::is_integral<T>::value, "Integral Required!!");
private:

    T trieCnt;

    T pos[NumOfPattern];
    T posCnt;
    std::array<Node, Size> trie;
    std::array<std::pair<T/* pos */, T/* encounter */>, NumOfPattern> encounterCache;

    struct {
        T head[Size];
        T next[Size];
    } dfs;

    inline void _before_dfs() {
        // generate the tree of fail
        for (T i = 1; i <= trieCnt; ++i) {
            auto fail = trie[i].fail; // fail is the parent's node
            // connect the current node to the front of the `next` list, which is head[parent]
            // e.g.
            //                   parent
            //                      |
            // the next list x1 <- x2 <- x3 ; connect x3 to x2, which is head[parent]
            dfs.next[i] = dfs.head[fail];
            // then, change parent point to x3, the front of the `next` list
            dfs.head[fail] = i;
        }
    }

    inline void _dfs(T cur = 0) {
        // evaluate the encounter
        for (T i = dfs.head[cur]; i; i = dfs.next[i]) {
            _dfs(i);
            trie[cur].encounter += trie[i].encounter;
        }
    }


public:

    ACAutomaton() noexcept: trieCnt{}, pos{}, posCnt{}, trie{}, encounterCache{}, dfs{} {}

    void clear() {
        trie.fill({});
        encounterCache.fill({});
        trieCnt = 0;
        memset(pos, 0, sizeof pos);
        posCnt = 0;
        memset(&dfs, 0, sizeof dfs);
    }

    void insert(const char *str) {
        T cur = 0;
        for (; *str; ++str) {
            int idx = *str - 'a';
            if (!trie[cur].children[idx]) {
                trie[cur].children[idx] = ++trieCnt;
            }
            cur = trie[cur].children[idx];
        }
        ++trie[cur].cnt;
        pos[posCnt++] = cur;
    }

    void makeFail() {
        std::queue<T> queue;
        // first process the second level's fail
        for (int i = 0; i < childrenNum; ++i) {
            auto child = trie[0].children[i];
            if (child) {
                trie[child].fail = 0;
                queue.push(child);
            }
        }

        while (!queue.empty()) {
            auto front = queue.front();
            queue.pop();
            for (int i = 0; i < childrenNum; ++i) {
                auto child = trie[front].children[i];
                if (child) {
                    trie[child].fail = trie[trie[front].fail].children[i];
                    queue.push(child);
                } else {
                    trie[front].children[i] = trie[trie[front].fail].children[i];
                }
            }
        }
    }

    /// \brief
    /// \param str
    /// \return num of the format string matches
    T query(const char *str) {
        T ret = 0, cur = 0;
        for (; *str; ++str) {
            cur = trie[cur].children[*str - 'a'];
            for (T j = cur; j && trie[j].cnt != -1; j = trie[j].fail) {
                ret += trie[j].cnt;
                trie[j].cnt = -1;
            }
        }
        return ret;
    }

    /// \brief
    /// \param str
    /// \return position and num of encounter of format strings
    auto queryEncounter(const char *str) ->
    std::pair<decltype(encounterCache.begin()), decltype(encounterCache.end())> /* pos , encounter */{


        T cur = 0;
        for (; *str; ++str) {
            cur = trie[cur].children[*str - 'a'];
            ++trie[cur].encounter;
        }
        _before_dfs();
        _dfs();
        for (T i = 0; i < posCnt; ++i) {
            encounterCache[i] = {i, trie[pos[i]].encounter};
        }
        return {encounterCache.begin(), encounterCache.begin() + posCnt};
    }

};

}

}

#endif //NITETRICHOR_ACAUTOMATON_HPP
