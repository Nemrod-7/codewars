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

def isnumber (expr) :
    try :
        float(expr)
        return True
    except :
        return False

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
        self.functions = {}


    def evaluate (self, expr) :
        running = True
        i, sign = 0, 1, 
        oper, value = [], []
        # identf =  ("_?[a-zA-Z]+_?|_[0-9]+");

        while running :
            if isminus(expr, i) : sign = -1; i += 1
           
            if expr[i] == '(' :
                sub = getsub(expr[i:])
                value.append(self.evaluate(sub) * sign)
                sign = 1; i += len(sub) + 1
            elif expr[i] == 'fn' :

                pass

            elif expr[i].isalnum() :

                if '=' in expr :
                    # print(float(''.join( expr[i+2:])))
                    try :
                        if not expr[i] in self.vars :
                            self.vars[expr[i]] = float( self.evaluate( ''.join( expr[i+2:])))
                            return self.vars[expr[i]]
                    except :
                        return 'ERROR: Invalid identifier. Existing variable'
                else :
                    if expr[i] in self.vars :
                        return self.vars[expr[i]]

            elif expr[i] in operator :
                while oper and order(oper[-1]) >= order(expr[i]) :
                    value.append( operate(oper, value) )
                oper.append(expr[i])
            elif isnumber (expr[i]) :
                value.append(float(expr[i]) * sign)
                sign = 1
            
            i += 1
            if i >= len(expr) : running = False

        while oper : value.append( operate(oper, value))

        return value.pop()

    def input(self, expression):
        code = tokenize(expression)
        index = 0
        if len(code) == 0 : return 'ERROR: Empty expression'
        
        # print('expression : ', code)

        return self.evaluate(code)


# Tests
class test :
    def assert_equals(actual, result) :

        if actual != result :
            print('actual : ', actual, 'expect : ', result)

interpreter = Interpreter()

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
#     test.assert_equals(interpreter.input(x), y)
# 
# # Basic arithmetic
# test.assert_equals(interpreter.input("1 + 1"), 2)
# test.assert_equals(interpreter.input("2 - 1"), 1)
# test.assert_equals(interpreter.input("2 * 3"), 6)
# test.assert_equals(interpreter.input("8 / 4"), 2)
# test.assert_equals(interpreter.input("7 % 4"), 3)

test.assert_equals(interpreter.input("x2 = 4"), 4)


print('end')
