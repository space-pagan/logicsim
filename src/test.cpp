/*  Author:     Zoya Samsonov
 *  Created:    December 20, 2020
 */

#include <iostream>
#include "wire.h"
#include "gate.h"
#include "util.h"

int main(int argc, char** argv) {
    IO io1;
    IO io2;
    IO io3;
    IO io4;
    IO io5;
    
    Gate sum = Gate({&io1, &io2, &io3}, xorgate);
    io4.set_in(&sum);

    Gate carry1 = Gate({&io1, &io2}, xorgate);
    Gate carry2 = Gate({&io1, &io2}, andgate);
    Gate carry3 = Gate({&carry1, &io3}, andgate);
    Gate carry4 = Gate({&carry2, &carry3}, orgate);
    io5.set_in(&carry4);
    
    long tick = 1;
    for (bool i : {0, 1}) {
        for (bool j : {0, 1}) {
            for (bool k : {0, 1}) {
                io1.write(i);
                io2.write(j);
                io3.write(k);
                std::cout << i << j << k << "->";
                std::cout << io4.getstate(tick++) << ", ";
                std::cout << io5.getstate(tick++) << "\n";
            }
        }
    }
}
