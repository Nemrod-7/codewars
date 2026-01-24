import re

for _ in range(3) : print()

table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1], 'I':[1, 0.0], 'As':[3, 0.0] }

RADICALS = ["meth",  "eth", "prop", "but", "pent", "hex", "hept", "oct", "non", "dec", "undec", "dodec", "tridec", "tetradec", "pentadec", "hexadec", "heptadec", "octadec", "nonadec"]
MULTIPLIERS = ["--","di", "tri", "tetra", "penta", "hexa", "hepta", "octa", "nona", "deca", "undeca", "dodeca", "trideca", "tetradeca", "pentadeca", "hexadeca", "heptadeca", "octadeca", "nonadeca"]

ALDEHYDES = ['amine', 'phosphine','arsine']
MISC = ['di','meth','an ', 'cyclo']
SUFFIX  = ['ol','thiol','imine','one','al', 'amido']
PREFIX = ['hydroxy','mercapto','imino','oxo','formyl','amide', 'fluoro','chloro','bromo','iodo']
HYDROCARBON = ['ane','ene', 'yne' , 'yl' ]
KEYWORDS = (HYDROCARBON + PREFIX + SUFFIX + MISC)

def identify_proto(name) :
    if re.findall(r'\d+', name) : return 'position'
    elif name == 'cyclo': return 'cycle'
    elif name in RADICALS : return 'radical'
    elif name in MULTIPLIERS : return 'multiplier'
    return 'element'

def get_rad(name, index) :
    while index < len(name) :
        if name[index] in RADICALS :
            return (RADICALS.index(name[index]) + 1, index)
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
    name = name.replace('formyl', ' formul ')
    for sub in KEYWORDS : name = name.replace(sub, ' ' + sub + ' ')
    name = name.replace('thi@l', ' thiol ')
    name = name.replace('formul', ' formyl ')
    name = re.findall(r'\S+', name)
    name.reverse()
    suffix(name)
    return name

def parser(name) :
    index = 0
    code = tokenize(name)
    molecule = []
    element = []
    print(code)

    while index < len(code) :
        if code[index] == '[' : pass
        if code[index] == ']' : pass
        type = identify_proto(code[index])

        if type == 'position' :
            pos = [int(it) for it in re.findall(r'\d+', code[index])]

        if code[index] == 'cyclo' :
            branch = molecule[-1]

        if type == 'element' :
            element = code[index]
            # print(code[index], ' => ' , code[index])

        if type == 'radical' :
            radical = RADICALS.index(code[index]) + 1

        if type == 'multiplier' :
            mul = MULTIPLIERS.index(code[index])

        index += 1

parser("ethyldimethylamine")
parser("diethylether")

parser("methylamine")
parser("methan-1-amine")

# parser("methylphosphine")
# parser("methan-1-phosphine")
# parser("methylarsine")
# parser("methan-1-arsine")
# parser("ethyldimethylamine") # fails
# parser("ethan-1-[dimethyl]amine")
# parser("hexan-1,6-diamine")
# parser("1,6-diaminohexan-3-ol")
# parser("1-amino-6-[diethyl]arsinohexan-3-ol")
