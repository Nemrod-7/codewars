import re

def clean (line) :
    it = line.find(';')
    return line[0:it] if it != -1 else line

def assembler_interpreter (source):
    oss = ''
    cmp = 0
    reg = {}
    stack = []
    index, running = 0, True
    formt = re.compile("[;|\\w].+")
    quote = re.compile("\".+\"|\'.+\'")
    token = re.compile("\\w+:?|\'.+?\'")

    prog = formt.findall(source)

    while running :
        jump, line = False, token.findall(clean(prog[index]))
        com = line[0] if len(line) > 0 else ''
        key = line[1] if len(line) > 1 else ''
        var = ' ' if len(line) <= 2 or com == 'msg' else int(line[2]) if line[2].isnumeric() else reg[line[2]]

        if com == "inc": reg[key] += 1
        if com == "dec": reg[key] -= 1
        if com == "mov": reg[key]  = var
        if com == "add": reg[key] += var
        if com == "sub": reg[key] -= var
        if com == "mul": reg[key] *= var
        if com == "div": reg[key] //= var
        if com == "cmp": cmp = reg[key] - var
        if com == "jmp": jump = True
        if com == "end": return oss
        if com == "je"  and cmp == 0: jump = True
        if com == "jg"  and cmp  > 0: jump = True
        if com == "jl"  and cmp  < 0: jump = True
        if com == "jne" and cmp != 0: jump = True
        if com == "jge" and cmp >= 0: jump = True
        if com == "jle" and cmp <= 0: jump = True
        if com == "ret": index = stack.pop() - 1 

        if com == "call": 
            stack.append(index + 1)
            jump = True

        if jump == True :
            for i in range(0,len(prog)) : 
                if line[1] + ':' in prog[i] : index = i

        if com == "msg": 
            for i in range(1, len(line)) :
                oss += line[i][1: len(line[i])-1] if quote.match(line[i]) else str(reg[line[i]])

        index += 1
        if index >= len(prog) : running = False

    return -1
