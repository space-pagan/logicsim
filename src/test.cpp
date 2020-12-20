/*  Author:     Zoya Samsonov
 *  Created:    December 20, 2020
 */

#include <iostream>
#include <string>
#include "wire.h"
#include "gate.h"
#include "util.h"

bool andfunc(int opsize, bool* ops) {
    bool out = 1;
    for (int i : range(opsize))
        out &= ops[i];
    return out;
}

bool orfunc(int opsize, bool* ops) {
    bool out = 0;
    for (int i : range(opsize))
        out |= ops[i];
    return out;
}

int main(int argc, char** argv) {
    Wire w1;
    IO io1;
    IO io2;
    IO io3;
    IO io4;
    
    Wire* array1[2] = { &io1, &io2 };
    Gate g1 = Gate(2, array1, &w1, andfunc);
    Wire* array2[2] = { &w1, &io3 };
    Gate g2 = Gate(2, array2, &io4, orfunc);
    
    w1.update_cons(&g1, &g2);
    io1.update_cons(&io1, &g1);
    io2.update_cons(&io2, &g1);
    io3.update_cons(&io3, &g2);
    io4.update_cons(&g2, &g2);
    
    long tick = 1;
    for (int i : range(2)) {
        for (int j : range(2)) {
            for (int k : range(2)) {
                io1.write(i);
                io2.write(j);
                io3.write(k);
                std::cout << i << j << k << "->";
                std::cout << io4.getstate_update(tick++) << "\n";
            }
        }
    }
}
