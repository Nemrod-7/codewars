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

    return branch

def identify(name) :
    if name[-3:] == 'ane' : return 'alkane'
    elif name[-3:] == 'ene' : return 'alkene'
    elif name[-3:] == 'yne' : return 'alkyne'
    elif name[-2:] == 'yl' : return 'alkyl'
    elif name[:8] == 'hydroxy' or name[-2:] == 'ol' : return 'alcool'
    elif name[:8] == 'mercapto' or name[-5:] == 'thiol' : return 'thiol'
    elif name[:5] == 'imino' or name[-5:] == 'imine' : return 'imine'
    elif name[:3] == 'oxo' or name[-3:] == 'one' : return 'ketone'
    elif name[:5] == 'amido' or name[-5:] == 'amide' : return 'amide'
    elif name[:6] == 'formyl' or name[-2:] == 'al': return 'aldehyde'
    elif name[-5:] == 'acid' : return 'carboxylic acid'
    if name[-6:] == 'fluoro' or name[-6:] == 'chloro' or name[-5:] == 'bromo' or name[-4:] == 'iodo' : return 'halogen'
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

    for name in token :
        type = identify(name)
        # print(name, type)
        match type :
            case 'alkane' : # simple bound between carbons. CnH2n+2 => radical + "ane"
                name = name.replace('cyclo', ' cyclo ')
                name = re.findall(r"\S+", name)
                cyclo = False
                
                for ident in name :
                    if ident == 'cyclo' :
                        cyclo = True
                    else :
                        name, radical = prefix(ident, RADICALS)
                        branch = brancher(radical)
                        if cyclo : bond(branch[0], branch[-1])
                        arms.append(branch)
            case 'alkyl' : # positions + "-" + multiplier + radical + "yl"
                name = name.replace('cyclo', ' cyclo ')
                name = re.findall(r"\S+", name)
                # cyclo = False
                print(name)

                # for ident in name :
                #     if ident == 'cyclo' :
                #         cyclo = True
                #     else :
                #         ident, position = ident[ident.rfind('-') + 1:], re.findall( r'\d+' , ident)
                #         ident, multipl = prefix(ident, MULTIPLIERS)
                #         ident, radical = prefix(ident, RADICALS)
                #         main = arms[0];
                #
                #         for i in range(multipl) :
                #             branch = brancher(radical)
                #             if cyclo : bond(branch[0], branch[-1])
                #
                #             index = int(position[i]) - 1
                #             bond(main[index], branch[0])
                #             arms.append(branch)
            case 'alkene' : # double bound between carbons. CnH2n   => radical + "-" + positions + "-" + multiplier + "ene"
                name, radical = prefix(name, RADICALS)
                name, position = name[name.rfind('-') + 1:], re.findall( r'\d+' , name)
                name, multipl = prefix(name, MULTIPLIERS)

                for i in range(multipl) :
                    branch = brancher(radical)
                    link = arms[0][int(position[i]) - 1]
                    for _ in range(2) : bond(link, branch[0])

                    arms.append(branch)
            case 'alkyne' : # triple bound between carbons. CnH2n-2 => radical + "-" + positions + "-" + multiplier + "yne"
                name, radical = prefix(name, RADICALS)
                name, position = name[name.rfind('-') + 1:], re.findall( r'\d+' , name)
                name, multipl = prefix(name, MULTIPLIERS)

                for i in range(multipl) :
                    branch = brancher(radical)
                    link = arms[0][int(position[i]) - 1]
                    for _ in range(3) : bond(link, branch[0])

                    arms.append(branch)
            case 'halogen' :
                atom, position = name[name.rfind('-') + 1:], re.findall( r'\d+' , name)
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

# parser('methane')
# parser('ethane')
# parser('propane')
# parser('butane')

# parser('2-methylbutane')
# parser('2,3,5-trimethylhexane')
# parser('3-ethyl-2,5-dimethylhexane')

parser('1-fluoropentane')

# parser('cyclobutane')
# parser('1-ethylcyclobutane')
parser('3,7-dicyclobutyloctane')

# parser('1,2-di[1-ethyl-3-[2-methyl]propyl]heptylcyclobutane')
# parser('1-[3-propyl]heptylcyclobutane')



# for name in token : print(name, end=' ')
