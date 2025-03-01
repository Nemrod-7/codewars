
import re

operator = ['+','-','*','/']

def tokenize(expression):
    if expression == "": return []

    regex = re.compile("\s*(=>|[-+*\/\%=\(\)]|[A-Za-z_][A-Za-z0-9_]*|[0-9]*\.?[0-9]+)\s*")
    code = regex.findall(expression)

    return [s for s in code if not s.isspace()]
def isnumber (expr) :
    try :
        int (expr)
        return True
    except :
        return False

def order (cell) :
    if cell == '+' or cell == '-' : return 1
    if cell == '*' or cell == '/' : return 2
    return 0

def operate (oper, stack) :
    b, a = stack.pop(), stack.pop()

    match oper.pop() :
        case '+': return (a + b)
        case '-': return (a - b)
        case '*': return (a * b)
        case '/': return (a / b)

class Interpreter:
    def __init__(self):
        self.vars = {}
        self.functions = {}

    def input(self, expression):
        index = 0
        running = True
        oper, stack = [],[]
        code = tokenize(expression)

        while running :
            
            if code[index] in operator :
                while oper and order(oper[-1]) >= order(code[index]) :
                    stack.append( operate(oper, stack) )
                oper.append(code[index])

            elif isnumber(code[index]) :
                stack.append(int(code[index]))

            index += 1
            if index >= len(code) : running = False

        while oper : stack.append( operate(oper, stack))

        return stack.pop()

class test :
    def assert_equals(actual, result) :

        print(actual, result)


interpreter = Interpreter()


test.assert_equals(interpreter.input("1 + 1"), 2)
test.assert_equals(interpreter.input("2 - 1"), 1)
test.assert_equals(interpreter.input("2 * 3"), 6)
test.assert_equals(interpreter.input("8 / 4"), 2)
test.assert_equals(interpreter.input("7 % 4"), 3)
