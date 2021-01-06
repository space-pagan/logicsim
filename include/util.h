#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

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
std::vector<bool> getbits(int number, int bitlen);
std::string bitstring(std::vector<bool> bits);

#endif
