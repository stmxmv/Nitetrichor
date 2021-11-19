/*!
* Created by Molybdenum on 9/2/21.
*
*/
#ifndef NITETRICHOR_REDIRECT_H
#define NITETRICHOR_REDIRECT_H

#ifndef __cplusplus
#include <stdio.h>
#include <stddef.h>

#endif

#ifdef __cplusplus
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>

namespace WN::Learn {

class Redirect {
    std::ifstream fin;
    std::ofstream fout;
    std::streambuf *in;
    std::streambuf *out;

    Redirect();
public:
    Redirect(const Redirect &) = delete;

    Redirect &operator=(const Redirect &) = delete;

    static Redirect &SharedRedirect();

    void recover();

    void redirect();
};
}

extern "C" {
#endif //__cplusplus

#if (__GNUC__*10+__GNUC_MINOR__ >= 42)
#define WN_PRINTF_LIKE(fmtarg, firstvararg) \
	        __attribute__((__format__ (__printf__, fmtarg, firstvararg)))
#define WN_SCANF_LIKE(fmtarg, firstvararg) \
	        __attribute__((__format__ (__scanf__, fmtarg, firstvararg)))
#else
#define WN_PRINTF_LIKE(fmtarg, firstvararg)
#define WN_SCANF_LIKE(fmtarg, firstvararg)
#endif

#define printf(...) WNPrintf(__VA_ARGS__)
#define puts(s) WNPuts(s)
#define scanf(...) WNScanf(__VA_ARGS__)
#define getchar() WNGetchar()
#define WN_GETS(str, n) WNGets(str, n)
void WNPuts(const char * s);
void WNPrintf(const char * format, ...) WN_PRINTF_LIKE(1, 2);
void WNPrintfv(const char * format, va_list args) WN_PRINTF_LIKE(1, 0);
void WNScanf(const char * format, ...) WN_SCANF_LIKE(1, 2);
void WNScanfv(const char * format, va_list args) WN_SCANF_LIKE(1, 0);
int  WNGetchar(void);
char * WNGets(char * str, int n);
void WNRedirectIO(FILE * file);
void WNRedirectRecoverIO(FILE * file);
void WNRedirect(void);
void WNRedirectRecover(void);
void WNRedirectSet(FILE * io, const char * file);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //NITETRICHOR_REDIRECT_H
