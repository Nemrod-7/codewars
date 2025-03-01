def find_next (code, pos) :

    size = len(code)
    index = pos
    pile = 0 
    fwrd = True
    if code[index] == ']' : fwrd = False

    while index < size :
        if code[index] == '[' : pile += 1
        if code[index] == ']' : pile -= 1

        if pile == 0 : return index
        index += 1 if fwrd == True else -1

    return index

def boolfuck (code, input="") :
    
    oss = ''
    tape = [0] * 30
    index, it, bin, out = 0, 0, 0, 0
    input = iter(input)

    while index < len(code) :
        match code[index] :
            case '>' : it += 1
            case '<' : it -= 1
            case '+' : tape[it] ^= 1

            case ';' : 
                if out / 8 >= len(oss) : oss += 0
                oss[out / 8] |= tape[it] << (out % 8);
                out += 1

            case ',' : 
                try :
                    tape[it] = input[bin / 8] >> (bin % 8) &1;
                    bin += 1;
                except :
                    pass

            case '[' : 
                if tape[it] == 0 : index = find_next(code, index)
            case ']' : 
                if tape[it] != 0 : index = find_next(code, index)

        index += 1    

    return oss

def brain_luck(code, input) :

    oss = ''
    tape = [0] * 30
    index, it, id = 0, 0, 0
    input = iter(input)

    print(input)
    
    while index < len(code) :
        match code[index] :
            case '>' : it += 1
            case '<' : it -= 1
            case '+' : 
                tape[it] = 0 if tape[it] == 255 else tape[it] + 1
            case '-' : 
                tape[it] = 255 if tape[it] == 0 else tape[it] - 1
            case '.' : 
                oss += chr(tape[it])
            case ',' : 
                try :
                    tape[it] = ord(next(input))
                except :
                    tape[it] = 0
            case '[' : 
                if tape[it] == 0 : index = find_next(code, index)
            case ']' : 
                if tape[it] != 0 : index = find_next(code, index)

        index += 1    

    return oss

brain_luck(',[.[-],]', 'Codewars' + chr(0))

