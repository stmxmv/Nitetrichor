/*!
* Created by Molybdenum on 9/10/21.
*
*/
#include <unistd.h>
#include "LearnKit/Redirect.h"


namespace WN {


Learn::Redirect::Redirect() : fin("OJ/in.txt"),
                              fout("OJ/out.txt", std::ofstream::trunc),
                              in(std::cin.rdbuf()),
                              out(std::cout.rdbuf()) {
}

Learn::Redirect &Learn::Redirect::SharedRedirect() {
    static Redirect redirect;
    return redirect;
}

void Learn::Redirect::recover() {
    std::cin.rdbuf(in);
    std::cout.rdbuf(out);
}

void Learn::Redirect::redirect() {
    std::cin.rdbuf(fin.rdbuf());
    std::cout.rdbuf(fout.rdbuf());
}
}


static FILE * in = stdin;
static FILE * out = stdout;
static const char * rin = "OJ/in.txt";
static const char * rout = "OJ/out.txt";

void WNRedirectIO(FILE * file) {
    if (file == stdin) {
        in = fopen(rin, "r");
        if (in == nullptr) {
            perror("freopen stdin to in.txt error :");
        }
    } else if (file == stdout) {
        out = fopen(rout, "w");
        if (out == nullptr) {
            perror("freopen stdin to out.txt error :");
        }
    }
}

void WNScanf(const char * format, ...) {
    va_list args;
    va_start(args, format);
    WNScanfv(format, args);
    va_end(args);
}

void WNScanfv(const char * format, va_list args) {
    vfscanf(in, format, args);
}

int WNGetchar(void) {
    return fgetc(in);
}

char * WNGets(char * str, int n) {
    return fgets(str, n, in);
}

void WNPrintf(const char * format, ...) {
    va_list args;
    va_start(args, format);
    WNPrintfv(format, args);
    va_end(args);
}
void WNPrintfv(const char * format, va_list args) {
    vfprintf(out, format, args);
}

void WNPuts(const char * s) {
    fputs(s, out);
}

void WNRedirect(void) {
    in = fopen(rin, "r");
    if (in == nullptr) {
        perror("freopen stdin to in.txt error :");
    }
    out = fopen(rout, "w");
    if (out == nullptr) {
        perror("freopen stdin to out.txt error :");
    }
}

void WNRedirectRecoverIO(FILE * file) {
    if (file == stdin) {
        if (in != stdin) {
            fclose(in);
            in = stdin;
        }
    } else if (file == stdout) {
        if (out != stdout) {
            fclose(out);
            out = stdout;
        }
    }
}

void WNRedirectRecover(void) {
    if (in != stdin) {
        fclose(in);
        in = stdin;
    }
    if (out != stdout) {
        fclose(out);
        out = stdout;
    }
}

void WNRedirectSet(FILE * io, const char * file) {
    if (io == stdin) {
        rin = file;
    } else if (io == stdout) {
        rout = file;
    }
}
