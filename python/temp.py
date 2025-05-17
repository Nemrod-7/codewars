import re

table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1], 'I':[1, 126.9] }

RADICALS = ["--","meth",  "eth", "prop", "but", "pent", "hex", "hept", "oct", "non", "dec", "undec", "dodec", "tridec", "tetradec", "pentadec", "hexadec", "heptadec", "octadec", "nonadec"]
MULTIPLIERS = ["--","--","di", "tri", "tetra", "penta", "hexa", "hepta", "octa", "nona", "deca", "undeca", "dodeca", "trideca", "tetradeca", "pentadeca", "hexadeca", "heptadeca", "octadeca", "nonadeca"]

HYDROCARBON = ['ane','ene', 'yne' , 'yl' ]
HALOGEN = ['fluoro','chloro','bromo','iodo']

# radical + "ane"
# radical + "-" + positions + "-" + multiplier + "ene"
# radical + "-" + positions + "-" + multiplier + "yne"
# positions + "-" + multiplier + "cyclo" + [" + subramification + "]" + radical + "yl"

def prefix(text, arr) :
    max = [i for i in range(len(arr)) if arr[i] == text[:len(arr[i])] ]
    return [text, 0] if max == [] else [text[len(arr[max[-1]]):] , max[-1]]

def search(text, arr) :
    max = [i for i in range(len(arr)) if arr[i] == text[:len(arr[i])] ]
    return 0 if max == [] else max[-1] + 1

def identify(chain) :
    if chain[-3:] == 'ane' : return 'alkane'
    elif chain[-3:] == 'ene' : return 'alkene'
    elif chain[-3:] == 'yne' : return 'alkyne'
    elif chain[-2:] == 'yl' : return 'alkyl'
    elif chain[:8] == 'hydroxy' or chain[-2:] == 'ol' : return 'alcool'
    elif chain[:8] == 'mercapto' or chain[-5:] == 'thiol' : return 'thiol'
    elif chain[:5] == 'imino' or chain[-5:] == 'imine' : return 'imine'
    elif chain[:3] == 'oxo' or chain[-3:] == 'one' : return 'ketone'
    elif chain[:5] == 'amido' or chain[-5:] == 'amide' : return 'amide'
    elif chain[:6] == 'formyl' or chain[-2:] == 'al': return 'aldehyde'
    elif chain[-5:] == 'acid' : return 'carboxylic acid'
    if chain == 'fluoro' or chain == 'chloro' or chain == 'bromo' or chain == 'iodo' : return 'halogen'

    return ''

def bond (a, b) :
    a[1].append(b[0])
    b[1].append(b[0])

def brancher(radical) :

    branch = [['C', []] for _ in range(radical)]

    for i in range(1, len(branch)) :
        bond(branch[i-1], branch[i-0])

    return branch

name = 'hex-1,4-diene'
name = '2-methylbutane'
name = '1-heptylcyclobutane'
name = '2,7-dicyclobutyl'
name = '3-ethyl-2,5-dimethylhexane'

# positions + "-" + multiplier + "cyclo" + [" + subramification + "]" + radical + "yl"
name = '1-[3-propyl]heptylcyclobutane'
name = '1,2-di[1-ethyl-3-[2-methyl]propyl]heptylcyclobutane'

name = '1-fluoropentane'
name = '2,3,5-triethylhexane'

name = name.replace('cyclo', ' cyclo ')
name = name.replace('[', ' [ ').replace(']', ' ] ')
name = name.replace('-', ' ')
for sub in HYDROCARBON : name = name.replace(sub, sub + ' ')
for sub in HALOGEN : name = name.replace(sub, sub + ' ')
name = re.findall(r'\S+', name)

index = 0
position = []
molecule = []

while index < len(name) :
    pos = re.findall(r'\d+', name[index])

    if pos != [] :
        # if len(pos) > 1 : index += 1
        position.append(pos)
    elif name[index] == '[' :
        pass
    elif name[index] == ']' :
        pass
    else :
        molecule.append(name[index])
    index += 1

arms = []

while molecule :
    cell = molecule.pop()
    type = identify(cell)

    # print(cell, '=> ', type)
    match identify(cell) :
        case 'alkane' : # simple bound between carbons. CnH2n+2 => radical + "ane"
            cell, radical = prefix(cell, RADICALS)
            branch = brancher(radical)

            arms.append(branch)
        case 'alkyl' : # positions + "-" + multiplier + radical + "yl"
            pos = position.pop()
            cell, multipl = prefix(cell, MULTIPLIERS)
            cell, radical = prefix(cell, RADICALS)

            for nb in pos :
                branch = brancher(radical)
                link = arms[0][int(nb)]
                
                bond(link, branch[-1])
                arms.append(branch)

        case 'alkene' : # double bound between carbons. CnH2n   => radical + "-" + positions + "-" + multiplier + "ene"
            cell, radical = prefix(cell, RADICALS)
            cell, position = cell[cell.rfind('-') + 1:], re.findall( r'\d+' , cell)
            cell, multipl = prefix(cell, MULTIPLIERS)

            branch = brancher(radical)

        case 'alkyne' : # triple bound between carbons. CnH2n-2 => radical + "-" + positions + "-" + multiplier + "yne"
            cell, radical = prefix(cell, RADICALS)
            cell, position = cell[cell.rfind('-') + 1:], re.findall( r'\d+' , cell)
            cell, multipl = prefix(cell, MULTIPLIERS)

            branch = brancher(radical)

        case 'alcool' :
            elt = 'OH'
        case 'thiol'  :
            elt = 'SH'
        case 'imine'  :
            elt = 'NH'
        case 'ketone' :
            elt += 'O'
        case 'amide'  :
            pass
        case 'aldehyde' :
            pass


for branch in arms :
    for atom in branch :
        element = atom[0]
        valence = table[element][0]

        # while len(atom[1]) < valence : atom[1].append('H')
        # print(atom)

###################################################################################################
# def detect(token) :
#     # print(chain, end = ' >> ')
#     # chain = chain.replace('-', ' ')
#     # chain = chain.replace('cyclo', ' cyclo ')
#     #
#     # token = re.findall(r'\S+', chain)
#     position = []
#     multipl, radical, cyclo = 1, 1, True if 'cyclo' in token else False
#
#     for cell in token :
#         num, rad, mul = re.findall(r'\d+', cell), search(cell, RADICALS), search(cell, MULTIPLIERS)
#
#         if num != [] : position = num
#         if rad != 0 : radical = rad
#         if mul != 0 : multipl = mul
#
#     return position, multipl, radical, cyclo

# index = 0
# chain = []
# while index < len(name) :
#     chain.append( name[index] )
#
#     if name[index] == 'ane' :
#         position, multipl, radical, cyclo = detect(chain)
#         # print(chain, end=' >> ')
#         # print(position, multipl, radical, cyclo)
#         chain = []
#     elif name[index] == 'yl' :
#         position, multipl, radical, cyclo = detect(chain)
#         # print(chain, end=' >> ')
#         # print(position, multipl, radical, cyclo)
#         chain = []
#     elif name[index] in HALOGEN:
#         print(chain)
#         chain = []
#
#     index += 1
