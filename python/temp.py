import re

table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1], 'I':[1, 126.9] }

RADICALS = ["meth",  "eth",   "prop",   "but",      "pent",     "hex",     "hept",     "oct",     "non",    "dec",  "undec", "dodec", "tridec", "tetradec", "pentadec", "hexadec", "heptadec", "octadec", "nonadec"]
MULTIPLIERS = ["--","di",     "tri",     "tetra",     "penta",     "hexa",     "hepta",     "octa",     "nona",     "deca", "undeca", "dodeca", "trideca", "tetradeca", "pentadeca", "hexadeca", "heptadeca", "octadeca", "nonadeca"]

HYDROCARBON = ['ane','ene', 'yne' , 'yl' ]
HALOGEN = ['fluoro','chloro','bromo','iodo'] 

def prefix(text, arr) :
    max = [i for i in range(len(arr)) if arr[i] == text[:len(arr[i])] ]
    return [text, 1] if max == [] else [text[len(arr[max[-1]]):] , max[-1] + 1]

def search(text, arr) :
    max = [i for i in range(len(arr)) if arr[i] == text[:len(arr[i])] ]
    return 0 if max == [] else max[-1] + 1


branch = [0 for _ in range(10)]

name = '2,7-dicyclobutyl'
name = 'hex-1,4-diene'

name = '3-ethyl-2,5-dimethylhexane'

name = '2,3,5-trimethylhexane'
name = '2-methylbutane'
name = '1-fluoropentane'

for sub in HYDROCARBON : name = name.replace(sub, sub + ' ')
for sub in HALOGEN : name = name.replace(sub, sub + ' ')

name = re.findall(r'\S+', name)

for chain in name :
    print(chain, end = ' >> ')

    chain = chain.replace('-', ' ')
    chain = chain.replace('cyclo', ' cyclo ')

    position = []
    multipl, radical, cyclo = 1, 1, False

    token = re.findall(r'\S+', chain)

    for cell in token :
        num= re.findall(r'\d+', cell) 
        rad = search(cell, RADICALS)
        mul = search(cell, MULTIPLIERS)

        if cell == 'cyclo' : cyclo = True
        if num != [] : position = num
        if rad != 0 : radical = rad 
        if mul != 0 : multipl = mul

    # print(token)
    print(position, multipl, radical, cyclo)

# cell, radical = prefix(chain, RADICALS)
# cell, position = chain[chain.rfind('-') + 1:], re.findall(r'\d+', chain)
# cell, multipl = prefix(cell, MULTIPLIERS)
# cell, cyclo = prefix(cell, ['','cyclo'])

# alkyls : position - MULTIPLIERS - cyclo - RADICALS
# alkene : RADICALS - position - MULTIPLIERS

# if len(position) == multipl :
#     for pos in position :
#         branch[int(pos) - 1] = 9


