/*  Author:     Zoya Samsonov
 *  Created:    December 20, 2020
 */

#include <iostream>
#include "wire.h"

Wire::Wire(Wire* in, bool initstate) :
    input(in),
    state(initstate)
{}

void Wire::update(long tick) {
    if (tick > last_updated) {
        last_updated = tick;
        state = input->getstate(tick);
    }
}

bool Wire::getstate(long tick) {
    update(tick);
    return state;
}

void Wire::set_in(Wire* in) {
    input = in;
}

IO::IO() :
    Wire(this, false)
{}

IO::IO(Wire* io) : 
    Wire(io, false)
{}

void IO::write(bool state) {
    this->state = state;
}
