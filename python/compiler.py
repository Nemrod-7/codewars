import re

    # function   ::= '[' arg-list ']' expression
    #
    # arg-list   ::= /* nothing */
    #              | variable arg-list
    #
    # expression ::= term
    #              | expression '+' term
    #              | expression '-' term
    #
    # term       ::= factor
    #              | term '*' factor
    #              | term '/' factor
    #
    # factor     ::= number
    #              | variable
    #              | '(' expression ')'

operator = ['+','-','*','/']

def isnumber (x) :
    try :
        int(x)
        return True
    except :
        return False

def order (x) :
    if x == '+' or x == '-' : return 1
    if x == '*' or x == '/' : return 2
    return 0

class Compiler(object):
    def tokenize (self, program) : # Turn a program string into an array of tokens.  Each token is either '[', ']', '(', ')', '+', '-', '*', '/', a variable name or a number (as a string)
        token_iter = (m.group(0) for m in re.finditer(r'[-+*/()[\]]|[A-Za-z]+|\d+', program))
        return [int(tok) if tok.isdigit() else tok for tok in token_iter]

    def operate (self, vars, oper) :
        b, a = vars.pop(), vars.pop()
        op = oper.pop()
        vars.append( { 'op':op, 'a':a, 'b':b } )

    def pass1 (self, program) : # Returns an un-optimized AST
        it = program.find(']')
        args = self.tokenize(program[1:it])
        expr = self.tokenize(program[it+1:])

        vars, oper = [], []

        for cell in expr :
            if isnumber(cell) :
                vars.append( { 'op':'imm', 'n':cell } )
            elif cell in operator :
                while oper and order(oper[-1] >= cell) :
                    self.operate(vars,oper)
                oper += cell
            elif cell == '(' :
                oper += cell
            elif cell == ')' :
                while oper and oper[-1] != '(' :
                    self.operate(vars,oper)
                if oper : oper.pop()
            else :
                try :
                    vars.append( { 'op':'arg', 'n': args.index(cell)} )
                except :
                    return 'ERROR: Invalid program.'

        while oper : self.operate(vars,oper)
        return vars.pop()
        
    def pass2 (self, ast) : # Returns an AST with constant expressions reduced
        if ast['op'] == 'imm' or ast['op'] == 'arg' : return ast
        ast['a'] = self.pass2(ast['a'])
        ast['b'] = self.pass2(ast['b'])

        if ast['a']['op'] == 'imm' and ast['b']['op'] == 'imm' :
            a, b, n = ast['a']['n'], ast['b']['n'], 0
            match ast['op'] :
                case '+' : n = a + b
                case '-' : n = a - b
                case '*' : n = a * b
                case '/' : n = a / b
            ast = {'op':'imm', 'n':n }
        return ast

    def pass3 (self, ast) : # Returns assembly instructions
        temp = ast
        s1, s2 = [], []
        s1.append(temp)
        
        while s1 :
            temp = s1.pop()
            s2.append(temp)
            if temp['op'] != 'imm' and temp['op'] != 'arg' :
                if temp['a'] : s1.append(temp['a'])
                if temp['b'] : s1.append(temp['b'])

        print(s2)
        oss = ''
        tree = {}
        
        while tree :
            node = tree.pop()
            if node['op'] == 'imm' : 
                oss += 'IM' + str(node['n'])
            elif node['op'] == 'arg' :
                oss += 'AR' + str(node['n'])
            else :
                oss += ['PO','SW','PO']
                if node['op'] == '+' : oss += 'AD'
                if node['op'] == '-' : oss += 'SU'
                if node['op'] == '*' : oss += 'MU'
                if node['op'] == '/' : oss += 'DI'
        return oss
    def compile(self, program) :
        return self.pass3(self.pass2(self.pass1(program)))

def simulate(asm, argv):
    r0, r1 = None, None
    stack = []
    for ins in asm:
        if ins[:2] == 'IM' or ins[:2] == 'AR':
            ins, n = ins[:2], int(ins[2:])
        if   ins == 'IM':   r0 = n
        elif ins == 'AR': r0 = argv[n]
        elif ins == 'SW': r0, r1 = r1, r0
        elif ins == 'PU': stack.append(r0)
        elif ins == 'PO': r0 = stack.pop()
        elif ins == 'AD': r0 += r1
        elif ins == 'SU': r0 -= r1
        elif ins == 'MU': r0 *= r1
        elif ins == 'DI': r0 /= r1
    return r0

gcc = Compiler()

prog = '[ x y z ] ( 2*3*x + 5*y - 3*z ) / (1 + 3 + 2*2)';
 
t1 = {'op':'/','a':{'op':'-','a':{'op':'+','a':{'op':'*','a':{'op':'*','a':{'op':'imm','n':2},'b':{'op':'imm','n':3}},'b':{'op':'arg','n':0}},'b':{'op':'*','a':{'op':'imm','n':5},'b':{'op':'arg','n':1}}},'b':{'op':'*','a':{'op':'imm','n':3},'b':{'op':'arg','n':2}}},'b':{'op':'+','a':{'op':'+','a':{'op':'imm','n':1},'b':{'op':'imm','n':3}},'b':{'op':'*','a':{'op':'imm','n':2},'b':{'op':'imm','n':2}}}};

t2 = {'op':'/','a':{'op':'-','a':{'op':'+','a':{'op':'*','a':{'op':'imm','n':6},'b':{'op':'arg','n':0}},'b':{'op':'*','a':{'op':'imm','n':5},'b':{'op':'arg','n':1}}},'b':{'op':'*','a':{'op':'imm','n':3},'b':{'op':'arg','n':2}}},'b':{'op':'imm','n':8}};
        
ast = gcc.pass1(prog)
ast = gcc.pass2(ast)
act = gcc.pass3(ast)
print(act)
