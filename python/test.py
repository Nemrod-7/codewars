import re

func = { 'echo': [['x'],['x']], 'avg': [['x','y'],['(','x','+', 'y',')','/','2']] }

def tokenize(expression):
    if expression == "" : return []
    regex = re.compile("\s*(=>|[-+*\/\%=\(\)]|[A-Za-z_][A-Za-z0-9_]*|[0-9]*\.?[0-9]+)\s*")
    code = regex.findall(expression)
    return [s for s in code if not s.isspace()]

def getargs(expr, index) :
    args = []
    nvar = len(func[expr[index]][0])

    while index + 1 < len(expr) and len(args) < nvar :
        index += 1
        arg = expr[index]

        if arg in func :
            [sub, nxt] = getargs(expr, index)
            arg += ' ' + ''.join(sub)
            index = nxt

        args.append(arg)

    return [args,index]



expr = tokenize('avg echo 4 echo 2')

args = getargs(expr, 0)
print('args : ', args)


