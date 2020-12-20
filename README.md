# LogicSim
Logic Gate Simulation Framework

## Usage  
All gates are initialized via `gatetype(*args)`,  \*args differ, see below.

All gates internally use `True` and `False` for states. Conversion to 1 and 0 can be made via `booltobin(state)`

Default methods are as follows:  
* `.update()` or `.update(touched_list)`
    - calculates and sets the new state of the gate, recursively calling `.update(touched_list)` on all inputs.  
    - returns `self.state` (True/False).
    - `touched_list` is necessary for gates that recursively use themselves as inputs (i.e. a D flip-flop with ~Q feeding into D)

* `.setstate(state)`
    - changes the current output state of the gate without recalculation of inputs.  
    - Mostly only used for `basicio()` and to set latch memory states

* `.verbose()`
    - returns a formatted string `"(current_state)[states of all inputs] |-> new_state"`.  
    - States of each object are calculated with `.update([])`

* `.__repr__()` or via `str(gatetype)`
    - calls `.update([])` on self and returns `"new_state"`.  
    - Since most gates only have one output, this defaults to returning a single bit `"1"` or `"0"`

* `.getbasicio()` or `.getbasicio(touched_list)`
    - returns a list of all unique `basicio()` objects in the gate hierarchy

* `.selftest()`
    - Tests all possible binary inputs to unique `basicio()` objects in the gate hierarchy and prints the results
    - ex: `andgate([basicio(), basicio()]).selftest()` prints
    ```
    [0, 0] |-> 0
    [0, 1] |-> 0
    [1, 0] |-> 0
    [1, 1] |-> 1
    ```

## Gates

### basicio  
Represents an in/out logic element like a toggle switch.  
* Initialize: `basicio()` or `basicio(state)` (state defaults to 0)  
* Set: `.setstate(state)`  
* Get: `.update()`
    - defaults to True/False. If 1/0 is required, use `booltobin(basicio().update())`

### andgate  
An N-input AND gate
* Initialize: `andgate([list of gatetypes])`, ex: `andgate([basicio(), basicio()])`
* Set: None. Set inputs and call `.update([])`
* Get: `.update([])`

### orgate  
An N-input OR gate
* Initialize: `orgate([list of gatetypes])`, ex: `orgate([basicio(), basicio()])`
* Set: None. Set inputs and call `.update([])`
* Get: `.update([])`

### notgate  
A 1-input NOT gate
* Initialize: `notgate([gatetype])`, ex: `notgate([basicio()])`
* Set: None. Set inputs and call `.update([])`
* Get: `.update([])`

### nandgate  
An N-input NAND gate
* Initialize: `nandgate([list of gatetypes])`, ex: `nandgate([basicio(), basicio()])`
* Set: None. Set inputs and call `.update([])`
* Get: `.update([])`

### norgate  
An N-input NOR gate
* Initialize: `norgate([list of gatetypes])`, ex: `norgate([basicio(), basicio()])`
* Set: None. Set inputs and call `.update([])`
* Get: `.update([])`

### xorgate  
A 2-input XOR gate
* Initialize: `xorgate([gatetype, gatetype])`, ex: `xorgate([basicio(), basicio()])`
* Set: None. Set inputs and call `.update([])`
* Get: `.update([])`

### xnorgate  
A 2-input XNOR gate
* Initialize: `xnorgate([gatetype, gatetype])`, ex: `xnorgate([basicio(), basicio()])`
* Set: None. Set inputs and call `.update([])`
* Get: `.update([])`

### pulsegate
A 1-input rising edge pulse generator
* Initialize: `pulsegate([gatetype])`, ex: `pulsegate([basicio()])`
* Behavior: Goes high when input goes high and `.update([])` is called. Goes low on next `.update([])` and stays low until input toggles.
* Set: None. Toggle the input while calling `.update([])`
* Get: `.update([])`

## Latches

### srlatch
A basic SR Latch.
* Metastable condition with S,R = (1,1) results in Q being set high/True
* Initialize: `srlatch([S, R], init_state)`, ex: `srlatch([basicio(), basicio()], 0)`
* Set: `.setstate(state)` or set inputs and call `.update([])`
* Get: `.update([])`

### esrlatch  
SR Latch with Enable input
* Initialize: `esrlatch([E, S, R], init_state)`, ex: `esrlatch([basicio(), basicio(), basicio()], 0)`
* Set: `.setstate(state)` or set inputs and call `.update([])`
* Get: `.update([])`

### dlatch
D Latch (Enable, Data) -> Q
* Initialize: `dlatch([E, D], state)`, ex: `dlatch([basicio(), basicio()], 0)`
* Set: `.setstate(state)` or set inputs and call `.update([])`
* Get: `.update([])`

### register
1 bit memory register (Load, Data, Enable/CLK)
* Initialize: `register([L, D, CLK], init_state)`, ex: `register([basicio(), basicio(), basicio()], 0)`
* Set: `.setstate(state)` or set inputs and call `.update([])`
* Get: `.update([])`

### divider
A rising edge clock divider, made by setting ~Q -> D. Default init_state is 0
* Initialize: `divider([CLK])`, ex: `divider([basicio()])`
* Set: `.setstate(state)` or toggle CLK input while calling `.update([])`
* Get: `.update([])`

## Multigates  
Modules with many parallel, sometimes interconnected, gates

### inputarray  
N parallel `basicio` objects
* Initialize: `inputarray(n)`, ex: `inputarray(8)`
* Set: `.setstate([boolean list of length n])`, ex: `inputarray(8).setstate([0,0,1,0,1,1,1,0])`
* Get: `.update([])`, returns a list of the states of all n `basicio` objects
    - `inputarray` is indexable, allowing explicitly getting and setting the i<sup>th</sup> `basicio` object.
    ex:
    ```python
    inarr = inputarray(8)
    inarr.setstate([0,0,1,0,1,1,1,0])
    print(inarr[2]) #prints 1
    inarr[2] = 0
    print(inarr[2]) #prints 0
    ```

### byteregister
8 parallel `register` objects (Load, Data, Enable/CLK)
* Initialize: `byteregister([L, [gatetype list of length 8], CLK])`, an `inputarray(8)` object can be used as the Data list
    - ex: `byteregister([basicio(), inputarray(8), basicio()])`
* Set: None. Set inputs and call `.update([])`
* Get: `.update([])`, returns a list of the states of all 8 `register` objects

### bytecounter
7 serial `divider` objects, fed by a single input (CLK)
* Initialize: `bytecounter([gatetype])`, ex: `bytecounter([basicio()])`
* Set: None. Toggle the CLK while calling `.update([])`
    - A Set function is planned to make this useful as an Instruction Counter Register
* Get: `.update([])`, returns a list of the states of all 7 `divider` objects, followed by the CLK state.
