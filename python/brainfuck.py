
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

def brainfuck(code, input) :
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

def boolfuck (code, input="") :
    oss = []
    tape = [0] * 500
    index, it, bin, out = 0, 0, 0, 0

    while index < len(code) :
        match code[index] :
            case '>' :
                it += 1
                if it > len(tape) : tape.append(0)
            case '<' : it -= 1
            case '+' : tape[it] ^= 1

            case ';' :
                if out // 8 >= len(oss) : oss.append(0)
                oss[out // 8] |= tape[it] << (out % 8);
                out += 1
            case ',' :
                try :
                    tape[it] = ord(input[bin // 8]) >> (bin % 8) &1;
                    bin += 1;
                except :
                    pass

            case '[' :
                if tape[it] == 0 : index = find_next(code, index)
            case ']' :
                if tape[it] != 0 : index = find_next(code, index)

        index += 1
    return ''.join([chr(it) for it in oss])

# Increment a variable ... in brainfuck
#
# Your task is simple: add a number n (passed as parameter) to a variable, but with a few little caveats:
#     the variable is a cell in a BF interpreter, your task is to generate the BF code that adds n to the cell;
#     there is a limit on the size of generated code (42 chars);
#     the BF interpreter is a 12-bit interpreter, not a standard 8-bit one
#
# BF interpreter used in this kata
# Main features :
#     12-bit cells with wrap around: memory cells can contain values from 0 to 4095; incrementing (+) a cell which holds 4095 yields 0, and decrementing (-) a cell which holds 0 yields 4095.
#     array size: 8 cells: there are only 8 memory cells (more than sufficient for a single addition); moving the pointer outside the bounds of the array is an error.
#     30,000-steps timeout: after 30,000 steps the interpreter stops and gives an error. A step is defined as:
#         the execution of a single </>/[/] command (the matching bracket at the destination of a jump is not executed)
#         the execution of a sequence of consecutive + or - commands (++++ counts as a single step, ++---+ counts as 3 steps)
#     I/O commands are ignored
#
# Errors
# The tests will fail if, when the BF code is executed:
#     the pointer goes beyond the bounds of the array (at any time)
#     the execution requires more then 30,000 steps
#     the final value of the pointer is not 0 (the pointer must return to the first cell)
#     the final value of the first cell is not start + n (initial value + function arg) (mod 4096)
#     the final value of any other cell is non-zero

def increment (n) :
    if n < 42 : return '+' * n
    if n > 4054 : return '-' * (4095 - n)

# '+>>>++[[<+>>++<-]>]<<[-<->]' # set up tape : 0 2 4 8 16 32 192 : pos = 8

code = '+++++'
code += '>>+++++[<++>-]<'

code += '''
<[->
    [>+>+<<-]>>[-<<+>>]<<
< ]
'''

print(len(code))

mem = [0] * 12
index, it = 0, 0

while index < len(code) :
    match code[index] :
        case '>' : 
            it += 1
        case '<' : 
            it -= 1
        case '+' :
            mem[it] = 0 if mem[it] == 4095 else mem[it] + 1
        case '-' :
            mem[it] = 4095 if mem[it] == 0 else mem[it] - 1
        case '[' :
            if mem[it] == 0 : index = find_next(code, index)
        case ']' :
            if mem[it] != 0 : index = find_next(code, index)
    index += 1


for i in range(12) :
    if it == i :
        print(' ','{:^4}'.format('v'), end=' ')
    else :
        print(' ','{:^4}'.format(' '), end=' ')
print()

for i in range(12) :
    print('[','{:^4}'.format(mem[i]), end=']')
