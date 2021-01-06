/*  Author:     Zoya Samsonov
 *  Created:    December 20, 2020
 */

#include <iostream>
#include "gate.h"
#include "util.h"

bool andgate(std::vector<bool> ops) {
    for (bool b : ops) 
        if (!b) return false;
    return true;
}

bool orgate(std::vector<bool> ops) {
    for (bool b : ops)
        if (b) return true;
    return false;
}

bool notgate(std::vector<bool> ops) {
    if (ops.size() != 1)
        customerrorquit("Invalid number of arguments to NOT");
    return !ops[0];
}

bool xorgate(std::vector<bool> ops) {
    bool out = 0;
    for (bool b : ops)
        if (b) out = !out;
    return out;
}

Gate::Gate(std::vector<Wire*> ins, bool (*func)(std::vector<bool> ops)) :
    inputs(ins),
    update_func(func)
{
    // std::cout << "G";
}

void Gate::update(long tick) {
    if (tick > last_updated) {
        last_updated = tick;
        std::vector<bool> states;
        for (Wire* in : inputs)
            states.push_back(in->getstate(tick));
        state = update_func(states);
    }
}
