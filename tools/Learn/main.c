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

#define None ((void*)0)

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define INF 0x3f3f3f3f

#define STRING_MAX_LENGTH 100

int is_same_string(const char *restrict str1, const char *restrict str2) {
    for (; ; ++str1, ++str2) {
        if (!*str1 && !*str2) { break; }
        if (*str1 == *str2) { continue; }
        return 0;
    }
    return 1;
}


int find_index(int vtx_num, const char str[restrict][STRING_MAX_LENGTH], const char *cmp) {
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
                printf("%d", mtx[i][0]);
            } else {
                printf(" %d", mtx[i][j]);
            }
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


int main(void) {
    // main entry here

//    freopen("OJ/in.txt", "r", stdin);
//    freopen("OJ/out.txt", "w", stdout);

    int t;
    scanf("%d", &t);
    while (t--) {
        int vtx_num;
        scanf("%d", &vtx_num);

        int mtx[vtx_num][vtx_num];

        for (int i = 0; i < vtx_num; ++i) {
            for (int j = 0; j < vtx_num; ++j) {
                scanf("%d", &mtx[i][j]);
            }
        }

        m_transform_adjacency_to_reachability(vtx_num, mtx);

//        m_print(vtx_num, mtx);
        for (int i = 0; i < vtx_num; ++i) {
            for (int j = 0; j < vtx_num; ++j) {
                if (mtx[i][j] == 0) {
                    puts("No");
                    goto next_test;
                }
            }
        }

        puts("Yes");

    next_test:
        None;
    }



    return 0;
}