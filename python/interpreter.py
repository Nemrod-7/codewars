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
    if expression == "": return []
    regex = re.compile("\s*(=>|[-+*\/\%=\(\)]|[A-Za-z_][A-Za-z0-9_]*|[0-9]*\.?[0-9]+)\s*")
    tokens = regex.findall(expression)
    return [s for s in tokens if not s.isspace()]

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
    if re.match("-?[0-9]*.?[0-9]+$", expr[prev]) : return False
    if expr[prev] == ')' or expr[prev] == '(' : return False
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
        nvar = len(self.func[expr[index]][0])
        print('name : ', expr[index], index)
        while index + 1 < len(expr) and len(args) < nvar :
            index += 1
            arg = expr[index]
            print('arg ', arg, 'index ', index )

            if arg in self.func :
                sub = ' '.join( self.getargs(expr, index))
                arg += ' ' + sub


            # print('sub :', arg, end=' ')
            args.append(arg)

        return args

    def evaluate (self, source) :
        i, sign = 0, 1
        running = True
        oper, stack = [], []
        number = ("^-?[0-9]*.?[0-9]+$")
        identf = ("_?[a-zA-Z]+_?|_[0-9]+");
        expr = tokenize(source)
        
        if len(expr) == 0 : return ''

        if expr[0] == 'fn' :
            if expr[1] in self.func : raise ValueError("ERROR: this function already exist.")
            if expr[1] in self.vars : raise ValueError("ERROR: this name already exist as a variable.")
            mid = expr.index('=>')
            vars, func, args = expr[2:mid], expr[mid + 1:], {}
            
            for var in vars : 
                if not var in func or var in args :  raise ValueError("ERROR: Invalid function.")
                args[var] = True
                
            self.func[expr[1]] = [vars, func]
            return ''
        
        while running :
            if isminus(expr, i) : 
                sign = -1; i += 1

            if expr[i] == '(' :
                sub = getsub(expr[i:])
                stack.append(self.evaluate(' '.join(sub)) * sign)
                sign = 1; i += len(sub) + 1
            elif re.match(number, expr[i]) :
                stack.append(float(expr[i]) * sign)
                sign = 1
            elif re.match(identf, expr[i]): # if cell is a variable
                if '=' in expr :            # intialize variable
                    if expr[i] in self.func : raise ValueError("ERROR: a variable of the same name already exist.")

                    self.vars[expr[i]] = self.evaluate(' '.join(expr[i+2:]))
                    return self.vars[expr[i]]
                elif expr[i] in self.vars : # return variable
                    stack.append( self.vars[expr[i]] )
                elif expr[i] in self.func : # execute function
                    [vars,lmdb] = self.func[expr[i]]
                    args, sub = self.getargs(expr, i), []

                    if len(vars) != len(args) : raise ValueError("ERROR: Invalid function.")

                    for cell in lmdb :
                        if cell in vars : cell = args[vars.index(cell)]
                        sub += cell

                    i += len(args)
                    stack.append( self.evaluate(' '.join(sub))) 
                else :
                    raise ValueError('ERROR: Unknown identifier.')
            elif expr[i] in operator :
                while oper and order(oper[-1]) >= order(expr[i]) :
                    stack.append( operate(oper, stack) )
                oper.append(expr[i])
            # print(expr[i], stack, oper)
            i += 1
            if i >= len(expr) : running = False

        while oper : stack.append( operate(oper, stack))

        match len(stack) :
            case 0 : return 0
            case 1 : return stack.pop()
        
        raise ValueError('ERROR: Invalid input.')

    def input(self, expression):

        return self.evaluate(expression)    


    def prototype(self, source) :
        code = tokenize(source)
        index = 0

        args = self.getargs(code, index)

        print(args, index)

        pass


# Tests
class test :
    def assert_equals(actual, result) :

        if actual != result :
            print('actual : ', actual, 'expect : ', result)

    def expect_error(actual, input) :
        # (actual, input)
        pass

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
# test.assert_equals(interpret.input("fn inc x => x + 1"), (0))
# test.assert_equals(interpret.input("fn add x y => x + y"), (0))
#
# test.assert_equals(interpret.input("a = 0"), (0))
# test.assert_equals(interpret.input("a = inc a"), (1))
# test.assert_equals(interpret.input("a = inc a"), (2))
# test.assert_equals(interpret.input("a = inc a"), (3))

interpreter = Interpreter();

test.assert_equals(interpreter.input("fn echo x => x"), (''))
test.assert_equals(interpreter.input("fn avg x y => (x + y) / 2"), (''))

interpreter.prototype('avg echo 4 echo 2')

# # Basic arithmetic
# test.assert_equals(interpreter.input("1 + 1"), 2)
# test.assert_equals(interpreter.input("2 - 1"), 1)
# test.assert_equals(interpreter.input("2 * 3"), 6)
# test.assert_equals(interpreter.input("8 / 4"), 2)
# test.assert_equals(interpreter.input("7 % 4"), 3)
#
# # Variables
# test.assert_equals(interpreter.input("x = 1"), 1)
# test.assert_equals(interpreter.input("x"), 1)
# test.assert_equals(interpreter.input("x + 3"), 4)
# test.expect_error("input: 'y'", lambda : interpreter.input("y"))
#
# # Functions
#
# test.expect_error("input: 'avg 7'", lambda : interpreter.input("avg 7"))
# test.expect_error("input: 'avg 7 2 4'", lambda : interpreter.input("avg 7 2 4"))
# # Conflicts
# test.expect_error("input: 'fn x => 0'", lambda : interpreter.input("fn x => 0"))
# test.expect_error("input: 'avg = 5'", lambda : interpreter.input("avg = 5"))
#
# test.assert_equals(interpreter.input("avg 4 2"), 3)
#


for name in interpreter.func :
    [var, lam] = interpreter.func[name]
    # print(name, len(var))
    # print(name, '=> ', var, lam)

for name in interpreter.vars :
    # print(name, '=>', interpret.vars[name])
    pass


print('end')
