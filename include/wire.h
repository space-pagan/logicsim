#ifndef WIRE_H
#define WIRE_H

class Wire {
    public:
        Wire() {}
        Wire(Wire* in, bool initstate);
        virtual void update(long tick);
        bool getstate(long tick);
        void set_in(Wire* in);

    protected:
        Wire* input = nullptr;
        long last_updated = 0;
        bool state = false;
};

class IO : public Wire {
    public:
        IO();
        IO(Wire* io);
        void write(bool state);
};

#endif
