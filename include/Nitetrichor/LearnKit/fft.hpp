/*!
* Created by Molybdenum on 9/17/21.
*
*/
#ifndef NITETRICHOR_FFT_HPP
#define NITETRICHOR_FFT_HPP


namespace NC {

#define NC_PI 3.141592653589793238462643383279502884197

const int *_get_rev(int n) {
#define MAX_NUM 4000009
    static int rev[MAX_NUM] = {0};
    static int last = 0;
    if (last == n) {
        return rev;
    }
    last = n;
    int bit = -1, temp = n;
    while (temp >>= 1) ++bit;
    for (int i = 0; i < n; ++i) {
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << bit);
    }
    return rev;
#undef MAX_NUM
}

using Complex = std::complex<double>;

void _fft(std::vector<Complex> &vec, int type) {
    int n = (int) vec.size();
    auto rev = _get_rev(n);
    for (int i = 0; i < n; ++i) {
        auto r = rev[i];
        if (i < r) {
            std::swap(vec[i], vec[r]);
        }
    }
    // n time recursion, start at 1 element.
    for (int i = 1; i < n; i <<= 1) {
        Complex w(std::cos(NC_PI / i), std::sin(NC_PI / i) * type); // i = 2n
        for (int j = 0; j < n; j += i << 1) {
            Complex k(1, 0);
            for (int l = j; l < j + i; ++l) {
                Complex pe = vec[l];
                Complex po = k * vec[l + i];
                vec[l] = pe + po;
                vec[l + i] = pe - po;
                k *= w;
            }
        }
    }
    // i_fft
    if (type == -1) {
        for (auto &item: vec) {
            item = item / (double) n + .5;
        }
    }
}

}

#endif //NITETRICHOR_FFT_HPP
