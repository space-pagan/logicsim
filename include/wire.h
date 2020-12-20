#ifndef WIRE_H
#define WIRE_H

class Wire {
    public:
        Wire() {}
        Wire(Wire* in, Wire* out, bool initstate);
        virtual void update(long tick);
        bool getstate();
        bool getstate_update(long tick);
        void update_cons(Wire* in, Wire* out);

    protected:
        Wire* input = nullptr;
        Wire* output = nullptr;
        long last_updated = 0;
        bool state = false;
};

class IO : public Wire {
    public:
        IO() {}
        IO(Wire* io);
        void write(bool state);
};

#endif
