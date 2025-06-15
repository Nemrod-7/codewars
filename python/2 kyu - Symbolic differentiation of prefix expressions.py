import re

def is_number (x) :
    try :
        num = float(x)
        return True
    except :
        return False

def trunc(fx) : 
    return str(fx).rstrip('0.')

def add (a, b) :
    if is_number(a) and is_number(b) : return trunc(float(a) + float(b))
    if a == '0' : return b
    if b == '0' : return a
    return f'(+ {a} {b})'
def sub (a, b) :
    if is_number(a) and is_number(b) : return trunc(float(a) - float(b))
    if b == '0' : return a
    return f'(- {a} {b})'

def mul (a, b) :
    if is_number(a) and is_number(b) : return trunc(float(a) * float(b))
    if a == '0' or b == '0' : return '0'
    if a == '1' : return b
    if b == '1' : return a
    return f'(* {a} {b})'

def div (a, b) :
    if is_number(a) and is_number(b) : return trunc(float(a) / float(b))
    if b == '1' : return a
    return f'(/ {a} {b})'

def exp (a, b) :
    if is_number(a) and is_number(b) : return trunc(float(a) ** float(b))
    if b == '1' : return a
    if a == '1' or b == '0' : return '1'
    return f'(^ {a} {b})'

def deriv (input) : 
    # input  : expression in token form
    # return : [fx, dx] (ie : fx = simplified expression, derivation)

    if input == 'x' : return [input,'1']
    if re.match(r'^-?[0-9]+$', input) : return [input, '0']
    expr = re.findall('-?\\w+|[*/^+-]|\\(.*\\)', input[1:-1])
    operator = '+-*/^'

    fx, dx = deriv(expr[1])

    if expr[0] in operator :
        fy, dy = deriv(expr[2])

        match expr[0] :
            case '+' : return [add(fx, fy), add(dx, dy)]
            case '-' : return [sub(fx, fy), sub(dx, dy)]
            case '*' : return [mul(fx, fy), add( mul(fx,dy), mul(fy,dx) )]
            case '^' : 
                ex = exp(fx, sub(fy,'1'))
                return [exp(fx, fy), mul(dx, mul(fy, ex))]
            case '/' :
                num, den = sub(mul(dx,fy), mul(dy,fx)), exp(fy,'2')
                return [div(fx, fy), div(num, den)]
    else :
        match expr[0] :
            case 'ln' : return [ f'({expr[0]} {fx})', div(dx, fx)]
            case 'sin' : return [ f'({expr[0]} {fx})', mul(dx, f'(cos {fx})') ]
            case 'exp' : return [ f'({expr[0]} {fx})', mul(dx, f'(exp {fx})') ]
            case 'cos' : return [ f'({expr[0]} {fx})', mul(sub('0', dx), f'(sin {fx})') ]
            case 'tan' : return [ f'({expr[0]} {fx})', div(dx, exp(f'(cos {fx})', '2')) ]

    return ['','']

def diff (input) : return deriv(input)[1]

fx, dx = deriv('(cos (+ x -10))')
# print(dx)
