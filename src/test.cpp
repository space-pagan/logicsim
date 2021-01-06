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

    IO S, R;
    Wire Q, Qnot;
    Gate or1({&R, &Qnot}, orgate);
    Gate not1({&or1}, notgate);
    Q.set_in(&not1);
    Gate or2({&Q, &S}, orgate);
    Gate not2({&or2}, notgate);
    Qnot.set_in(&not2);

    long tick = 1;
    srand(getpid());

    while (tick++) {
        S.write(rand() % 2);
        R.write(rand() % 2);
        std::cout << S.getstate(tick)
                  << R.getstate(tick)
                  << Q.getstate(tick)
                  << Qnot.getstate(tick)
                  << "\n";
        sleep(1);
    }

/*
 * 8-BIT ADDER
 *     int input_len = 16;
 *     IO io[input_len];
 *     IO nc;
 * 
 *     Module fa[input_len / 2];
 *     Module out;
 *     for (int i : range(input_len / 2)) {
 *         fa[i] = fulladder(
 *                 &io[input_len / 2 - 1 - i], 
 *                 &io[input_len - 1 - i], 
 *                 i > 0 ? fa[i-1].outputs[0] : &nc);
 *         out.add_updateable(&fa[i]);
 *         out.add_output(fa[i].outputs[1]);
 *     }
 *     out.add_output(fa[input_len / 2 - 1].outputs[0]);
 *     std::reverse(out.outputs.begin(), out.outputs.end());
 * 
 *     for (int i : range(pow(2, input_len))) {
 *         std::vector<bool> bits = getbits(i, input_len);
 *         for (int j : range(input_len)) io[j].write(bits[j]);
 *         out.update(i + 1);
 *         std::cout << bitstring(bits).substr(0, input_len / 2)
 *                   << " + "
 *                   << bitstring(bits).substr(input_len / 2, input_len / 2)
 *                   << " -> "
 *                   << bitstring(out.outstates)
 *                   << "\n";
 *     }
 */
}
