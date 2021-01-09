/*  Author:     Zoya Samsonov
 *  Created:    December 20, 2020
 */

#include <iostream>
#include <cmath>
#include <algorithm>
#include <unistd.h>
#include "wire.h"
#include "gate.h"
#include "module.h"
#include "util.h"

int main(int argc, char** argv) {
    int input_len = 8;
    IO io[input_len + 1];

    std::vector<Wire*> ins;
    for (int i : range(input_len + 1)) ins.push_back(&io[i]);
    AddArray aa = AddArray(ins, input_len / 2);

    for (int i : range(pow(2, input_len))) {
        std::vector<bool> bits = getbits(i, input_len);
        for (int j : range(input_len)) io[j].write(bits[j]);
        aa.update(i + 1);
        std::cout << bitstring(bits).substr(0, input_len / 2)
                  << " + "
                  << bitstring(bits).substr(input_len / 2, input_len / 2)
                  << " -> "
                  << bitstring(aa.outstates)
                  << "\n";
    }
}
