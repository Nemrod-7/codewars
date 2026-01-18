import re

table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1] }

RADICALS = ["meth",  "eth", "prop", "but", "pent", "hex", "hept", "oct", "non", "dec", "undec", "dodec", "tridec", "tetradec", "pentadec", "hexadec", "heptadec", "octadec", "nonadec"]
MULTIPLIERS = ["--","di", "tri", "tetra", "penta", "hexa", "hepta", "octa", "nona", "deca", "undeca", "dodeca", "trideca", "tetradeca", "pentadeca", "hexadeca", "heptadeca", "octadeca", "nonadeca"]

MISC = ['di','meth','an ', 'cyclo']
HALOGEN = ['fluoro','chloro','bromo','iodo']
PREFIX = ['hydroxy','mercapto','imino','oxo','formyl','amide']
SUFFIX  = ['ol','thiol','imine','one','al', 'amido','amine', 'phosphine','arsine']
HYDROCARBON = ['ane','ene', 'yne' , 'yl' ]
KEYWORDS = (HYDROCARBON + HALOGEN + PREFIX + SUFFIX + MISC)

def suffix(code) : # retreat each suffix which is not part of a main chain
    # source : [suffix, position, alkane, radical]
    # expect : [alkane, radical, suffix, position]
    i = 0
    while i < len(code):
        if code[i] in SUFFIX :
            code[i:i + 2],code[i + 2:i + 4] = code[i + 2:i + 4],code[i:i + 2] 
            i += 4
        i += 1

def tokenize (name) :
    name = name.replace('[', ' { ').replace(']', ' } ')
    name = name.replace('{', ' ] ').replace('}', ' [ ')
    name = name.replace('-', ' ')
    name = name.replace('thiol', ' thi@l ')
    for sub in KEYWORDS : name = name.replace(sub, ' ' + sub + ' ')
    # for sub in HYDROCARBON : name = name.replace(sub,' ' + sub + ' ')
    # for sub in HALOGEN : name = name.replace(sub, ' ' + sub + ' ')
    # for sub in SUFFIX : name = name.replace(sub, ' ' + sub + ' ')
    # for sub in PREFIX : name = name.replace(sub, ' ' + sub + ' ')
    # for sub in ['di','meth','an ', 'cyclo'] : name = name.replace(sub, ' ' + sub + ' ')
    name = name.replace('thi@l', ' thiol ')
    name = re.findall(r'\S+', name)
    name.reverse()
    suffix(name)
    return name



def parser(name) :
    code = tokenize(name)
    molecule = []

    print(code)

parser("pentan-2-ol")
parser("pentan-2-thiol")
parser("pentan-2-imine")
parser("pentan-2-one")
parser("methan-1-amine")
parser("methan-1-arsine")
parser("methan-1-phosphine")

parser("2-iminopentan-3-ol")



# al     => [ [0, 'C', [0]], [1, 'O', [0,0]] ]
# formyl => [[0,'C', [0]], [1, 'C', [3,2,2]], [2, 'O', [1,1]], [3, 'H', [1]]]
# amide  => [[0,'C',[]], [1,'N',[2,3]], [2,'H',[1]], [3,'H',[1]]]
# amido  => [[0,'C',[]], [1,'N',[2,3]], [2,'H',[1]], [3,'H',[1]]]
# carboxy => [[0,'C',[]], [1,'C',[2,2,3]], [2,'O',[1,1]], [3,'O',[1,4]], [4,'H',[3]]]


# parser("3-[1-hydroxy]methylpentan-1,4-diol")
# parser("5-methylhexan-2-ol")
#
# parser("diethylether")
#
# parser('1-[3-propyl]heptylcyclobutane')
# parser('1,2-di[1-ethyl-3-[2-methyl]propyl]heptylcyclobutane')
