import re

operator = ['+','-','*','/']

def order (cell) :
    if cell == '+' or cell == '-' : return 1
    if cell == '*' or cell == '/' : return 2
    return 0

def operate (oper, value) :
    b, a = value.pop(), value.pop()

    match oper.pop() :
        case '+': return (a + b)
        case '-': return (a - b)
        case '*': return (a * b)
        case '/': return (a / b)

def isnumber (expr) :
    try :
        float (expr)
        return True
    except :
        return False

def isminus (expr, idx) :
    prev , next = idx - 1, idx + 1
    if expr[idx] != '-' : return False
    if prev >= 0 and isnumber(expr[prev]) : return False
    return (next < len(expr) and expr[next] == '(')

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

def evaluate (expr) :
    running = True
    i, sign = 0, 1, 
    oper, value = [], []

    while running :
        if isminus(expr, i) : # prevents to calculate situation : "-7 * -(x)"
            sign = -1; i += 1
       
        if expr[i] == '(' :
            sub = getsub(expr[i:])
            value.append(evaluate(sub) * sign)
            sign = 1; i += len(sub) + 1
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

def calc (source) :
    expression  = re.findall(r"[-+]?[0-9]+\.?[0-9]*|[-+/*()]", source)
    # print(expression)
    return evaluate(expression)


for i in range(0,5) : print()

print(calc("-99 / -(68 + 2) * 75"))
# print(calc("(83) * (-27 + 42 / -(23)) * (-99 / -((((62 + 8)))) * 75)")) # -253782.81055900626

# print(calc("(12) + (52 / 57 - (69)) * (3 * -(((-(-67 / -47)))) + -72)"))
# print(calc("3 -(-1)"))
