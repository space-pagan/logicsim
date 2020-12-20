#ifndef UTIL_H
#define UTIL_H

#include <string>

struct range {
    int* data;
    int* ptr;
    int size;
    range(int T);
    range(int S, int T);
    range(int S, int T, int U);
    const int* begin();
    const int* end();
};

void customerrorquit(std::string s);

#endif
