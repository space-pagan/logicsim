#ifndef GATE_H
#define GATE_H

#include <vector>
#include "wire.h"

bool andgate(std::vector<bool> ops);
bool orgate(std::vector<bool> ops);
bool notgate(std::vector<bool> ops);
bool xorgate(std::vector<bool> ops);

class Gate : public Wire {
    public:
        Gate() {}
        Gate(std::vector<Wire*> ins, bool (*func)(std::vector<bool> ops));
        void update(long tick) override;

    protected:
        std::vector<Wire*> inputs;
        bool (*update_func)(std::vector<bool> ops);
};

#endif
