# function        ::= fn-keyword fn-name { identifier } fn-operator expression
# fn-name         ::= identifier
# fn-operator     ::= '=>'
# fn-keyword      ::= 'fn'
# 
# expression      ::= factor | expression operator expression
# factor          ::= number | identifier | assignment | '(' expression ')' | function-call
# assignment      ::= identifier '=' expression
# function-call   ::= fn-name { expression }
# 
# operator        ::= '+' | '-' | '*' | '/' | '%'
# 
# identifier      ::= letter | '_' { identifier-char }
# identifier-char ::= '_' | letter | digit
# 
# number          ::= { digit } [ '.' digit { digit } ]
# 
# letter          ::= 'a' | 'b' | ... | 'y' | 'z' | 'A' | 'B' | ... | 'Y' | 'Z'
# digit           ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'


import re

operator = ['+','-','*','/','%']

def tokenize(expression):
    if expression == "" : return []

    regex = re.compile("\s*(=>|[-+*\/\%=\(\)]|[A-Za-z_][A-Za-z0-9_]*|[0-9]*\.?[0-9]+)\s*")
    code = regex.findall(expression)

    return [s for s in code if not s.isspace()]

def order (cell) :
    if cell == '+' or cell == '-' : return 1
    if cell == '*' or cell == '/' or cell == '%' : return 2
    return 0

def operate (oper, stack) :
    b, a = stack.pop(), stack.pop()

    match oper.pop() :
        case '+': return (a + b)
        case '-': return (a - b)
        case '*': return (a * b)
        case '/': return (a / b)
        case '%': return (a % b)

def isminus (expr, idx) :
    prev , next = idx - 1, idx + 1

    if expr[idx] != '-' : return False
    if idx == 0 : return True

    if isnumber(expr[prev]) : return False
    if expr[prev] == ')' : return False
    if expr[prev] == '(' : return False
    if expr[prev] in operator : return True

    return False

def getsub (expr) : 
    sub = []
    i , pile = 1, 1

    while True :
        match expr[i] :
            case '(' : pile += 1
            case ')' : pile -= 1

        if pile  == 0 : return sub
        sub.append(expr[i])
        i += 1

    return []

class Interpreter:
    def __init__(self):
        self.vars = {}
        self.func = {}

    def getargs(self, expr, index) :
        args = []
        nvar = len(self.func[expr[index]])

        while index + 1 < len(expr) and len(args) < nvar :
            index += 1
            arg = ' '.join(expr[index])
            
            if arg in self.func :
                sub = ' '.join( self.getargs(expr, index))
                arg += sub

            args.append(arg)

        return args

    def evaluate (self, expr) :
        i, sign = 0, 1
        running = True
        oper, stack = [], []
        number = ("^-?[0-9]*.?[0-9]+$")
        identf = ("_?[a-zA-Z]+_?|_[0-9]+");

        while running :
            if isminus(expr, i) : 
                sign = -1; i += 1

            if expr[i] == '(' :
                sub = getsub(expr[i:])
                stack.append(self.evaluate(sub) * sign)
                sign = 1; i += len(sub) + 1
            elif re.match(number, expr[i]) :
                stack.append(float(expr[i]) * sign)
                sign = 1
            elif re.match(identf, expr[i]): # if cell is a variable
                if '=' in expr :            # intialize variable
                    try :
                        if not expr[i] in self.vars :
                            self.vars[expr[i]] = self.evaluate(expr[i+2:])
                            return self.vars[expr[i]]
                    except :
                        return 'ERROR: Invalid identifier. Existing variable'
                elif expr[i] in self.vars : # return variable
                    stack.append( self.vars[expr[i]] )
                elif expr[i] in self.func : # execute function
                    name = expr[i]
                    [vars,lmdb] = self.func[expr[i]]
                    args = self.getargs(expr, i)

                    if len(vars) != len(args) : return "ERROR: Invalid function."
                    print(vars, args)

                    for cell in lmdb :
                        if cell in vars :
                            cell = args[vars.index(cell)]

                    print(lmdb)
            elif expr[i] in operator :
                while oper and order(oper[-1]) >= order(expr[i]) :
                    stack.append( operate(oper, stack) )
                oper.append(expr[i])
            else :
                pass

            i += 1
            if i >= len(expr) : running = False

        while oper : stack.append( operate(oper, stack))

        return stack.pop()

    def input(self, expression):
        code = tokenize(expression)
        if len(code) == 0 : return 'ERROR: Empty expression'

        if code[0] == 'fn' :
            if code[1] in self.func : return "ERROR : this function already exist."

            mid = code.index('=>')
            self.func[code[1]] = [code[2:mid], code[mid + 1:]]

            return 0
        else :
            return self.evaluate(code)


# Tests
class test :
    def assert_equals(actual, result) :

        if actual != result :
            print('actual : ', actual, 'expect : ', result)

    def expect_error(actual, input) :
        # print(actual, input)

        pass

interpret = Interpreter()

# cases = (
#     ("1 + 1", 2),
#     ("8/16", 0.5),
#     ("3 -(-1)", 4),
#     ("2 + -2", 0),
#     ("10- 2- -5", 13),
#     ("(((10)))", 10),
#     ("3 * 5", 15),
#     ("-7 * -(6 / 3)", 14)
# )
# 
# for x, y in cases:
#     test.assert_equals(interpret.input(x), y)
# 
# # Basic arithmetic
# test.assert_equals(interpret.input("1 + 1"), 2)
# test.assert_equals(interpret.input("2 - 1"), 1)
# test.assert_equals(interpret.input("2 * 3"), 6)
# test.assert_equals(interpret.input("8 / 4"), 2)
# test.assert_equals(interpret.input("7 % 4"), 3)

test.assert_equals(interpret.input("x = 4"), 4)
test.assert_equals(interpret.input("x"), 4)
test.assert_equals(interpret.input("x + 3"), 7)
test.expect_error("input: 'y'", lambda : interpret.input("y"))

test.assert_equals(interpret.input("x = 3"), 3)

test.assert_equals(interpret.input("fn inc x => x + 1"), (0))
test.assert_equals(interpret.input("fn avg x y => (x + y) / 2"), (0))
test.assert_equals(interpret.input("fn add x y => x + y"), (0))
test.assert_equals(interpret.input("fn echo x => x"), (0))

test.assert_equals(interpret.input("a = 0"), (0))

test.assert_equals(interpret.input("inc a"), (1))

for name in interpret.func :
    var = interpret.func[name][0]
    lam = interpret.func[name][1]
    # print(name, '=> ', var, lam)

for name in interpret.vars :
    # print(name, '=>', interpret.vars[name])
    pass


print('end')
