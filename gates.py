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
        statestring = ""
        if isinstance(self.state, list):
            for i in self.state:
                statestring += format(i, 'b') + ", "
        else:
            statestring = format(self.state, 'b') + ", "
        return "(" + statestring[0:-2] + ")" + str(iolist) + " |-> " + str(self)

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
            print(self.verbose())

class multigate(gate): #Generic class for modules with multiple parallel gates
    def __init__(self, dependents, gatetype, instances, gateinit):
        self.gatearray = []
        super().__init__(dependents, [0]*instances, selffn)
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
            print(self.verbose())

def esrlatch(dependents, state): #Enable, S, R
    if len(dependents) != 3:
        raise NotImplementedError
    return srlatch([andgate([dependents[0], dependents[1]]), andgate([dependents[0], dependents[2]])], state)

def dlatch(dependents, state): #Enable, Data
    if len(dependents) != 2:
        raise NotImplementedError
    return srlatch([andgate([dependents[0], dependents[1]]), andgate([dependents[0], notgate([dependents[1]])])], state)

def register(dependents, state): #1 bit register, Load, Data, Clock, state
    if len(dependents) != 3:
        raise NotImplementedError
    reg = dlatch([dependents[2], orgate([andgate([basicio(), notgate([dependents[0]])]), andgate([dependents[0], dependents[1]])])], 0)
    reg.dependents[0].dependents[1].dependents[0].dependents[0] = reg
    return reg

def divider(dependents): # ~Q -> D. Chained dividers create a binary counter
    if len(dependents) != 1:
        raise NotImplementedError
    reg = dlatch([pulsegate([notgate([dependents[0]])]), basicio()], 0)
    reg.dependents[1].dependents[1] = reg
    reg.dependents[0].dependents[1] = notgate([reg])
    return notgate([reg])

class inputarray(multigate):
    def __init__(self, inputs):
        super().__init__([], basicio, inputs, [])
        self.length = inputs

    def setstate(self, data):
        if len(self.gatearray) > 0:
            if len(data) != len(self.gatearray):
                raise SyntaxError
            for i,j in enumerate(data):
                self[i] = j

    def __getitem__(self, key):
        return self.gatearray[key]

    def __setitem__(self, key, value):
        self.gatearray[key].setstate(value)

    def __len__(self):
        return self.length

class byteregister(multigate): #LD, [DATA byte], CLK
    def __init__(self, dependents):
        if len(dependents) != 3 or len(dependents[1]) != 8:
            raise NotImplementedError
        super().__init__(dependents, register, 8, [[dependents[0], dependents[1][0], dependents[2]], 0])
        for i in range(8):
            self.gatearray[i].dependents[0].dependents[1].dependents[1].dependents[1] = dependents[1][i]

class bytecounter(multigate): #8 bit binary counter, input CLK
    def __init__(self, dependents):
        if len(dependents) != 1:
            raise NotImplementedError
        super().__init__(dependents, divider, 7, [dependents])
        self.gatearray.append(dependents[0])
        for i in range(7):
            self.gatearray[i].dependents[0].dependents[0].dependents[0].dependents[0].dependents[0] = self.gatearray[i+1]

    def selftest(self):
        for i in range(0, 2**8):
            self.gatearray[-1].setstate(int(format(i, 'b')[-1]))
            print("{0:3d}".format(i), self.gatearray[-1], "|->", self.update([]))

def main():
    print("LogicSim (v1.0.0): A Logic Gate Simulation Framework")

if __name__ == "__main__":
    main()
