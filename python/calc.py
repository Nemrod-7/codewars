import re

operator = ['+','-','*','/']

def order (cell) :
    if cell == '+' or cell == '-' : return 1
    if cell == '*' or cell == '/' : return 2
    return 0

def operate (oper, value) :
    b, a = value.pop(), value.pop()

    match oper.pop() :
        case '+': value.append(a + b)
        case '-': value.append(a - b)
        case '*': value.append(a * b)
        case '/': value.append(a / b)

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
    return (prev >= 0 and expr[prev] in operator) or (next < len(expr) and expr[next] == '(')

def evaluate (expr) :
    running = True
    sign = 1
    i, oper, value = 0, [], []

    while running :
        if isminus(expr, i) : # prevents to calculate situation : "-7 * -(x)"
            # oper.append(expr[i])
            # value.append(0.0)
            sign = -1
            i += 1

        if expr[i] == '(' :
            oper.append(expr[i])
        elif expr[i] == ')' :

            while oper and oper[-1] != '(' :
                operate(oper, value)
            oper.pop();
        elif expr[i] in operator :

            while oper and order(oper[-1]) >= order(expr[i]) :
                operate(oper, value)
            oper.append(expr[i])
        elif isnumber (expr[i]) :
            value.append(float(expr[i]))
        # endif
        i += 1
        if i >= len(expr) : running = False
    # end

    while oper : operate(oper, value)

    return value.pop()

def calc (source) :
    expression  = re.findall(r"[-+]?[0-9]+\.?[0-9]*|[-+/*()]", source)
    print(expression)
    return evaluate(expression)


for i in range(0,5) : print()


print(calc("-99 / ((((68 + 2)))) * 75"))
# print(calc("(83) * (-27 + 42 / -(23)) * (-99 / -((((62 + 8)))) * 75)")) # -253782.81055900626

# print(calc("(12) + (52 / 57 - (69)) * (3 * -(((-(-67 / -47)))) + -72)"))
# print(calc("3 -(-1)"))
