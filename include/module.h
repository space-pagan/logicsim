#ifndef MODULE_H
#define MODULE_H

#include <string>
#include "gate.h"

class Module : public Wire {
    public:
        Module() {}
        Module(std::vector<Wire*> mustupdate,
               std::vector<Wire*> outs,
               std::string name);

        void update(long tick) override;
        
        std::vector<Wire*> outputs;
        std::vector<bool> outstates;

    protected:
        std::string name;
        std::vector<Wire*> updateable;
};

class HalfAdder : public Module {
    public:
        HalfAdder(Wire* A, Wire* B);
        HalfAdder(std::vector<Wire*> ins);
        HalfAdder();

    private:
        AndGate g_And;
        XorGate g_Xor;
};

class FullAdder : public Module {
    public:
        FullAdder(Wire* A, Wire* B, Wire* Cin);
        FullAdder(std::vector<Wire*> ins);
        FullAdder();

    private:
        HalfAdder HA1, HA2;
        OrGate g_Or;
};

class AddArray : public Module {
    public: 
        AddArray(std::vector<Wire*> ins, int adder_count);
        AddArray();
        ~AddArray();

    private:
        std::vector<FullAdder*> fa;
};

#endif
