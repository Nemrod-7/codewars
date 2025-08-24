

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

def assert_equals(actual, expect) :
    if actual != expect :
        print('actual : ', actual, 'expect : ', expect)

def fixed_tests():
    assert_equals(boolfuck("", ""), "")

    assert_equals(boolfuck("<"), "")
    assert_equals(boolfuck(">"), "")
    assert_equals(boolfuck("+"), "")
    assert_equals(boolfuck("."), "")
    assert_equals(boolfuck(";"), "\u0000")

    assert_equals(boolfuck(";;;+;+;;+;+;+;+;+;+;;+;;+;;;+;;+;+;;+;;;+;;+;+;;+;+;;;;+;+;;+;;;+;;+;+;+;;;;;;;+;+;;+;;;+;+;;;+;+;;;;+;+;;+;;+;+;;+;;;+;;;+;;+;+;;+;;;+;+;;+;;+;+;+;;;;+;+;;;+;+;+;", ""), "Hello, world!\n")

    assert_equals(boolfuck(">,>,>,>,>,>,>,>,<<<<<<<[>]+<[+<]>>>>>>>>>[+]+<<<<<<<<+[>+]<[<]>>>>>>>>>[+<<<<<<<<[>]+<[+<]>>>>>>>>>+<<<<<<<<+[>+]<[<]>>>>>>>>>[+]<<<<<<<<;>;>;>;>;>;>;>;<<<<<<<,>,>,>,>,>,>,>,<<<<<<<[>]+<[+<]>>>>>>>>>[+]+<<<<<<<<+[>+]<[<]>>>>>>>>>]<[+<]", "Codewars\u00ff"), "Codewars")
    assert_equals(boolfuck(">,>,>,>,>,>,>,>,>+<<<<<<<<+[>+]<[<]>>>>>>>>>[+<<<<<<<<[>]+<[+<]>;>;>;>;>;>;>;>;>+<<<<<<<<+[>+]<[<]>>>>>>>>>[+<<<<<<<<[>]+<[+<]>>>>>>>>>+<<<<<<<<+[>+]<[<]>>>>>>>>>[+]+<<<<<<<<+[>+]<[<]>>>>>>>>>]<[+<]>,>,>,>,>,>,>,>,>+<<<<<<<<+[>+]<[<]>>>>>>>>>]<[+<]", "Codewars"), "Codewars")

    assert_equals(boolfuck(">,>,>,>,>,>,>,>,>>,>,>,>,>,>,>,>,<<<<<<<<+<<<<<<<<+[>+]<[<]>>>>>>>>>[+<<<<<<<<[>]+<[+<]>>>>>>>>>>>>>>>>>>+<<<<<<<<+[>+]<[<]>>>>>>>>>[+<<<<<<<<[>]+<[+<]>>>>>>>>>+<<<<<<<<+[>+]<[<]>>>>>>>>>[+]>[>]+<[+<]>>>>>>>>>[+]>[>]+<[+<]>>>>>>>>>[+]<<<<<<<<<<<<<<<<<<+<<<<<<<<+[>+]<[<]>>>>>>>>>]<[+<]>>>>>>>>>>>>>>>>>>>>>>>>>>>+<<<<<<<<+[>+]<[<]>>>>>>>>>[+<<<<<<<<[>]+<[+<]>>>>>>>>>+<<<<<<<<+[>+]<[<]>>>>>>>>>[+]<<<<<<<<<<<<<<<<<<<<<<<<<<[>]+<[+<]>>>>>>>>>[+]>>>>>>>>>>>>>>>>>>+<<<<<<<<+[>+]<[<]>>>>>>>>>]<[+<]<<<<<<<<<<<<<<<<<<+<<<<<<<<+[>+]<[<]>>>>>>>>>[+]+<<<<<<<<+[>+]<[<]>>>>>>>>>]<[+<]>>>>>>>>>>>>>>>>>>>;>;>;>;>;>;>;>;<<<<<<<<", "\u0008\u0009"), "\u0048")

fixed_tests()
