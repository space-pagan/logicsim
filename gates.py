from gateutils import *

class gate: # Generic class for gate simulation
    def __init__(self, dependents, starting_state, outputfn):
        self.dependents = dependents
        self.setstate(starting_state)
        self.outputfn = outputfn
        self.testcases = max([2, 2**len(self.dependents)])

    def update(self, touched_list=[]):
        if self not in touched_list:
            touched_list.append(self)
            self.setstate(self.outputfn(self.dependents, self.state, touched_list))
            return self.state
        return self.state

    def setstate(self, state):
        self.state = bool(state)

    def __repr__(self):
        self.update([])
        if self.state:
            return "1"
        return "0"

    def verbose(self):
        iolist = self.getbasicio()
        return "(" + format(self.state, 'b') + ")" + str(iolist) + " |-> " + str(self)

    def getbasicio(self, touched_list=[]):
        if self not in touched_list:
            touched_list.append(self)
            output = []
            for i in self.dependents:
                if isinstance(i, basicio) and i not in output:
                    output.append(i)
                else:
                    addlist = i.getbasicio(touched_list)
                    for j in addlist:
                        if j not in output:
                            output.append(j)
            return output
        return []

    def selftest(self):
        iolist = self.getbasicio()
        for i in range(0, 2**len(iolist)):
            instring = bin(i)[2:].zfill(len(iolist))
            for j in range(0, len(instring)):
                iolist[j].setstate(int(instring[j]))
            print(iolist, "|->", self)

class basicio(gate): #simple I/O element
    def __init__(self, state=0):
        super().__init__([], state, selffn)

class andgate(gate): # many input AND
    def __init__(self, dependents):
        if len(dependents) < 2:
            raise NotImplementedError
        super().__init__(dependents, 0, andfn)

class orgate(gate): # many input OR
    def __init__(self, dependents):
        if len(dependents) < 2:
            raise NotImplementedError
        super().__init__(dependents, 0, orfn)

class notgate(gate): # 1 input inverter
    def __init__(self, dependents):
        if len(dependents) != 1:
            raise NotImplementedError
        super().__init__(dependents, 0, notfn)

def nandgate(dependents): # many input NAND
    return notgate([andgate(dependents)])

def norgate(dependents): # many input NOR
    return notgate([orgate(dependents)])

def xorgate(dependents): # 2 input XOR
    if len(dependents) != 2:
        raise NotImplementedError
    return orgate([andgate([notgate([dependents[0]]), dependents[1]]), andgate([dependents[0], notgate([dependents[1]])])])

def xnorgate(dependents): # 2 input XNOR
    if len(dependents) != 2:
        raise NotImplementedError
    return notgate([xorgate(dependents)])

class srlatch(gate): # basic S/R Latch
    def __init__(self, dependents, state):
        if len(dependents) != 2:
            raise NotImplementedError
        super().__init__(dependents, state, srfn)

    def selftest(self):
        iolist = self.getbasicio()
        for i in range(0, 2**(len(iolist)+1)):
            instring = bin(i)[2:].zfill(len(iolist)+1)
            self.setstate(int(instring[0]))
            for j in range(1, len(instring)):
                iolist[j-1].setstate(int(instring[j]))
            print("(" + instring[0] + ")" + str(iolist), "|->", self)

def esrlatch(dependents, state): #Enable, S, R
    if len(dependents) != 3:
        raise NotImplementedError
    return srlatch([andgate([dependents[0], dependents[1]]), andgate([dependents[0], dependents[2]])], state)

def dlatch(dependents, state): #Enable, Data
    if len(dependents) != 2:
        raise NotImplementedError
    return srlatch([andgate([dependents[0], dependents[1]]), andgate([dependents[0], notgate([dependents[1]])])], state)

def register(LD, DATA, CLK, state): #1 bit register, Load, Data, Clock, state
    reg = dlatch([CLK, orgate([andgate([basicio(), notgate([LD])]), andgate([LD, DATA])])], 0)
    reg.dependents[0].dependents[1].dependents[0].dependents[0] = reg
    return reg

def divider(CLK): # ~Q -> D. Chained dividers create a binary counter
    reg = dlatch([pulsegate([notgate([CLK])]), basicio()], 0)
    reg.dependents[1].dependents[1] = reg
    reg.dependents[0].dependents[1] = notgate([reg])
    return notgate([reg])

class pulsegate(gate): # rising edge pulse
    def __init__(self, dependents):
        if len(dependents) != 1:
            raise NotImplementedError
        super().__init__(dependents, [0]*2, pulsefn)

    def setstate(self, state):
        self.state = state

    def update(self, touched_list=[]):
        super().update(touched_list)
        return self.state[0]

class multigate(gate): #Generic class for modules with multiple parallel gates
    def __init__(self, dependents, gatetype, instances, gateinit):
        self.gatearray = []
        gate.__init__(self, dependents, [], selffn)
        for _ in range(instances):
            self.gatearray.append(gatetype(*gateinit))
        self.dependents = self.gatearray

    def update(self, touched_list=[]):
        output = []
        for g in self.gatearray:
            output.append(booltobin(g.update([])))
        self.setstate(output)
        return output

    def setstate(self, state):
        self.state = state

    def __repr__(self):
        return str(self.update([]))

class inputarray(multigate):
    def __init__(self, inputs):
        multigate.__init__(self, [], basicio, inputs, [])

    def setstate(self, data):
        if len(data) != len(self.gatearray):
            raise SyntaxError
        for i,j in enumerate(data):
            self[i] = j

    def __getitem__(self, key):
        return self.gatearray[key]

    def __setitem__(self, key, value):
        self.gatearray[key].setstate(value)

class byteregister(gate): #LD, [DATA byte], CLK

    # load = basicio()
    # data = inputarray(8)
    # clk = basicio()
    # regs = byteregister([load, data, clk])
    # load.setstate(1)
    # clk.setstate(1)
    # data.setstate([0,1,1,1,0,0,1,1])
    # print(regs.verbose())

    def __init__(self, dependents):
        if len(dependents) != 3:
            raise NotImplementedError
        self.regs = []
        super().__init__(dependents, [0]*8, srfn)
        for i in range(8):
            self.regs.append(register(dependents[0], dependents[1][i], dependents[2], 0))

    def update(self, touched_list=[]):
        output = []
        for r in self.regs:
            touched_list.append(r)
            output.append(booltobin(r.update([])))
        self.setstate(output)
        return self.state

    def setstate(self, state):
        self.state = state

    def __repr__(self):
        return str(self.update([]))

class bytecounter(gate):
    # CLK = basicio()
    # bc = bytecounter(CLK)
    # for i in range(0,2**8):
    #     CLK.setstate(int(format(i, 'b')[-1]))
    #     print(bc)

    def __init__(self, CLK):
        self.dividers = []
        self.CLK = CLK
        super().__init__([CLK], [0]*8, srfn)
        self.dividers.append(divider(CLK))
        for i in range(1, 7):
            self.dividers.append(divider(self.dividers[i-1]))

    def update(self, touched_list=[]):
        output = []
        self.dividers[-1].update([])
        for d in range(6, -1, -1):
            output.append(booltobin(self.dividers[d].state))
        output.append(booltobin(self.CLK.state))
        self.state = output
        return self.state

    def __repr__(self):
        return str(self.update([]))

def main():
    print("Logic Gate Framework & Simulator (v1.0.0)")
    print("")
    print("Available Gates:")
    print("{0:<20}{1}".format("basicio", "represents a toggle switch for logic testing"))
    print("{0:<20}{1}".format("andgate", "(Nx1) AND gate"))
    print("{0:<20}{1}".format("orgate", "(Nx1) OR gate"))
    print("{0:<20}{1}".format("notgate", "(1x1) Inverter"))
    print("{0:<20}{1}".format("nandgate", "(Nx1) NAND gate"))
    print("{0:<20}{1}".format("norgate", "(Nx1) NOR gate"))
    print("{0:<20}{1}".format("xorgate", "(2x1) XOR gate"))
    print("{0:<20}{1}".format("xnorgate", "(2x1) XNOR gate"))
    print("{0:<20}{1}".format("pulsegate", "(1x1) Rising Edge simulator"))
    print("")
    print("Available Latches:")
    print("{0:<20}{1}".format("srlatch", "SR Latch, monostable input resolves to Q being set high"))
    print("{0:<20}{1}".format("esrlatch", "SR Latch with enable line"))
    print("{0:<20}{1}".format("dlatch", "D Latch (CLK, DATA)"))
    print("{0:<20}{1}".format("register", "A 1 bit register (LOAD, DATA, CLK, init_state)"))
    print("{0:<20}{1}".format("divider", "D Latch with ~Q -> D. Only input is CLK"))
    print("")
    print("Complex Gates:")
    print("{0:<20}{1}".format("inputarray", "A collection of n basicio(s) in parallel. Addressible via inputarray[i]"))
    print("{0:<20}{1}".format("byteregister", "Extended register, storing 8 bits. Data should be loaded via inputarray"))
    print("{0:<20}{1}".format("bytecounter", "Extended divider, can count to 256, needs to extend set function to be used for IPR"))
    print("")
    print("TODO:")
    print("- Fix inconsistency with calling some gates with an array and some with individual arguments")
    print("- Add SET to bytecounter for use as IPR")
    print("\n\n\n")

    inputarray(8).selftest()

if __name__ == "__main__":
    main()
