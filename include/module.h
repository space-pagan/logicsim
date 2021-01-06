#ifndef MODULE_H
#define MODULE_H

#include "gate.h"

class Module : public Wire {
    public:
        Module() {}
        Module(std::vector<Wire*> mustupdate, std::vector<Wire*> outs);
        void update(long tick) override;
        void add_output(Wire* w);
        void add_updateable(Wire* w);
        
        std::vector<Wire*> outputs;
        std::vector<bool> outstates;

    protected:
        std::vector<Wire*> updateable;
};

Module* halfadder(Wire* A, Wire* B);
Module* halfadder(std::vector<Wire*> ins);
Module fulladder(Wire* A, Wire* B, Wire* Cin);
Module fulladder(std::vector<Wire*> ins);
#endif
