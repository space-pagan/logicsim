/*  Author:     Zoya Samsonov
 *  Created:    January 6, 2021
 */

#include <iostream>
#include "module.h"

Module::Module(std::vector<Wire*> mustupdate, std::vector<Wire*> outs) :
    outputs(outs),
    updateable(mustupdate)
{}

void Module::update(long tick) {
    if (tick > last_updated) {
        last_updated = tick;
        outstates.clear();
        for (Wire* up : updateable) up->update(tick);
        for (Wire* out : outputs) {
            outstates.push_back(out->getstate(tick));
        }
    }
}
        
void Module::add_output(Wire* w) {
    outputs.push_back(w);
}

void Module::add_updateable(Wire* w) {
    updateable.push_back(w);
}

Module* halfadder(Wire* A, Wire* B) {
    Gate* And = new Gate({A, B}, andgate);
    Gate* Xor = new Gate({A, B}, xorgate);
    return new Module({Xor, And}, {Xor, And});
}

Module* halfadder(std::vector<Wire*> ins) {
    return halfadder(ins[0], ins[1]);
}

Module fulladder(Wire* A, Wire* B, Wire* Cin) {
    Module* AB = halfadder(A, B);
    Module* Sum = halfadder(AB->outputs[0], Cin);
    Gate* Cout = new Gate({Sum->outputs[1], AB->outputs[1]}, orgate);
    return Module({AB, Sum, Cout}, {Cout, Sum->outputs[0]});
}

Module fulladder(std::vector<Wire*> ins) {
    return fulladder(ins[0], ins[1], ins[2]);
}
