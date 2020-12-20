/*  Author:     Zoya Samsonov
 *  Created:    December 20, 2020
 */

#include <iostream>
#include "wire.h"

Wire::Wire(Wire* in, Wire* out, bool initstate) :
    input(in),
    output(out),
    state(initstate)
{}

void Wire::update(long tick) {
    if (tick > this->last_updated) {
        this->last_updated = tick;
        this->input->update(tick);
        this->state = this->input->getstate();
    }
}

bool Wire::getstate() {
    return this->state;
}

bool Wire::getstate_update(long tick) {
    this->update(tick);
    return this->getstate();
}

void Wire::update_cons(Wire* in, Wire* out) {
    this->input = in;
    this->output = out;
}

IO::IO(Wire* io) {
    this->input = io;
    this->output = io;
    this->state = false;
}

void IO::write(bool state) {
    this->state = state;
}
