/*  Author:     Zoya Samsonov
 *  Created:    December 20, 2020
 */

#include <iostream>
#include "wire.h"
#include "gate.h"
#include "util.h"

int main(int argc, char** argv) {
    IO nc;
    IO io1; // A0
    IO io2; // B0
    IO io3; // A1
    IO io4; // B1
    IO io5; // A2
    IO io6; // B2
    IO io7; // A3
    IO io8; // B3
    IO io9, io10, io11, io12, io13; // O0-5
    
    Gate sum0 = Gate({&nc, &io1, &io2}, xorgate);
    io9.set_in(&sum0);

    Gate car0_1 = Gate({&io1, &io2}, xorgate);
    Gate car0_2 = Gate({&io1, &io2}, andgate);
    Gate car0_3 = Gate({&car0_1, &nc}, andgate);
    Gate car0_4 = Gate({&car0_2, &car0_3}, orgate);
    
    Gate sum1 = Gate({&car0_4, &io3, &io4}, xorgate);
    io10.set_in(&sum1);

    Gate car1_1 = Gate({&io3, &io4}, xorgate);
    Gate car1_2 = Gate({&io3, &io4}, andgate);
    Gate car1_3 = Gate({&car1_1, &car0_4}, andgate);
    Gate car1_4 = Gate({&car1_2, &car1_3}, orgate);

    Gate sum2 = Gate({&car1_4, &io5, &io6}, xorgate);
    io11.set_in(&sum2);

    Gate car2_1 = Gate({&io5, &io6}, xorgate);
    Gate car2_2 = Gate({&io5, &io6}, andgate);
    Gate car2_3 = Gate({&car2_1, &car1_4}, andgate);
    Gate car2_4 = Gate({&car2_2, &car2_3}, orgate);

    Gate sum3 = Gate({&car2_4, &io7, &io8}, xorgate);
    io12.set_in(&sum3);

    Gate car3_1 = Gate({&io7, &io8}, xorgate);
    Gate car3_2 = Gate({&io7, &io8}, andgate);
    Gate car3_3 = Gate({&car3_1, &car2_4}, andgate);
    Gate car3_4 = Gate({&car3_2, &car3_3}, orgate);
    io13.set_in(&car3_4);

    long tick = 1;
    for (int A : range(16)) {
        for (int B : range(16)) {
            std::vector<bool> bitsA = getbits(A, 4);
            std::vector<bool> bitsB = getbits(B, 4);
            io1.write(bitsA[3]);
            io2.write(bitsB[3]);
            io3.write(bitsA[2]);
            io4.write(bitsB[2]);
            io5.write(bitsA[1]);
            io6.write(bitsB[1]);
            io7.write(bitsA[0]);
            io8.write(bitsB[0]);
            for (int i : range(4)) std::cout << bitsA[i];
            std::cout << " + ";
            for (int i : range(4)) std::cout << bitsB[i];
            std::cout << " -> "
                      << io13.getstate(tick)
                      << io12.getstate(tick)
                      << io11.getstate(tick)
                      << io10.getstate(tick)
                      << io9.getstate(tick)
                      << "\n";
            tick++;
        }
    }
}
