from math import comb

def getterm(str) :
    num = ''.join([ch for ch in str if ch.isdigit()])
    var = ''.join([ch for ch in str if ch.isalpha()])
    sign = -1 if '-' in str else 1
    value = 1 if num == '' else int(num)
    return (value * sign, var)

def formexponent (var, ex) :
    if ex == 0 : return ''
    if ex == 1 : return var
    return var + '^' + str(ex)

def expand(expr):
    if expr.count('^') != 1: return '-' + expr
    res = ''
    expr = expr.split('^')
    term, exp = expr[0], int(expr[1])
    # print(mid, term, expr[mid+1:])
    for i in range(len(term) -1, 0, -1) :
        if term[i] == '+' or term[i] == '-' :
            v0 = getterm(term[:i]), getterm(term[i:])
            break
    # print('[', v0, ']')
    for i in range(exp + 1) :
        sig = [exp - i, i]
        term, mult = '', comb(exp, i)

        for j in range(2) :
            mult *= pow(v0[j][0], sig[j])
            if v0[j][1] != '' :
                term += formexponent(v0[j][1], sig[j])

        if mult != 0 :
            if i == 0 and mult == -1 :
                res += '-'
            if i > 0 and mult > 0 :
                res += '+'
            if mult > 1 or mult < -1 or term == '' :
                res += str(mult)
            res += term
        # print('[', mult, ']', '[', term, ']')
    return res

def assert_equals(a,b) :
    if a != b :
        print("error got : ", a, 'expect : ', b )


actual = expand('a^5-85a^4-2890a^3-49130a^2-417605a-1419857')
print(actual)
actual = expand('(-y+3)^1')


assert_equals(expand("x-4"), "-x-4")
assert_equals(expand("z^3+15z^2-75z+125"), "-z^3+15z^2-75z+125")

assert_equals(expand("(x+1)^0"), "1")

assert_equals(expand("(x-1)^0"), "1")
assert_equals(expand("(7x-7)^0"), "1")
assert_equals(expand("(-7x-7)^0"), "1")
assert_equals(expand("(x+1)^1"), "x+1")
assert_equals(expand("(x+1)^2"), "x^2+2x+1")

assert_equals(expand("(x-1)^1"), "x-1")
assert_equals(expand("(x-1)^2"), "x^2-2x+1")
assert_equals(expand("(5m+3)^4"), "625m^4+1500m^3+1350m^2+540m+81")

assert_equals(expand("(2x-3)^3"), "8x^3-36x^2+54x-27")
assert_equals(expand("(-2k-3)^3"), "-8k^3-36k^2-54k-27")

assert_equals(expand("(-5m+3)^4"), "625m^4-1500m^3+1350m^2-540m+81")
