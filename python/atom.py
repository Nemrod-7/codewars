import re
#####################################################################################################
# Symbol:           H     B     C     N     O     F    Mg     P     S    Cl    Br      I            #
# Valence number:   1     3     4     3     2     1     2     3     2     1     1      1            #
# Atomic weight:  1.0  10.8  12.0  14.0  16.0  19.0  24.3  31.0  32.1  35.5  80.0  126.9  (in g/mol)#
#####################################################################################################

order = ['C','H','O','B','Br','Cl','F','Mg','N','P','S']
table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1] }

RADICALS = ["meth",  "eth",   "prop",   "but",      "pent",     "hex",     "hept",     "oct",     "non",    "dec",  "undec", "dodec", "tridec", "tetradec", "pentadec", "hexadec", "heptadec", "octadec", "nonadec"]
MULTIPLIERS = ["--","di",     "tri",     "tetra",     "penta",     "hexa",     "hepta",     "octa",     "nona",     "deca", "undeca", "dodeca", "trideca", "tetradeca", "pentadeca", "hexadeca", "heptadeca", "octadeca", "nonadeca"]
HYDROCARBON = ['ane','ene', 'yne' , 'yl' ] # => ['C']
HALOGEN = ['fluoro','chloro','bromo','iodo'] # => ['F','Cl','Br','I']


def prefix(text, arr) :
    max = [i for i in range(len(arr)) if arr[i] == text[:len(arr[i])] ]
    return [text, 1] if max == [] else [text[len(arr[max[-1]]):] , max[-1] + 1]

def bond (a, b) :
    a[1].append(b[0])
    b[1].append(a[0])

def brancher(radical) :
    branch = [['C', []] for _ in range(radical)]

    for i in range(1, len(branch)) :
        bond(branch[i-1], branch[i-0])
    # print(branch)
    return branch

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
    if chain[-6:] == 'fluoro' or chain[-6:] == 'chloro' or chain[-5:] == 'bromo' or chain[-4:] == 'iodo' : return 'halogen'
    return ''

def tokenize(name) :
    # name = name.replace('cyclo', ' cyclo ')
    for sub in HALOGEN : name = name.replace(sub, sub + ' ')
    for sub in HYDROCARBON : name = name.replace(sub, sub + ' ')
    return re.findall(r"\S+", name)

def parser(name) :
    arms = []
    atoms = []
    formula = ''
    token = tokenize(name)
    token.reverse()
    print(token)

    for chain in token :
        type = identify(chain)
        # print(chain, type)
        match type :
            case 'alkane' : # simple bound between carbons. CnH2n+2 => radical + "ane"
                name = chain.replace('cyclo', ' cyclo ')
                name = re.findall(r"\S+", name)
                cyclo = false

                for ident in token :
                    if ident == 'cyclo' :
                        cyclo = true
                    else :



                chain, radical = prefix(chain, RADICALS)
                branch = brancher(radical)
                if 'cyclo' in chain : bond(branch[0], branch[-1])

                arms.append(branch)
            case 'alkyl' : # positions + "-" + multiplier + radical + "yl"
                chain, position = chain[chain.rfind('-') + 1:], re.findall( r'\d+' , chain)
                chain, multipl = prefix(chain, MULTIPLIERS)
                chain, radical = prefix(chain, RADICALS)
                main = arms[0];

                for nb in range(multipl) :
                    branch = brancher(radical)
                    if 'cyclo' in chain : bond(branch[0], branch[-1])

                    index = int(position[i]) - 1
                    bond(main[index], branch[0])
                    arms.append(branch)

            case 'alkene' : # double bound between carbons. CnH2n   => radical + "-" + positions + "-" + multiplier + "ene"
                chain, radical = prefix(chain, RADICALS)
                chain, position = chain[chain.rfind('-') + 1:], re.findall( r'\d+' , chain)
                chain, multipl = prefix(chain, MULTIPLIERS)

                for i in range(multipl) :
                    branch = brancher(radical)
                    link = arms[0][int(position[i]) - 1]
                    for _ in range(2) : bond(link, branch[0])

                    arms.append(branch)
            case 'alkyne' : # triple bound between carbons. CnH2n-2 => radical + "-" + positions + "-" + multiplier + "yne"
                chain, radical = prefix(chain, RADICALS)
                chain, position = chain[chain.rfind('-') + 1:], re.findall( r'\d+' , chain)
                chain, multipl = prefix(chain, MULTIPLIERS)

                for i in range(multipl) :
                    branch = brancher(radical)
                    link = arms[0][int(position[i]) - 1]
                    for _ in range(3) : bond(link, branch[0])

                    arms.append(branch)
            case 'halogen' :
                atom, position = chain[chain.rfind('-') + 1:], re.findall( r'\d+' , chain)
                main = arms[0]

                for i in range(len(position)) :
                    match atom :
                        case 'fluoro' : branch = [['F',[]]]
                        case 'chloro' : branch = [['Cl',[]]]
                        case 'bromo' : branch = [['Br',[]]]
                        case 'iodo' : branch = [['I',[]]]

                    index = int(position[i]) - 1
                    bond(main[index], branch[0])
                    arms.append(branch)

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

            while len(atom[1]) < valence : atom[1].append('H')

            print(atom)
            hist = { x: atom[1].count(x) for x in set(atom[1]) if table[x][0] == 1 }
            formula += element + ''.join(elt + str(hist[elt]) if hist[elt] != 1 else elt for elt in hist) + '-'

        print()
    formula = formula[:-1]
    print(formula)
#
# parser('methane')
# parser('ethane')
# parser('propane')
# parser('butane')

# parser('2-methylbutane')
# parser('2,3,5-trimethylhexane')
# parser('3-ethyl-2,5-dimethylhexane')

# parser('1-fluoropentane')


parser('cyclobutane')
# parser('1-ethylcyclobutane')
# parser('3,7-dicyclobutyloctane')

# parser('1,2-di[1-ethyl-3-[2-methyl]propyl]heptylcyclobutane')
# parser('1-[3-propyl]heptylcyclobutane')



# for chain in token : print(chain, end=' ')
