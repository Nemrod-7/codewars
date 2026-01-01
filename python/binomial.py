
def getterm(str) :
    num = ''.join([ch for ch in str if ch.isdigit()])
    value = 1 if num == '' else int(num)
    sign = -1 if '-' in str else 1
    var = ''.join([ch for ch in str if ch.isalpha()])

    return (value * sign, var)

def binomial(exp) :
    tri = [[1 for _ in range(exp + 1)] for _ in range(exp + 1)]

    for i in range(exp + 1) :
        for j in range(1, i) :
            tri[i][j] = tri[i-1][j] + tri[i-1][j-1]
    return tri

def formexponent (var, ex) :
    match ex :
        case 0 : return ''
        case 1 : return var

    return var + '^' + str(ex)

def expand(expr):
    mid = expr.find("^")
    exp = int(expr[mid+1:])
    term = expr[1:mid-1]
    bino = binomial(exp)
    res = ''
    # if exp == '0' : return '1'
    # if exp == '1' : return term
    for i in range(len(term)) :
        ch = term[i]
        if ch == '+' or ch == '-' :
            v0 = getterm(term[:i]), getterm(term[i:])
            break
    # print('[', v0, ']')

    for i in range(len(bino[exp])) :
        sig = [exp - i, i]
        mult, term = bino[exp][i], ''
#     let mul = (0..2).fold(binom[exp][i].clone(), |mul, x| mul * BigInt::from(v0[x].0).pow(sig[x] as u32));
#     let term = (0..2).filter(|&x| v0[x].1 != None).map(|x| formexponent(v0[x].1.unwrap(), sig[x])).collect::<String>();
        for j in range(2) :
            mult *= pow(v0[j][0], sig[j])
            if v0[j][1] != '' :
                term += formexponent(v0[j][1], sig[j])

        if mult != 0 :
            if i > 0 and mult > 0 :
                res += '+'
            if mult > 1 or mult < -1 :
                res += str(mult)
            elif term == '' :
                res += str(mult)

            res += term

        # print('[', mult, ']', '[', term, ']=>', res)
    #     print('[', mult, ']', '[', term, ']')
    # print(res)
    return res

def assert_equals(a,b) :
    if a != b :
        print("error got : ", a, 'expect : ', b )

assert_equals(expand("(x+1)^0"), "1")
assert_equals(expand("(x-1)^0"), "1")
assert_equals(expand("(7x-7)^0"), "1")
assert_equals(expand("(-7x-7)^0"), "1")
assert_equals(expand("(x+1)^1"), "x+1")
assert_equals(expand("(x+1)^2"), "x^2+2x+1")

assert_equals(expand("(x-1)^1"), "x-1")
assert_equals(expand("(x-1)^2"), "x^2-2x+1")
assert_equals(expand("(2x-3)^3"), "8x^3-36x^2+54x-27")
assert_equals(expand("(5m+3)^4"), "625m^4+1500m^3+1350m^2+540m+81")

# assert_equals(expand("(-2k-3)^3"), "-8k^3-36k^2-54k-27")
#
# assert_equals(expand("(-5m+3)^4"), "625m^4-1500m^3+1350m^2-540m+81")
