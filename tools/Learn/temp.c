// -*- Nitetrichor -*-
//===--------------------------- `target name` ---------------------------------===//
//
// temp.c
//
// Created by Molybdenum on 11/15/21.
// Copyright (c) 2021 Nitetrichor. All rights reserved.
//===----------------------------------------------------------------------===//


static __always_inline void swapI(int *restrict a, int *restrict b) {
    int t = *a;
    *a = *b;
    *b = t;
}
static __always_inline void swapL(long *restrict a, long *restrict b) {
    long t = *a;
    *a = *b;
    *b = t;
}



#define Swap(a, b) _Generic((a), \
                      int*: swapI,\
                     long*: swapL,\
                  default: swapI \
)(a, b)


static __always_inline int GetInt(void) {
    register int c = getchar();
    register int x = 0;
    for(; c < 48 || c > 57; c = getchar() )
        ;
    for(; c > 47 && c < 58; c = getchar() )
        x = ( x << 3 ) + ( x << 1 ) + ( c ^ 48 );
    return x;
}

static __always_inline long GetLong(void) {
    register long c = getchar();
    register long x = 0;
    for(; c < 48 || c > 57; c = getchar() )
        ;
    for(; c > 47 && c < 58; c = getchar() )
        x = ( x << 3 ) + ( x << 1 ) + ( c ^ 48 );
    return x;
}

static __always_inline void PutInt(int num) {
    static char buf[10];
    register int t = num;
    register int t2;
    register int cur = 0;
    while (t) {
        t2 = t % 10;
        buf[cur++] = (char)(t2 + '0');
        t /= 10;
    }
    while (cur) {
        putchar((int)buf[--cur]);
    }
}


static __always_inline void PutLong(long num) {
    static char buf[20];
    register long t = num;
    register long t2;
    register long cur = 0;
    while (t) {
        t2 = t % 10;
        buf[cur++] = (char)(t2 + '0');
        t /= 10;
    }
    while (cur) {
        putchar((int)buf[--cur]);
    }
}


static __always_inline void GetNumToCString(char * str) {
    register int ch = 0;
    for(; ch < 48 || ch > 57; ch = getchar() )
        ;
    for(; ((ch > 47 && ch < 58) || ch == '-'); ch = getchar() ) {
        *(str++) = (char)ch;
    }
    *(str) = 0;
}



#ifndef WN_GETS
#define WN_GETS(str, n) fgets(str, n, stdin)
#endif



//void Put128(unsigned __int128 x) {
//    if(x > 9) {
//        Put128(x / 10);
//    }
//    putchar((int)(x % 10 + '0'));
//}


static __always_inline void Reverse(int arr[restrict], int size) {
    int i = 0;
    while (i != size) {
        if (i == --size)
            break;
        swapI(&arr[i], &arr[size]);
        ++i;
    }
}


bool IsPrime(int num) {
    if ((!(num & 1) && num != 2) || num <= 1) {
        return false;
    }
    for (int i = 3; i * i <= num; i += 2) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}



// not dic order
void Permutation(int arr[restrict], int size, int cur) {
    if (size == cur) {
        for (int i = 0; i < size; ++i) {
            printf("%5d", arr[i]);
        }
        puts("");
        return;
    }
    for (int i = cur; i < size; ++i) {
        swapI(&arr[i], &arr[cur]);
        Permutation(arr, size, cur + 1);
        swapI(&arr[i], &arr[cur]);
    }
}



bool WNNextPermutation(int arr[restrict], int size) {
    int i = size - 1;
    for (;;) {
        --i;
        if (arr[i] < arr[i + 1]) {
            int i2 = size;
            while (arr[i] >= arr[--i2])
                ;
            // the array layout is like [...] | [i]`<`[i + 1] | >[..]>[..]..>[i2]>[..]..
            // and arr[i] < arr[i2]， arr[i2] is the least number greater than arr[i].
            swapI(&arr[i], &arr[i2]);
            // start at i1 is already reversed order, so reverse it again.
            Reverse(arr + i + 1, size - i - 1);
            return true;
        }
        // the whole array is in reversed order, it is the largest dic order, so recover it.
        if (i == 0) {
            Reverse(arr, size);
            return false;
        }
    }
}

bool IsPalindrome(int num) {
    int temp = num;
    int copy = 0;
    while (temp) {
        copy = copy * 10 + temp % 10;
        temp /= 10;
    }
    if (num == copy) {
        return true;
    }
    return false;
}



#define WN_INFO_BITS (!!(1) * 3)




#define PRIMES_MAX_NUM 100
// call this function multiple time have no effect.
int * WNPrimesCreate(int * restrict outCnt) {
    static bool isCom[PRIMES_MAX_NUM + 1] = {0};
    static int primes[PRIMES_MAX_NUM] = {0};
    static int cnt = 0;
    if (cnt) {
        *outCnt = cnt;
        return primes;
    }

    for(int i = 2; i <= PRIMES_MAX_NUM; i++) {
        if(!isCom[i]) {
            primes[cnt]=i;
            ++cnt;
        }
        for(int j = 0; j < cnt && i * primes[j] <= PRIMES_MAX_NUM ;j++ ) {
            isCom[i * primes[j]] = true;
            if(i % primes[j] == 0)
                break;
        }
    }
    *outCnt = cnt;
    return primes;
}


void NCQuickSort(int * arr, int size) {
    // has one or none
    if (size == 0 || size == 1) {
        return;
    }

    // use central element as pivot
    int  pivot = arr[size / 2];
    int * lCur = arr;
    int * rCur = arr + size - 1;

    while (lCur <= rCur) {
        // find first *lCur < pivot is false
        while (*lCur < pivot) {
            ++lCur;
        }
        // find last *rCur > pivot is false
        while (pivot < *rCur) {
            --rCur;
        }

        if (lCur <= rCur) {
            Swap(lCur, rCur);
            ++lCur;
            --rCur;
        }
    }
    // now rCur < lCur
    // include *rCur, so add extra 1
    NCQuickSort(arr, (int) (rCur - arr + 1));
    NCQuickSort(lCur, (int) (size - (lCur - arr)));
}

long fact(long num, long cnt) {
    return cnt <= 1 ? num : num * fact(num - 1, cnt - 1);
}
