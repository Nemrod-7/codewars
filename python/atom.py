import re
##############################################################################################
# Symbol:           H     B     C     N     O     F    Mg     P     S    Cl    Br            #
# Valence number:   1     3     4     3     2     1     2     3     2     1     1            #
# Atomic weight:  1.0  10.8  12.0  14.0  16.0  19.0  24.3  31.0  32.1  35.5  80.0  (in g/mol)#
##############################################################################################

for _ in range(3): print()

order = ['C','H','O','B','Br','Cl','F','Mg','N','P','S']
table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1] }

RADICALS = ["meth",  "eth",   "prop",   "but",      "pent",     "hex",     "hept",     "oct",     "non",    "dec",  "undec", "dodec", "tridec", "tetradec", "pentadec", "hexadec", "heptadec", "octadec", "nonadec"]
MULTIPLIERS = ["--","di",     "tri",     "tetra",     "penta",     "hexa",     "hepta",     "octa",     "nona",     "deca", "undeca", "dodeca", "trideca", "tetradeca", "pentadeca", "hexadeca", "heptadeca", "octadeca", "nonadeca"]
HYDROCARBON = ['ane','ene', 'yne' , 'yl' ] # => ['C']
HALOGEN = ['fluoro','chloro','bromo','iodo'] # => ['F','Cl','Br','I']

def isnumber(str) :
    try :
        num = int(str)
        return num
    except :
        return False

def prefix(text, arr) :
    max = [i for i in range(len(arr)) if arr[i] == text[:len(arr[i])] ]
    return [text, 1] if max == [] else [text[len(arr[max[-1]]):] , max[-1] + 1]

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

# name = 'butane'
name = '2-methylbutane'
# name = '2,3,5-trimethylhexane'
# name = '1-fluoropentane'
# name = 'heptylcyclobutane'
# name = '3-ethyl-2,5-dimethylhexane'
# name = 'tridec-4,10-dien-2,6,8-triyne'
# name = '1,2-di[1-ethyl-3-[2-methyl]propyl]heptylcyclobutane'

name = name.replace('cyclo', ' ' + 'cyclo'  + ' ')
for sub in HALOGEN : name = name.replace(sub, sub + ' ')
for sub in HYDROCARBON : name = name.replace(sub, sub + ' ')

# "[0-9]+|[\[\],-]|[a-z]+"
token  = re.findall(r"\S+", name)
token.reverse()

arms = []

for cell in token :
    type = identify(cell)
    print(cell)

    match type :
        case 'alkane' : # simple bound between carbons. CnH2n+2 => radical + "ane"
            cell, radical = prefix(cell, RADICALS)
            branch = [['C', []] for _ in range(radical)]

            for i in range(1, len(branch)) :
                branch[i-1][1].append('C')
                branch[i-0][1].append('C')

            arms.append(branch)
        case 'alkyl' : # positions + "-" + multiplier + radical + "yl"
            cell, position = cell[cell.rfind('-') + 1:], re.findall( r'\w+' , cell[:cell.rfind('-')])
            cell, multipl = prefix(cell, MULTIPLIERS)
            cell, radical = prefix(cell, RADICALS)
            branch = [['C', []] for _ in range(radical)]

        case 'alkene' : # double bound between carbons. CnH2n   => radical + "-" + positions + "-" + multiplier + "ene"            pos = cell.rfind('-')
            cell, radical = prefix(cell, RADICALS)
            cell, position = cell[cell.rfind('-') + 1:], re.findall( r'\w+' , cell[:cell.rfind('-')])
            cell, multipl = prefix(cell, MULTIPLIERS)
            branch = [['C', []] for _ in range(radical)]

        case 'alkyne' : # triple bound between carbons. CnH2n-2 => radical + "-" + positions + "-" + multiplier + "yne"
            cell, radical = prefix(cell, RADICALS)
            cell, position = cell[cell.rfind('-') + 1:], re.findall( r'\w+' , cell[:cell.rfind('-')])
            cell, multipl = prefix(cell, MULTIPLIERS)
            branch = [['C', []] for _ in range(radical)]

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
        print(atom)



# for cell in token : print(cell, end=' ')
