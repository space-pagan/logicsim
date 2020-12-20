/*  Author:     Zoya Samsonov
 *  Created:    December 20, 2020
 */

#include <iostream>
#include "gate.h"
#include "util.h"

Gate::Gate(int numin, Wire** ins, Wire* out, 
        bool (*func)(int opsize, bool* ops)) :
    numinputs(numin),
    update_func(func)
{
    this->output = out;
    this->instates = new bool[numin];
    this->inputs = new Wire*[numin];
    for (int i : range(numin))
        this->inputs[i] = ins[i];
}

Gate::~Gate() {
    delete this->instates;
    delete this->inputs;
}

void Gate::update(long tick) {
    if (tick > this->last_updated) {
        this->last_updated = tick;
        for (int i : range(this->numinputs)) {
            this->inputs[i]->update(tick);
            this->instates[i] = this->inputs[i]->getstate();
        }
        this->state = this->update_func(this->numinputs, this->instates);
    }
}
