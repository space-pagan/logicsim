/*  Author:     Zoya Samsonov
 *  Created:    January 6, 2021
 */

#include <iostream>
#include <algorithm>
#include "module.h"
#include "util.h"

Module::Module(std::vector<Wire*> mustupdate, 
               std::vector<Wire*> outs,
               std::string name) :
    outputs(outs),
    name(name),
    updateable(mustupdate)
{}

void Module::update(long tick) {
    if (tick > last_updated) {
        // std::cout << "Updating " << name << " @ " << this << ": ";
        last_updated = tick;
        outstates.clear();
        for (Wire* up : updateable) {
            // std::cout << updateable.size() << " deps\n";
            up->update(tick);
        }
        for (Wire* out : outputs) {
            outstates.push_back(out->getstate(tick));
        }
    }
}
        
HalfAdder::HalfAdder(Wire* A, Wire* B) :
    g_And({A, B}),
    g_Xor({A, B})
{
    outputs = {&g_And, &g_Xor}; // Cout, Sum
    updateable = {&g_And, &g_Xor};
    name = "halfadder";
}

HalfAdder::HalfAdder(std::vector<Wire*> ins) :
    HalfAdder(ins[0], ins[1])
{}

HalfAdder::HalfAdder() :
    HalfAdder(nullptr, nullptr)
{}

FullAdder::FullAdder(Wire* A, Wire* B, Wire* Cin) :
    HA1(A, B),
    HA2(HA1.outputs[1], Cin),
    g_Or({HA1.outputs[0], HA2.outputs[0]})
{
    outputs = {&g_Or, HA2.outputs[1]};
    updateable = {&HA1, &HA2, &g_Or};
    name = "fulladder";
}

FullAdder::FullAdder(std::vector<Wire*> ins) :
    FullAdder(ins[0], ins[1], ins[2])
{}

FullAdder::FullAdder() :
    FullAdder(nullptr, nullptr, nullptr)
{}
        
AddArray::AddArray(std::vector<Wire*> ins, int adder_count) {
    for (int i : range(adder_count)) {
        if (i > 0) {
            fa.push_back(new FullAdder(
                    ins[adder_count - i - 1], ins[2*adder_count - i - 1], fa[i-1]->outputs[0]));
        } else {
            fa.push_back(new FullAdder(
                    ins[adder_count - i - 1], ins[2*adder_count - i - 1], ins[2*adder_count]));
        }
        updateable.push_back(fa[i]);
        outputs.push_back(fa[i]->outputs[1]);
    }
    outputs.push_back(fa[adder_count - 1]->outputs[0]);
    std::reverse(outputs.begin(), outputs.end());
    name = "adder x" + std::to_string(adder_count);
}

AddArray::~AddArray() {
    for (FullAdder* faptr : fa) delete faptr;
}
