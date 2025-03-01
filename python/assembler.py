
def interpreter (code) :
    end = len(code)
    index = 0
    register = {}
    getvar = lambda x : int(x) if not x.isalpha() else register[x]

    while index < end :
        # jump = 1
        com = code[index][0:3]
        key = code[index][4:5]
        var = code[index][6:]

        if com == 'inc' : register[key] += 1
        if com == 'dec' : register[key] -= 1
        if com == 'mov' : register[key] = getvar(var)
        if com == 'jnz' :
            cst = register[key] if key.isalpha() else int(key)
            index += 1 if cst == 0 else int(var)
            #if cst != 0 :
            #    # print(int(var) + index)
            #    if (int(var) + index < end) :
            #        jump = int(var)
            #    else :
            #        jump = end - index
        else :
            index += 1

        # index += jump
    return register



code = '''\
mov a 5
inc a
dec a
dec a
jnz a -1
inc a'''



actual = interpreter(code.splitlines())

print(actual)

