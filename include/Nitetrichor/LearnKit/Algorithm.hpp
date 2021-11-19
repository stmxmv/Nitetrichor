/*!
* Created by Molybdenum on 9/2/21.
*
*/
#ifndef NITETRICHOR_ALGORITHM_HPP
#define NITETRICHOR_ALGORITHM_HPP

#include "LearnKit/sort.hpp"
#include "LearnKit/hash.hpp"


namespace NC {
/// \brief not recommended, just show the recursion type of pow.
template<typename T, typename U>
T PowR(T x, U y) {
    if (y == 0) { return 1; }
    if (y & 1) { return x * PowR(x, y - 1); }
    else {
        int temp = PowR(x, y >> 1);
        return temp * temp;
    }
}

template<typename T, typename U>
T Pow(T x, U y) {
    // x^y, see y as binary.
    // e.g. y = 1010, ret = x^1000 * x^0010, each time x = x^2 y >>= 1, when (y&1) == 1, ret *= x.
    // x = a^0001, and x^2 = a^0010, x^3 = a^0100, ...
    int ret = 1;
    while (y) {
        if (y & 1) {
            ret *= x;
        }
        x *= x;
        y >>= 1;
    }
    return ret;
}

template<typename T, typename U, typename V>
T PowMod(T x, U y, V mod) {
    // see pow(int,int) and (a * b) % x == (a % x)(b % x) % x
    long ret = 1;
    while (y) {
        if (y & 1) {
            ret = ret * x % mod;
        }
        x = x * x % mod;
        y >>= 1;
    }
    return ret;
}

}
#endif //NITETRICHOR_ALGORITHM_HPP
