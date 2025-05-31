import re

def is_number (x) :
    try :
        num = float(x)
        return True
    except :
        return False

def trunc (fx) :
    ix = int(fx)
    if fx - ix > 0.0 : return str(float(fx))
    return str(ix)

def form(sym, a, b='') :
    return '('+ sym + ' ' + a + (' ' if b else '') + b + ')'

def add (a, b) :
    if is_number(a) and is_number(b) : return trunc(float(a) + float(b))
    if a == '0' : return b
    if b == '0' : return a
    return form('+',a,b)

def sub (a, b) :
    if is_number(a) and is_number(b) : return trunc(float(a) - float(b))
    if b == '0' : return a
    return form('-',a,b)

def mul (a, b) :
    if is_number(a) and is_number(b) : return trunc(float(a) * float(b))
    if a == '0' or b == '0' : return '0'
    if a == '1' : return b
    if b == '1' : return a
    return form('*',a,b)

def div (a, b) :
    if is_number(a) and is_number(b) : return trunc(float(a) / float(b))
    if b == '1' : return a
    return form('/',a,b)

def exp (a, b) :
    if is_number(a) and is_number(b) : return trunc(float(a) ** float(b))
    if b == '1' : return a
    if a == '1' or b == '0' : return '1'
    return form('^',a,b)

def diff (input) :
    if input == 'x' : return '1'
    if re.match(r'^-?[0-9]+$', input) : return '0'
    expr = re.findall('\\w+|[*/^+-]|\\(.*\\)', input[1:-1])
    operator = '+-*/^'

    if expr[0] in operator :
        f1, f2 = expr[1], expr[2]
        d1, d2 = diff(expr[1]), diff(expr[2])

        match expr[0] :
            case '+' : return add(diff(expr[1]), diff(expr[2]))
            case '-' : return sub(diff(expr[1]), diff(expr[2]))
            case '*' : return add( mul(f1,d2), mul(f2,d1) )
            case '^' : return mul(f2, exp(f1, sub(f2,'1')))
            case '/' :
                num, den = sub(mul(d1,f2), mul(d2,f1)), exp(f2,'2')
                return div(num, den)
    else :
        fx, dx = expr[1], diff(expr[1])

        match expr[0] :
            case 'cos' : return mul(sub('0', dx), form('sin', fx))
            case 'sin' : return mul(dx, form('cos', fx))
            case 'exp' : return mul(dx, form('exp', fx))
            case 'ln' : return div(dx, fx)
            case 'tan' : 
                # tan : tan x => x' / (cos(x))^2
                return div(dx, exp(form('cos', fx), '2'))

dx1 = diff("(^ x 3)")
dx2 = diff(dx1) # "(* 3 (* 2 x))", "(* 6 x)"), 

print(dx1)
print(dx2)
