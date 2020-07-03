def booltobin(state):
    return int(format(bool(state), 'b'))

def selffn(inputs, state, touched_list):
    return state

def andfn(inputs, state, touched_list):
    out = True
    for i in inputs:
        out &= i.update(touched_list)
    return out

def orfn(inputs, state, touched_list):
    out = False
    for i in inputs:
        out |= i.update(touched_list)
    return out

def notfn(inputs, state, touched_list):
    return not inputs[0].update(touched_list)

def srfn(inputs, state, touched_list):
    S, R = inputs
    if S.update(touched_list):
        return True
    if R.update(touched_list):
        return False
    return state

def pulsefn(inputs, state, touched_list):
    inp = inputs[0].update(touched_list)
    if state[0]:
        return [False, inp]
    if state[1]:
        return [False, inp]
    return [inp, inp]
