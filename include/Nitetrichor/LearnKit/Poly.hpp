/*!
* Created by Molybdenum on 9/17/21.
*
*/
#ifndef NITETRICHOR_POLY_HPP
#define NITETRICHOR_POLY_HPP
#include "LearnKit/fft.hpp"
#include "LearnKit/type.hpp"
#include <stack>
namespace NC {

namespace Private {
inline int Prior(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        case '(':
            return 0;
        default:
            return -1;
    }
}
}

void GetReverseExpression(const char * exp, char * dst, char differ = ' ') {
    std::stack<char> stack;
    stack.push('@');
    int j = 0;
    for (int i = 0; exp[i]; ++i) {
        if (IsDigit(exp[i])) {
            dst[j++] = exp[i];
        } else {
            if (j != 0 && IsDigit(dst[j - 1])) {
                dst[j++] = differ;
            }
            if (exp[i] == ')') {
                while (stack.top() != '(') {
                    dst[j++] = stack.top();
                    stack.pop();
                }
                stack.pop();
            } else if (exp[i] == '(') {
                stack.push('(');
            } else {
                while (Private::Prior(exp[i]) <= Private::Prior(stack.top())) {
                    dst[j++] = stack.top();
                    stack.pop();
                }
                stack.push(exp[i]);
            }
        }
    }
    if (IsDigit(dst[j - 1])) {
        dst[j++] = differ;
    }
    while (stack.top() != '@') {
        dst[j++] = stack.top();
        stack.pop();
    }
    dst[j] = 0;
}

int EvaluateReverseExpression(const char * exp) {
    std::stack<int> stack;
    int num = 0;
    for (; *exp; ++exp) {
        int a, b;
        switch (*exp) {
            case ' ':
                stack.push(num);
                num = 0;
                break;
#define CASE(c, op) case c: a = stack.top(); stack.pop(); b = stack.top(); stack.pop(); stack.push(b op a); break
            CASE('+', +);
            CASE('-', -);
            CASE('*', *);
            CASE('/', /);
#undef CASE
            case '^':
                a = stack.top(); stack.pop(); b = stack.top(); stack.pop(); stack.push((int)pow(b, a));
                break;

            default:
                num = num * 10 + (*exp - '0');
                break;
        }
    }
    return stack.top();
}


template<typename T>
class Poly {
    static_assert(std::is_floating_point<T>::value, "float type require!!");
    struct PolyNode {
        T coef;
        int expn;
        PolyNode(T coef, int expn) : coef(coef), expn(expn){}

        bool operator < (const PolyNode& node) const {
            if (expn == node.expn) {
                return coef < node.coef;
            }
            return expn < node.expn;
        }
    };


    std::list<PolyNode> list;

    auto _before_fft(const Poly &other) const {
        std::vector<Complex> vec1, vec2;
        int cnt = 0;
        for (auto iter = list.begin(); iter != list.end(); ++cnt) {
            if (iter->expn != cnt) {
                vec1.emplace_back(0, 0);
            } else {
                vec1.emplace_back(iter->coef, 0);
                ++iter;
            }
        }

        cnt = 0;
        for (auto iter = other.list.begin(); iter != other.list.end(); ++cnt) {
            if (iter->expn != cnt) {
                vec2.emplace_back(0, 0);
            } else {
                vec2.emplace_back(iter->coef, 0);
                ++iter;
            }
        }

        int size = (int)(vec1.size() + vec2.size());
        int length = 1;
        while (length < size) {
            length <<= 1;
        }
        vec1.resize(length);
        vec2.resize(length);
        return std::make_pair(std::move(vec1), std::move(vec2));
    }

public:

    Poly() = default;

    void append(T coef, int expn) {
        PolyNode node(coef, expn);
        list.insert(std::lower_bound(list.begin(), list.end(), node), node);
    }

    /// \param poly will become empty
    void add(Poly& poly) {
        list.merge(poly.list);
        auto iter = ++list.begin();
        while (iter != list.end()) {
            auto pre = std::prev(iter);
            if (pre->expn == iter->expn) {
                pre->coef += iter->coef;
                iter = list.erase(iter);
            } else {
                ++iter;
            }
        }
    }

    /// \param poly will become empty
    void sub(Poly& poly) {
        for (auto &item : poly.list) {
            item.coef = -item.coef;
        }
        add(poly);
    }

//    Poly mul(const Poly& poly) {
//        Poly ret;
//        for (auto &item1 : list) {
//            for (auto &item2 : poly.list) {
//                auto coef = item1.coef * item2.coef;
//                auto expn = item1.expn + item2.expn;
//                ret.append(coef, expn);
//            }
//        }
//        return ret;
//    }

    Poly mul(const Poly& poly) const {
        Poly ret;
        auto pair = _before_fft(poly);

        _fft(pair.first, 1);
        _fft(pair.second, 1);

        for (int i = 0; i < (int)pair.first.size(); ++i) {
            pair.first[i] *= pair.second[i];
        }

        _fft(pair.first, -1);

        auto t = list.size() + poly.list.size() - 1;

        for (int i = 0; i < (int)t; ++i) {
            if (pair.first[i].real() != 0) {
                ret.append(pair.first[i].real(), i);
            }
        }

        return ret;
    }





    friend std::ostream &operator<<(std::ostream &os, const Poly &poly) {
        for (auto iter = poly.list.begin(); iter != poly.list.end(); ++iter) {
            if (iter == poly.list.begin()) {
                os << (int)iter->coef;
            } else {
                os << ' ' << (int)iter->coef;
            }
        }
//        bool first = true;
//        for (auto item : poly.list) {
//            if (item.coef == 0) { continue; }
//
//            if (first) {
//                first = false;
//            } else {
//                os << " + ";
//            }
//
//            if (item.coef < 0) {
//                os << "(" << item.coef << ')';
//            } else {
//                os << item.coef;
//            }
//
//            if (item.expn == 0) { continue; }
//
//            os << "x^";
//            if (item.expn < 0) {
//                os << "(" << item.expn << ')';
//            } else {
//                os << item.expn;
//            }
//        }
        return os;
    }




};









}

#endif //NITETRICHOR_POLY_HPP
