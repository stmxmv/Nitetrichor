/*!
* Created by Molybdenum on 9/24/21.
*
*/
#ifndef NITETRICHOR_SAM_HPP
#define NITETRICHOR_SAM_HPP


namespace NC {

template<typename T = int, int Size = 1000000, int ChSize = 26>
class SAM {
    T count; // state count
    T maxLen[Size];
    T link[Size];
    T trans[Size][ChSize];



public:
    SAM() noexcept : count(1), maxLen(), link(), trans() {}

    void insert(T ch) {
        T now = ++count, cur;
        maxLen[now] = maxLen[now - 1] + 1;

        for (cur = now - 1; cur && !trans[cur][ch]; cur = link[cur]) {
            trans[cur][ch] = now;
        }
        if (!cur) {

            link[now] = 1;

        } else {
            T temp = trans[cur][ch];

            if (maxLen[temp] == maxLen[cur] + 1) {
                link[now] = temp;
            } else {
                // split the state into two pieces
                T split = ++count;
                maxLen[split] = maxLen[cur] + 1;
                // copy temp's trans info
                memcpy(trans[split], trans[temp], sizeof trans[0]);


                link[split] = link[temp];
                // copy cur trans to temp to split
                while (cur && trans[cur][ch] == temp) {
                    trans[cur][ch] = split;
                    cur = link[cur];
                }

                link[temp] = link[now] = split;

            }

        }

    }

    void solve(int n) {
        T cur = 1;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < ChSize; ++j) {
                if (trans[cur][j]) {
                    if (i == 0) {

                        cout << j;

                    } else {

                        cout << ' ' << j;
                    }
                    cur = trans[cur][j];
                    break;
                }
            }
        }
    }


    friend std::ostream &operator<<(std::ostream &os, const SAM &sam) {
        cout << "maxLen" << endl;
        for (auto i : sam.maxLen) {
            cout << i << ' ' ;
        }
        cout << endl;
        cout << "link" << endl;
        for (auto i : sam.link) {
            cout << i << ' ' ;
        }
        cout << endl;
        cout << "trans" << endl;
        for (int i = 0; i < Size; ++i) {
            for (int j = 0; j < ChSize; ++j) {
                cout << sam.trans[i][j] << ' ' ;
            }
            cout << endl;
        }

        cout << endl;

        return os;
    }


};


template<typename T, int Size>
class SAM<T, Size, 0> {
    T count; // state count
    T maxLen[Size];
    T link[Size];
    T last;

    std::map<T/*ch*/, T/*to*/> trans[Size];



public:
    SAM() noexcept : count(1), maxLen(), link(), last(1), trans() {}

    void insert(T ch) {
        T now = ++count, cur;
        maxLen[now] = maxLen[last] + 1;

        for (cur = last; cur && !trans[cur][ch] ; cur = link[cur]) {
            trans[cur][ch] = now;
        }
        if (!cur) {

            link[now] = 1;

        } else {
            T temp = trans[cur][ch];

            if (maxLen[temp] == maxLen[cur] + 1) {
                link[now] = temp;
            } else {
                // split the state into two pieces
                T split = ++count;
                maxLen[split] = maxLen[cur] + 1;
                // copy temp's trans info
                trans[split] = trans[temp];

                link[split] = link[temp];
                // copy cur trans to temp to split
                while (cur && trans[cur][ch] == temp) {
                    trans[cur][ch] = split;
                    cur = link[cur];
                }

                link[temp] = link[now] = split;

            }

        }
        last = now;

    }

    void solve(int n) {
        T cur = 1;
        for (int i = 0; i < n; ++i) {
            auto pair = trans[cur].begin();
            if (i == 0) {

                cout << pair->first;

            } else {

                cout << ' ' << pair->first;
            }
            cur = pair->second;
        }
    }


};

}

#endif //NITETRICHOR_SAM_HPP
