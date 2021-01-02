/* Author:      Zoya Samsonov
 * Created:     November 17, 2020
 * Last Edit:   November 17, 2020
 */

#include <iostream>
#include <cmath>
#include <algorithm>
#include "util.h"       //self func declarations

void customerrorquit(std::string s) {
    std::cerr << s << "\n";
    exit(-1);
}

range::range(int S, int T, int U) {
    // constructor for range, throws errors if invalid values are used
    if (T == S) customerrorquit("Integer range (" + 
        std::to_string(S) + ":" + std::to_string(T) + ":" +
        std::to_string(U) + ") invalid, Stop cannot equal Start");
    if (T <= S && U > 0) customerrorquit("Integer range (" + 
        std::to_string(S) + ":" + std::to_string(T) + ":" +
        std::to_string(U) + ") invalid, step size non-negative");
    if (T <= S && U > 0) customerrorquit("Integer range (" + 
        std::to_string(S) + ":" + std::to_string(T) + ":" +
        std::to_string(U) + ") invalid, step size non-negative");
    if (T > S && U < 0) customerrorquit("Integer range (" + 
        std::to_string(S) + ":" + std::to_string(T) + ":" +
        std::to_string(U) + ") invalid, step size negative");
    if (U == 0) customerrorquit("Integer range (" + 
        std::to_string(S) + ":" + std::to_string(T) + ":" +
        std::to_string(U) + ") invalid, step size 0");
    size = std::ceil((float)(T-S)/(float)U);
    data = new int[size];
    int i = 0;
    while (i < size) {
        data[i] = S + i*U;
        i++;
    }
    ptr = data;
}

// wraps range constructor to be able to take only one value
range::range(int T) : range(0, T, 1) {} 

// wraps range constructor to be able to take only two values
range::range(int S, int T) : range(S, T, 1) {}

// implements begin state for range iterator
const int* range::begin() {
    return ptr;
}

// implements end condition for range iterator
const int* range::end() {
    return &(this->data[this->size]);
}

std::vector<bool> getbits(int number, int bitlen) {
    std::vector<bool> out;
    for (int i : range(bitlen)) {
        out.push_back( (number & (1 << i)) >> i );
    }
    std::reverse(std::begin(out), std::end(out));
    return out;
}
