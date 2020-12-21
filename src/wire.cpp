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
    if (tick > this->last_updated) {
        this->last_updated = tick;
        this->state = this->input->getstate(tick);
    }
}

bool Wire::getstate(long tick) {
    this->update(tick);
    return this->state;
}

void Wire::set_in(Wire* in) {
    this->input = in;
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
