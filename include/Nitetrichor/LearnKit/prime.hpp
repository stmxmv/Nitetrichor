/*!
* Created by Molybdenum on 9/17/21.
*
*/
#ifndef NITETRICHOR_PRIME_HPP
#define NITETRICHOR_PRIME_HPP
namespace NC {



template<typename T, long Num>
struct Primes0 {
    static_assert(std::is_integral<T>::value, "Integral required.");
    T cnt = 0;
    bool isCom[Num + 1]{0};
    T primes[Num] = {0};

    constexpr explicit Primes0(T num = Num) noexcept {
        for(long i = 2; i <= num; i++) {
            // is prime
            if(!isCom[i]) {
                primes[cnt]=i;
                ++cnt;
            }
            for(long j = 0; j < cnt && i * primes[j] <= num ; j++ ) {
                // not prime
                isCom[i * primes[j]] = true;
                if(i % primes[j] == 0) {   // primes[j] is i 's prime factor
                    break;
                }
            }
        }
    }

    inline T evaluateNumOfSquareFreeInteger(T num) const {
        return num - _evaluateNumOfNonSquareFreeInteger(0, num);
    }

private:
    /// \brief evaluate num of non square free integer of [1, num]
    /// \param cur pass 0
    /// \param num
    T _evaluateNumOfNonSquareFreeInteger(int cur, T num) const {
        long ret = 0;
        for(int i = cur; i < cnt && (long)primes[i] * primes[i] <= num; ++i) {
            long temp = num/primes[i]/primes[i];
            ret += temp - _evaluateNumOfNonSquareFreeInteger(i + 1, temp);// |S| - |Ai|(1<=i<=n) + |Ai&&Aj|(1<=i<j<=n)-...
        }
        return ret;
    }
};


template<typename T, long Num>
struct Primes1 {
    static_assert(std::is_integral<T>::value, "Integral required.");
    T cnt = 0;
    bool isCom[Num + 1]{0};
    T primes[Num] = {0};
    T mu[Num + 1] = {0, 1};

    constexpr explicit Primes1(T num = Num) noexcept {
        for(long i = 2; i <= num; i++) {
            // is prime
            if(!isCom[i]) {
                primes[cnt]=i;
                ++cnt;
                mu[i] = -1;
            }
            for(long j = 0; j < cnt && i * primes[j] <= num ; j++ ) {
                // not prime
                isCom[i * primes[j]] = true;
                if(i % primes[j] == 0) {   // primes[j] is i 's prime factor
                    mu[i * primes[j]] = 0;
                    break;
                } else {
                    mu[i * primes[j]] = -mu[i];
                }
            }
        }
    }

    /// \brief evaluate num of square free integer of [1, num]
    /// \param num
    T evaluateNumOfSquareFreeInteger(T num) const {
        T ret = 0;
        for(long i = 1; i < Num && i * i <= num; ++i) {
            long temp = num / (long)(i * i);
            ret += mu[i] * temp;
        }
        return ret;
    }

};

template<typename T, long Num>
struct Primes2 {
    static_assert(std::is_integral<T>::value, "Integral required.");
    T       cnt = 0;
    T       primes[Num] = {0};
    T       phi[Num] = {0, 1};
    bool    isCom[Num + 1] = {0};

    constexpr explicit Primes2(T num = Num) noexcept {
        for(long i = 2; i <= num; i++) {
            // is prime
            if(!isCom[i]) {
                phi[i] = i - 1;
                primes[cnt]=i;
                ++cnt;
            }
            for(long j = 0; j < cnt && i * primes[j] <= num ; j++ ) {
                // not prime
                isCom[i * primes[j]] = true;
                if(i % primes[j] == 0)
                {   // primes[j] is i 's prime factor
                    phi[i * primes[j]] = phi[i] * primes[j];
                    break;
                } else {
                    // i and primes[j] are relatively primes, and Etf[primes[j]] = primes[j] - 1
                    phi[i * primes[j]] = phi[i] * (primes[j] - 1);
                }
            }
        }
    }

};

namespace __old {

#define PRIMES_MAX_NUM 100

// call this function multiple time have no effect.
int *PrimesCreate(int *outCnt, int **phip = nullptr) {
    static bool isCom[PRIMES_MAX_NUM + 1] = {false};
    static int primes[PRIMES_MAX_NUM] = {0};
    static int phi[PRIMES_MAX_NUM] = {0, 1};
    static int cnt = 0;
    if (cnt) {
        *outCnt = cnt;
        *phip = phi;
        return primes;
    }

    for (int i = 2; i <= PRIMES_MAX_NUM; i++) {
        // is prime
        if (!isCom[i]) {
            phi[i] = i - 1;
            primes[cnt] = i;
            ++cnt;
        }
        for (int j = 0; j < cnt && i * primes[j] <= PRIMES_MAX_NUM; j++) {
            // not prime
            isCom[i * primes[j]] = true;
            if (i % primes[j] == 0) {   // primes[j] is i 's prime factor
                phi[i * primes[j]] = phi[i] * primes[j];
                break;
            } else {
                // i and primes[j] are relatively primes, and Etf[primes[j]] = primes[j] - 1
                phi[i * primes[j]] = phi[i] * (primes[j] - 1);
            }
        }
    }
    *outCnt = cnt;
    *phip = phi;
    return primes;
}

}


}

#endif //NITETRICHOR_PRIME_HPP
