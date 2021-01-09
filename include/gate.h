#ifndef GATE_H
#define GATE_H

#include <vector>
#include "wire.h"

bool andgate(std::vector<bool> ops);
bool orgate(std::vector<bool> ops);
bool notgate(std::vector<bool> ops);
bool nandgate(std::vector<bool> ops);
bool norgate(std::vector<bool> ops);
bool xorgate(std::vector<bool> ops);
bool xnorgate(std::vector<bool> ops);

class Gate : public Wire {
    public:
        Gate() {}
        Gate(std::vector<Wire*> ins, bool (*func)(std::vector<bool> ops));
        void update(long tick) override;

    protected:
        std::vector<Wire*> inputs;
        bool (*update_func)(std::vector<bool> ops);
};

class AndGate : public Gate {
    public:
        AndGate(std::vector<Wire*> ins) : Gate(ins, andgate) {}
};

class OrGate : public Gate {
    public:
        OrGate(std::vector<Wire*> ins) : Gate(ins, orgate) {}
};

class NotGate : public Gate {
    public:
        NotGate(std::vector<Wire*> ins) : Gate(ins, notgate) {}
};

class NandGate : public Gate {
    public:
        NandGate(std::vector<Wire*> ins) : Gate(ins, nandgate) {}
};

class NorGate : public Gate {
    public:
        NorGate(std::vector<Wire*> ins) : Gate(ins, norgate) {}
};

class XorGate : public Gate {
    public:
        XorGate(std::vector<Wire*> ins) : Gate(ins, xorgate) {}
};

class XnorGate : public Gate {
    public:
        XnorGate(std::vector<Wire*> ins) : Gate(ins, xnorgate) {}
};

#endif
