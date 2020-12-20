#ifndef GATE_H
#define GATE_H

#include "wire.h"

class Gate : public Wire {
    public:
        Gate() {}
        Gate(int numin, Wire** ins, Wire* out, 
                bool (*func)(int opsize, bool* ops));
        ~Gate();
        void update(long tick) override;

    protected:
        int numinputs;
        Wire** inputs;
        bool* instates;
        bool (*update_func)(int opsize, bool* ops);
};

#endif
