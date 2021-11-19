// -*- Nitetrichor -*-
//===--------------------------- `target name` ---------------------------------===//
//
// fast_stdio.hpp
// 
// Created by Molybdenum on 10/26/21.
// Copyright (c) 2021 Nitetrichor. All rights reserved.
//===----------------------------------------------------------------------===//
#ifndef NITETRICHOR_FAST_STDIO_HPP
#define NITETRICHOR_FAST_STDIO_HPP


#define DEBUG_PRINT(container) \
    {bool first = true;\
    for (const auto &i : container) { \
        if (first) {           \
            cout << i;         \
            first = false;     \
        } else {               \
            cout << ' ' << i;\
        }\
    }}             \
    cout << endl

#define DEBUG_PRINT_N(container, n) \
    for (int i = 0; i < (int)(n); ++i) { \
        if (i == 0) {               \
            cout << container[0];\
        } else {                    \
            cout << ' ' << container[i];\
        }\
    }\
    cout << endl


namespace NC {
class std_IO {
    static constexpr int buf_size = 1 << 21;
    bool eof:1;
    char *p1, *p2;
    int top;
    char buf0[buf_size], buf1[buf_size];
    int temp[105];

    std_IO() noexcept = default;

    ~std_IO() noexcept { clear_buf1(); }

    inline void clear_buf1() noexcept {
        fwrite(buf1, sizeof(char), top, stdout);
        top = 0;
    }

    inline void pc(const char c) noexcept {
        if (top == buf_size) clear_buf1();
        buf1[top++] = c;
    }

    inline char gc() noexcept {
        return p1 == p2 && (p2 = (p1 = buf0) + fread(buf0, sizeof(char), buf_size, stdin), p1 == p2) ? (char)EOF : *p1++;
    }

    inline void ungc(char x) noexcept {
        if (x != EOF) { --p1; }
    }

    inline char peek() noexcept {
        return p1 == p2 && (p2 = (p1 = buf0) + fread(buf0, 1, buf_size, stdin), p1 == p2) ? (char) EOF : *p1;
    }

public:

    static std_IO& GetIO() noexcept {
        static std_IO io;
        return io;
    }

    inline std_IO &operator>>(char &x) noexcept {
        while (x = gc(), x == ' ' || x == '\n' || x == '\r');
        if (x == EOF) { eof = true; }
        return *this;
    }

    inline std_IO &operator>>(std::string& str) noexcept {
        char x;
        str.clear();
        while (x = gc(), x == ' ' || x == '\n' || x == '\r');
        if (x == EOF) { eof = true; return *this; }
        do str.push_back(x);
        while ((x = gc()) != EOF && x != ' ' && x != '\n' && x != '\r');
        ungc(x);
        return *this;
    }

    inline std_IO &operator>>(char *str) noexcept {
        char x;
        while (x = gc(), x == ' ' || x == '\n' || x == '\r');
        if (x == EOF) { eof = true; return *this; }
        do *str++ = x;
        while ((x = gc()) != EOF && x != ' ' && x != '\n' && x != '\r');
        *str = 0;
        ungc(x);
        return *this;
    }

    inline std_IO &getline(char * str, int count) noexcept {
        char x = gc();
        if (x == EOF) { eof = true; return *this; }
        while (x != '\n' && x != '\r' && x != EOF && --count) {
            *str++ = x;
            x = gc();
        }
        if (x == '\r') { ignore(); }
        *str = 0;
        return *this;
    }

    template<typename T>
    inline std_IO &operator>>(T &x) noexcept  {
        x = 0;
        bool f = false;
        char ch;
        while (ch = gc(), ch == ' ' || ch == '\n' || ch == '\r');
        if (ch == EOF) { eof = true; return *this; }
        while (ch < '0' || ch > '9') {
            if (ch == '-') f ^= 1;
            ch = gc();
        }
        while (ch >= '0' && ch <= '9') x = (x << 3) + (x << 1) + ch - '0', ch = gc();
        ungc(ch);
        f ? x = -x : 0;
        return *this;
    }

    inline void ignore() noexcept { gc(); }

    template<typename T>
    inline std_IO &operator <<(T x) noexcept {
        static_assert(std::is_integral<T>::value, "integral require");
        if (x < 0) pc('-'), x = -x;
        do { temp[++temp[0]] = x % 10, x /= 10; } while (x);
        while (temp[0]) pc((char) ('0' + temp[temp[0]--]));
        return *this;
    }

    inline std_IO &operator << (char x) noexcept {
        pc(x);
        return *this;
    }

    inline std_IO &operator <<(const char *x) noexcept {
        int len = (int)strlen(x);
        if (top + len >= buf_size) { clear_buf1(); }
        strcpy(buf1 + top, x);
        top += len;
        return *this;
    }

    inline std_IO &operator <<(const std::string &string) noexcept {
        *this << string.c_str();
        return *this;
    }

    explicit operator bool() const noexcept {
        return !eof;
    }

    friend std_IO& getline(std_IO& input, std::string &str) noexcept;
};

std_IO& getline(std_IO& input, std::string &str) noexcept {
    str.clear();
    char x = input.gc();
    if (x == EOF) { input.eof = true; return input; }
    while (x != '\n' && x != '\r' && x != EOF) {
        str.push_back(x);
        x = input.gc();
    }
    if (x == '\r') { input.ignore(); }
    return input;
}


// global
std_IO& cin = std_IO::GetIO();
std_IO& cout = std_IO::GetIO();
[[maybe_unused]] constexpr char endl = '\n';



}




#endif //NITETRICHOR_FAST_STDIO_HPP
