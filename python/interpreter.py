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

##################################################################################
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

    def getargs(self,expr, index) :
        args = []
        nvar = len(self.func[expr[index]][0])

        while index + 1 < len(expr) and len(args) < nvar :
            index += 1
            arg = expr[index]

            if arg in self.func :
                [sub, nxt] = self.getargs(expr, index)
                arg += ' ' + ' '.join(sub)
                index = nxt

            args.append(arg)

        return [args,index]

    def input(self, source):
        i = 0
        running = True
        oper, stack = [], []
        number = ("^-?[0-9]*.?[0-9]+$")
        identf = ("_?[a-zA-Z]+_?|_[0-9]+");
        expr = tokenize(source)
        
        if len(expr) == 0 : return ''
        
        while running :
            if expr[i] == 'fn' :
                if expr[1] in self.vars : raise ValueError("ERROR: This name already exist as a variable.")
                mid = expr.index('=>')
                vars, func, args = expr[2:mid], expr[mid + 1:], {}
                
                for var in vars : 
                    if not var in func or var in args :  raise ValueError("ERROR: Invalid function.")
                    args[var] = True
                    
                self.func[expr[1]] = [vars, func]
                return ''
            elif expr[i] == '(' :
                sub = getsub(expr[i:])
                if 'fn'in sub : raise ValueError("ERROR.")
                stack.append(self.input(' '.join(sub)))
                i += len(sub) + 1
            elif re.match(number, expr[i]) :
                stack.append(float(expr[i]))
            elif re.match(identf, expr[i]): # if cell is a variable
                if '=' in expr :            # intialize variable
                    if expr[i] in self.func : raise ValueError("ERROR: a variable of the same name already exist.")

                    self.vars[expr[i]] = self.input(' '.join(expr[i+2:]))
                    return self.vars[expr[i]]
                elif expr[i] in self.vars : # return variable
                    stack.append( self.vars[expr[i]] )
                elif expr[i] in self.func : # input function
                    [vars,lmdb] = self.func[expr[i]]
                    [args, nxt], sub = self.getargs(expr, i), []

                    if len(vars) != len(args) : raise ValueError("ERROR: Invalid function.")

                    for cell in lmdb :
                        if cell in vars : cell = args[vars.index(cell)]
                        sub += cell

                    i = nxt 
                    stack.append( self.input(''.join(sub))) 
                else :
                    raise ValueError('ERROR: Unknown identifier.')
            elif expr[i] in operator :
                while oper and order(oper[-1]) >= order(expr[i]) :
                    stack.append( operate(oper, stack) )
                oper.append(expr[i])
                             
            i += 1
            if i >= len(expr) : running = False

        while oper : stack.append( operate(oper, stack))

        match len(stack) :
            case 0 : return 0
            case 1 : return stack.pop()
        
        raise ValueError('ERROR: Invalid input.')

##################################################################################
interpreter = Interpreter()

interpreter.input("fn avg x y => (x + y) / 2")
interpreter.input("fn echo x => x")
interpreter.input("fn f a b => a * b")
interpreter.input("fn g a b c => a * b * c")
actual = interpreter.input("g g 1 2 3 f 4 5 f 6 7")

print(actual)
print('end')
