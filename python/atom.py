import re
for _ in range(3) : print()

table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1], 'I':[1, 0.0], 'As':[3, 0.0] }

MISC = ['di','meth','an ', 'cyclo']
PREFIX = ['hydroxy','mercapto','imino','oxo','formyl','amide', 'fluoro','chloro','bromo','iodo']
SUFFIX  = ['ol','thiol','imine','one','al', 'amido','amine', 'phosphine','arsine']
HYDROCARBON = ['ane','ene', 'yne' , 'yl' ]
KEYWORDS = (HYDROCARBON + PREFIX + SUFFIX + MISC)

def get_rad(code, index) :
    while index < len(code) :
        if code[index] in RADICALS :
            return (RADICALS.index(code[index]) + 1, index)
        index += 1
    return 0,0

def get_pos(code, index) :
    while index < len(code) :
        if re.findall(r'\d+', code[index]) :
            return re.findall(r'\d+', code[index]), index
        index += 1

    return [],0

def suffix(code) : # retreat each suffix which is not part of a main chain
    # source : [suffix, (multiplier), (positions), alkane, radical]
    # expect : [alkane, radical, suffix, (multiplier), (positions)]
    i = 0

    while i < len(code):
        if code[i] in SUFFIX :
            _, end = get_rad(code, i)
            rad = i + end + 1
            pos = rad - 2
            code[i:rad] = code[pos : rad] + code[i : pos]
            i += end
        i += 1

def tokenize (name) :
    name = name.replace('[', ' { ').replace(']', ' } ')
    name = name.replace('{', ' ] ').replace('}', ' [ ')
    name = name.replace('-', ' ')
    name = name.replace('thiol', ' thi@l ')
    for sub in KEYWORDS : name = name.replace(sub, ' ' + sub + ' ')
    name = name.replace('thi@l', ' thiol ')
    name = re.findall(r'\S+', name)
    name.reverse()
    suffix(name)
    return name


def parser(name) :
    code = tokenize(name)

# source : [suffix, position, alkane, radical]
# expect : [alkane, radical, suffix, position]
#          [ol, di, 1,4, an, pent]

parser("fluoropentan-4-ol")
parser("5-fluoropentanol")
parser("fluoropentan-1,4-diol")
