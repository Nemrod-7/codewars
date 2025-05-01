import re

table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1], 'I':[1, 126.9] }

RADICALS = ["meth",  "eth",   "prop",   "but",      "pent",     "hex",     "hept",     "oct",     "non",    "dec",  "undec", "dodec", "tridec", "tetradec", "pentadec", "hexadec", "heptadec", "octadec", "nonadec"]
MULTIPLIERS = ["--","di",     "tri",     "tetra",     "penta",     "hexa",     "hepta",     "octa",     "nona",     "deca", "undeca", "dodeca", "trideca", "tetradeca", "pentadeca", "hexadeca", "heptadeca", "octadeca", "nonadeca"]

HYDROCARBON = ['ane','ene', 'yne' , 'yl' ]
HALOGEN = {'fluoro':'F','chloro':'Cl','bromo':'Br','iodo':'I'} 

class Atom :
    def __init__(self, element) :
        self.element = element
        self.edge = []

def prefix(text, arr) :
    max = [i for i in range(len(arr)) if arr[i] == text[:len(arr[i])] ]
    return [text, 1] if max == [] else [text[len(arr[max[-1]]):] , max[-1] + 1]


thiol = ['S', []]
imine = ['N', []] # double bond to carbon
alcool = ['O', []]

name = '3-ethyl-2,5-dimethylhexane'
name = name.replace('cyclo', ' ' + 'cyclo'  + ' ')
# name = name.replace('-', )

chain = re.findall(r'-?[0-9]+-?,?[0-9]+-?|\w+', name)


print(chain)

