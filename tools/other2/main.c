// -*- Nitetrichor -*-
//===------------------------------ Learn ---------------------------------===//
//
// main.c
//
// Created by Molybdenum on 8/14/21.
//
//===----------------------------------------------------------------------===//


#include <stdio.h>

#ifndef __asm_volatile_goto
#define __asm_volatile_goto(x...) asm goto(x)
#endif
#ifndef __always_inline
#define __always_inline inline __attribute__((always_inline))
#endif
#ifndef __noinline
#define __noinline __attribute__((noinline))
#endif
#ifndef __stringify_1
#define __stringify_1(x...) #x
#endif
#ifndef __stringify
#define __stringify(x...) __stringify_1(x)
#endif
#ifndef __same_type
#define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))
#endif

#define AN_EXPECT(exp, c) __builtin_expect(exp, c)
#define AN_LIKELY(x)      __builtin_expect(!!(x), 1)
#define AN_UNLIKELY(x)    __builtin_expect(!!(x), 0)
#define POP_COUNT(x)      __builtin_popcount(x)

// legacy bool
#ifndef BOOL
#define BOOL unsigned char
#endif
#ifndef YES
#define YES 1
#endif
#ifndef NO
#define NO 0
#endif

#define None ((void *) 0)

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define INF       0x3f3f3f3f
#define ABS(x) ((x) < 0 ? (-(x)) : (x))

#define STRING_MAX_LENGTH 100



//#define swap(a, b) \
//	do { typeof(a) __tmp = (a); (a) = (b); (b) = __tmp; } while (0)


#define swap(a, b) \
	do { __auto_type __tmp = (a); (a) = (b); (b) = __tmp; } while (0)

int is_same_string(const char *restrict str1, const char *restrict str2) {
    for (;; ++str1, ++str2) {
        if (!*str1 && !*str2) { break; }
        if (*str1 == *str2) { continue; }
        return 0;
    }
    return 1;
}


int find_index(int vtx_num, const char str[restrict][STRING_MAX_LENGTH], const char *restrict cmp) {
    for (int i = 0; i < vtx_num; ++i) {
        if (is_same_string(str[i], cmp)) {
            return i;
        }
    }
    return -1;
}


void m_print(int n, int mtx[restrict static n][n]) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j == 0) {
                printf(mtx[i][0] == INF ? "INF" : "%d", mtx[i][j]);
            } else {
                printf(mtx[i][j] == INF ? " INF" : " %d", mtx[i][j]);
            }
            //            printf(j == 0 ? "%d" : " %d", mtx[i][j]);
        }
        puts("");
    }
}


/// \brief use floyd algorithm
/// \param n
/// \param mtx
void m_transform_adjacency_to_reachability(int n, int mtx[restrict static n][n]) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                mtx[j][k] = mtx[j][k] || (mtx[j][i] && mtx[i][k]);
            }
        }
    }
}


void m_topology_sort(int n, int mtx[restrict static n][n], BOOL vis[restrict static n], int *out) {
    for (int i = 0; i < n;) {
        //col
        if (vis[i]) { goto find_next; }
        for (int j = 0; j < n; ++j) {
            if (mtx[j][i]) {
                goto find_next;
            }
        }

        // zero
        vis[i] = YES;
        *out++ = i;
        for (int j = 0; j < n; ++j) {
            mtx[i][j] = 0;
        }
        i = 0;
        continue;
    find_next:
        ++i;
    }
}


int arr_search(int n, int arr[restrict static n], int val) {
    --n;
    if (arr[n] == val) { return n; }

    int elem_n = arr[n];
    arr[n]     = val;

    int i = 0;
    for (; arr[i] != val; ++i)
        ;

    arr[n] = elem_n;

    return i == n ? -1 : i;
}

int arr_upper_bound(int n, const int arr[restrict static n], int val) {
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

int arr_lower_bound(int n, const int arr[restrict static n], int val) {
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


//#define EPSILON 1e-6
//#include <math.h>
//double ANSqrt(double num) {
//    double l = 0, r = num;
//    while (fabs(l - r) >= EPSILON) {
//        l = (r + num / r) / 2;
//        swap(l, r);
//    }
//    return l;
//}



void tree_print_balance(const int len, const int cur, const char tree[], int *depth) {
    if (cur >= len || tree[cur] == '0') {
        *depth = 0;
        return;
    }

    int lchild_depth, rchild_depth;
    tree_print_balance(len, cur * 2 + 1, tree, &lchild_depth);
    tree_print_balance(len, cur * 2 + 2, tree, &rchild_depth);

    printf("%c %d\n", tree[cur], lchild_depth - rchild_depth);

    *depth = 1 + MAX(lchild_depth, rchild_depth);

}

static inline int hash_func(int key) {
    return key % 11;
}

void hash_insert(int len, int arr[len], int key) {
    int hash_value = hash_func(key);
    if (arr[hash_value] == -1) {
        arr[hash_value] = key;
    } else {
        int cnt = 1;
        int d = 0;
        while (cnt < len && ABS(d) <= len / 2) {
            ++cnt;
            d = d <= 0 ? -d + 1 : -d;
            int hash = (hash_value + d * ABS(d)) % len;
            if (hash < 0) {
                hash += len;
            }
            if (arr[hash] == -1) {
                arr[hash] = key;
                return;
            }
        }
    }
}

static int find_cnt = 0;

int hash_search(int len, const int arr[len], int key) {
    int hash_value = hash_func(key);
    find_cnt = 0;
    int d = 0;
    do {
        ++find_cnt;

        int hash = (hash_value + d * ABS(d)) % len;
        if (hash < 0) {
            hash += len;
        }

        if (arr[hash] == key) {
            return hash;
        }
        if (arr[hash] == -1) {
            return -1;
        }

        d = d <= 0 ? -d + 1 : -d;

    } while (find_cnt < len && ABS(d) <= len / 2);
    return -1;
}

int main(void) {
    // main entry here

    //    freopen("OJ/in.txt", "r", stdin);
    //    freopen("OJ/out.txt", "w", stdout);


    int t;
    scanf("%d", &t);

    while (t--) {
        int m, n;
        scanf("%d %d", &m, &n);

        int table[m];

        for (int i = 0; i < m; ++i) {
            table[i] = -1;
        }

        for (int i = 0; i < n; ++i) {
            int key;
            scanf("%d", &key);
            hash_insert(m, table, key);
        }

        /// output
        for (int i = 0; i < m; ++i) {
            if (table[i] >= 0) {
                printf(i == 0 ? "%d" : " %d", table[i]);
            } else {
                printf(i == 0 ? "NULL" : " NULL");
            }
        }
        puts("");

        int k;
        scanf("%d", &k);
        for (int i = 0; i < k; ++i) {
            int key;
            scanf("%d", &key);
            int res = hash_search(m, table, key);
            printf(res < 0 ? "0 %d\n" : "1 %d %d\n" , find_cnt, res + 1);
        }
    }


    return 0;
}
