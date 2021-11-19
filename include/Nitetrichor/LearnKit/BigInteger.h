/*!
* Created by Molybdenum on 9/13/21.
*
*/
#ifndef NITETRICHOR_BIGINTEGER_H
#define NITETRICHOR_BIGINTEGER_H


#define WN_BIG_INTEGER_BIT_LENGTH 2048
#define WN_MAX_INTEGER_BASE_10 100000000
typedef struct s_WNBigInteger {
    int  width;
    int  integer[WN_BIG_INTEGER_BIT_LENGTH];
    bool isNegative;
} WNBigInteger, *WNBigIntegerRef;


void WNBigIntegerInitZero(WNBigIntegerRef self) {
    memset(self, 0, sizeof(struct s_WNBigInteger));
}

void WNBigIntegerInitWithCString(WNBigIntegerRef self, const char * str) {
    memset(self, 0, sizeof(struct s_WNBigInteger));
    if (str[0] == '-') {
        self->isNegative = true;
        ++str;
    }
    int pos = 0, base = 1;
    for (int i = (int)strlen(str) - 1; i >= 0; --i) {
        if (base == WN_MAX_INTEGER_BASE_10) {
            ++pos;
            ++self->width;
            base = 1;
        }
        self->integer[pos] += base * (str[i] - '0');
        base *= 10;
    }
}

void WNBigIntegerInitWithInt(WNBigIntegerRef self, int value) {
    memset(self, 0, sizeof(struct s_WNBigInteger));
    self->integer[0] = value;
}
void WNBigIntegerSub(WNBigIntegerRef dst, WNBigIntegerRef src1, WNBigIntegerRef src2);

void WNBigIntegerAdd(WNBigIntegerRef dst, WNBigIntegerRef src1, WNBigIntegerRef src2) {
    if (!src1->isNegative && src2->isNegative) {
        src2->isNegative = false;
        WNBigIntegerSub(dst, src1, src2);
        src2->isNegative = true;
        return;
    } else if (src1->isNegative && !src2->isNegative) {
        src1->isNegative = false;
        WNBigIntegerSub(dst, src2, src1);
        src1->isNegative = true;
        return;
    }
    dst->isNegative = src1->isNegative && src2->isNegative;

    int wide = MIN(src1->width, src2->width);
    int carry = 0, i = 0;
    for (; i <= wide || (carry != 0 && i < WN_BIG_INTEGER_BIT_LENGTH) ; ++i) {
        int temp = src1->integer[i] + src2->integer[i] + carry;
        dst->integer[i] = temp % WN_MAX_INTEGER_BASE_10;
        carry = temp / WN_MAX_INTEGER_BASE_10;
    }
    dst->width = i - 1;
}

void WNBigIntegerSub(WNBigIntegerRef dst, WNBigIntegerRef src1, WNBigIntegerRef src2) {
    if (src1->isNegative && !src2->isNegative) {
        src2->isNegative = true;
        WNBigIntegerAdd(dst, src1, src2);
        src2->isNegative = false;
        return;
    } else if (!src1->isNegative && src2->isNegative) {
        src2->isNegative = false;
        WNBigIntegerAdd(dst, src1, src2);
        src2->isNegative = true;
        return;
    }


    if (src1->width < src2->width) {
        WNBigIntegerRef t = src1;
        src1 = src2;
        src2 = t;
        dst->isNegative = !src1->isNegative;
    } else if (src1->width == src2->width) {
        for (int i = src1->width; i >= 0; --i) {
            if (src1->integer[i] == src2->integer[i]) {
                continue;
            }
            else if (src1->integer[i] < src2->integer[i]) {
                WNBigIntegerRef t = src1;
                src1 = src2;
                src2 = t;
                dst->isNegative = !src1->isNegative;
                break;
            } else {
                dst->isNegative = src1->isNegative; // abs(src1) is larger.
                break;
            }
        }
    } else {
        dst->isNegative = src1->isNegative;
    }
    int wide = src1->width;
    int carry = 0, i = 0;
    for (; i <= wide || (carry != 0 && i < WN_BIG_INTEGER_BIT_LENGTH) ; ++i) {
        int temp = src1->integer[i] - src2->integer[i] - carry;
        carry = 0;
        if (temp < 0) {
            temp += WN_MAX_INTEGER_BASE_10;
            ++carry;
        }
        dst->integer[i] = temp % WN_MAX_INTEGER_BASE_10;
    }
    // adjust width
    while (i > 0 && !dst->integer[--i])
        ;
    dst->width = i;
}
void WNBigIntegerMul(WNBigIntegerRef restrict dst, WNBigIntegerRef restrict src1, WNBigIntegerRef restrict src2) {
    dst->isNegative = src1->isNegative ^ src2->isNegative;
    long long carry = 0;
    int i = 0, j = 0;
    for (; i <= src1->width || (carry != 0 && i < WN_BIG_INTEGER_BIT_LENGTH); ++i) {
        for (j = 0; j <= src2->width || (carry != 0 && j < WN_BIG_INTEGER_BIT_LENGTH); ++j) {
            long long temp = src1->integer[i] * src2->integer[j] + carry;
            long long temp2 = dst->integer[i + j] + temp;
            dst->integer[i + j] = (int)(temp2 % WN_MAX_INTEGER_BASE_10);
            carry = temp2 / WN_MAX_INTEGER_BASE_10;
        }
    }
    dst->width = (i - 1) + (j - 1);
}


void WNBigIntegerPrint(WNBigIntegerRef integer) {
    // zero
    if (integer->width == 0 && integer->integer[0] == 0) {
        putchar('0');
        return;
    }
    // not zero
    if (integer->isNegative) {
        putchar('-');
    }
    for (int i = integer->width; i >= 0; --i) {
        if (i != integer->width) {
            int test = WN_MAX_INTEGER_BASE_10 / 10;
            while (integer->integer[i] < test) {
                putchar('0');
                test /= 10;
            }
        }
        // if it is zero, it's already print a zero.
        if (integer->integer[i]) {
            printf("%d", integer->integer[i]);
        }
    }
}

#endif //NITETRICHOR_BIGINTEGER_H
