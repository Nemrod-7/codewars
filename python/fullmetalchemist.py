import re
#####################################################################################################
# Symbol:           H     B     C     N     O     F    Mg     P     S    Cl    Br      I            #
# Valence number:   1     3     4     3     2     1     2     3     2     1     1      1            #
# Atomic weight:  1.0  10.8  12.0  14.0  16.0  19.0  24.3  31.0  32.1  35.5  80.0  126.9  (in g/mol)#
#####################################################################################################

order = ['C','H','O','B','Br','Cl','F','Mg','N','P','S','I']
table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1], 'I':[1, 126.9] }

RADICALS = ["meth",  "eth", "prop", "but", "pent", "hex", "hept", "oct", "non", "dec", "undec", "dodec", "tridec", "tetradec", "pentadec", "hexadec", "heptadec", "octadec", "nonadec"]
MULTIPLIERS = ["--","di", "tri", "tetra", "penta", "hexa", "hepta", "octa", "nona", "deca", "undeca", "dodeca", "trideca", "tetradeca", "pentadeca", "hexadeca", "heptadeca", "octadeca", "nonadeca"]

HYDROCARBON = ['ane','ene', 'yne' , 'yl' ] # => ['C']
HALOGEN = ['fluoro','chloro','bromo','iodo'] # => ['F','Cl','Br','I']


def prefix(text, arr) :
    max = [i for i in range(len(arr)) if arr[i] == text[:len(arr[i])] ]
    return 1 if max == [] else max[-1] + 1
    # return [text, 1] if max == [] else [text[len(arr[max[-1]]):] , max[-1] + 1]
def suffix(text, arr) :
    max = [i for i in range(len(arr)) if arr[i] == text[len(arr[i]):] ]
    return 1 if max == [] else max[-1] + 1

def bond (a, b) :
    a[2].append(b)
    b[2].append(a)

def brancher(radical) :
    branch = [[i, 'C', []] for i in range(radical)]

    for i in range(1, len(branch)) :
        bond(branch[i-1], branch[i-0])
        pass
    return branch

def tokenize (name) :
    name = name.replace('cyclo', ' cyclo ')
    name = name.replace('[', ' { ').replace(']', ' } ')
    name = name.replace('{', ' ] ').replace('}', ' [ ')
    name = name.replace('-', ' ')
    for sub in HYDROCARBON : name = name.replace(sub, sub + ' ')
    for sub in HALOGEN : name = name.replace(sub, ' ' + sub + ' ')
    name = re.findall(r'\S+', name)
    name.reverse()
    return name

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

def getsub(name, index) :
    pile = 1

    while True :
        if name[index] == '[' : pile += 1
        if name[index] == ']' : pile -= 1

        if pile == 0 : return index
        index += 1

    return 0

def display(molecule) :

    for branch in molecule :
        for atom in branch :
            id = atom[0]
            edge = atom[2];
            element = atom[1]
            valence = table[element][0]

            hist = [at[1] for at in edge]
            while len(hist) < valence :
                hist.append('H')

            print(id, element, hist)
        print()

def show_atom(branch) : 
    for atom in branch :
        id, element, edge = atom
        hist = [ at[1] for at in edge ]
        print(id, element, hist)

def formula(molecule) :
    formula = ''
    hist = []

    for branch in molecule :
        for atom in branch :
            id,element,edge = atom
            valence = table[element][0]
            hist.append(element)
            
            for _ in range(valence - len(atom[2]) ) :
                hist.append('H')

    hist = { x: hist.count(x) for x in set(hist) }
    arr = [(key,hist[key]) for key in hist]
    arr.sort(key = lambda x: order.index(x[0]))
    
    for elt, nb in arr :
        formula += elt + (str(nb) if nb > 1 else '')
    # formula = [elt + str(hist[elt]) if hist[elt] != 1 else elt for elt in hist]
    return formula

def smile(molecule) :
    width = 5
    height = len(molecule)
    graph = [[ [] for _ in range(width) ] for _ in range(height)]

    branch = molecule[0]
    atom = branch[0]
    id, element, edge = atom
    valence = table[element][0]

    cell = graph[0][0]
    cell += element;
    rest = valence - len(edge)

    match rest :
       case 0 : pass
       case 1 : cell += 'H'
       case _ : cell += 'H' + str(rest)


    for i in range(1, len(branch)) :
        cell = graph[0][i]
        atom = branch[i]

        id, element, edge = atom
        valence = table[element][0]
        rest = valence - len(edge)
        nb = [ 1 for j in range(len(edge)) if edge[j][0] == branch[i-1][0] ].count(1)

        match nb :
            case 1 : cell += '-'
            case 2 : cell += '='
            case 3 : cell += '{=}'

        cell += element
        rest = valence - len(edge)

        match rest :
           case 0 : pass
           case 1 : cell += 'H'
           case _ : cell += 'H' + str(rest)

    for i in range(height) :
        for j in range(width) :
            print(graph[i][j], end='')
        print()

def parser(name) :
    index = 0
    position = []
    molecule = []
    code = tokenize(name)

    print(name, code)
    while index < len(code) :
        pos = re.findall(r'\d+', code[index])
        # print(code[index], end='')

        if code[index] == '[' :
            # print(' => sub')
            next = getsub(code, index + 1)
            # parser(code[index + 1 : next])
            index = next
        if code[index] == 'cyclo' :
            # print(' => cycle')
            branch = molecule[-1]
            bond(branch[0], branch[-1])
        elif pos != [] and len(molecule) > 1 :
            # print(' => position')
            alkx = molecule.pop()
            main = molecule.pop()

            for i in range(len(pos)) :
                branch = [ (i, at[1], [edge for edge in at[2]]) for at in alkx ] # because python can't make a deep copy !
                bond(branch[0], main[ int(pos[i]) - 1 ])
                # show_atom(branch)
                molecule.append(branch)

            molecule.append(main)
        else :

            type = identify(code[index])
            # print(" => " , identify(code[index]))
            match type :
                case 'alkane' :
                    radical = prefix(code[index], RADICALS)
                    branch = brancher(radical)
                    molecule.append(branch)
                case 'alkyl' :
                    radical = prefix(code[index], RADICALS)
                    branch = brancher(radical)
                    molecule.append(branch)
                case 'alkene' : # double bound
                    radical = prefix(code[index + 2], RADICALS)
                    branch = brancher(radical)
                    position = re.findall(r'\d+', code[index + 1])

                    for pos in position :
                        bond(branch[int(pos) - 1], branch[int(pos)])
                    # show_atom(branch)
                    molecule.append(branch)
                    index += 3
                case 'alkyne' : # triple bound
                    radical = prefix(code[index + 2], RADICALS)
                    branch = brancher(radical)
                    position = re.findall(r'\d+', code[index + 1])

                    for pos in position :
                        bond(branch[int(pos) - 1], branch[int(pos)])
                        bond(branch[int(pos) - 1], branch[int(pos)])

                    molecule.append(branch)
                    index += 3
                case 'halogen' :
                    match code[index] :
                        case 'iodo' : molecule.append([ [0,'I',[]] ])
                        case 'bromo' : molecule.append([ [0,'Br',[]] ])
                        case 'fluoro' : molecule.append([ [0,'F',[]] ])
                        case 'chloro' : molecule.append([ [0,'Cl',[]] ])
                case 'alcool' :
                    radical = prefix(code[index + 2], RADICALS)
                    branch = brancher(radical)
                    position = re.findall(r'\d+', code[index + 1])

                    molecule.append([[0,'O',[]]])

                    index += 3
                case 'thiol'  :
                    molecule.append([[0,'SH',[]]])
                case 'imine'  : # double bond
                    molecule.append([[0,'NH',[]]])
                case 'ketone' :
                    molecule.append([[0,'O',[]]])
                case 'amide'  :
                    pass
                case 'aldehyde' :
                    pass

        index += 1

    smile(molecule)

    # display(molecule)
    # print(formula(molecule))

    return []

# parser('methane')
# parser('ethane')
# parser('propane')
parser('butane')
# parser('cyclobutane')
#
# parser('1-fluoropentane')
# parser('2-methylbutane')
# parser('2,3,5-trimethylhexane')
#
# parser('3-ethyl-2,5-dimethylhexane')
# parser('2,7-dicyclobutyloctane')
#
# parser("hex-2-yne")
# parser("1,1-dibromo-4-chloropentane")

# parser("pentan-2-ol")

# parser("diethylether")

# parser('1-[3-propyl]heptylcyclobutane')
# parser('1,2-di[1-ethyl-3-[2-methyl]propyl]heptylcyclobutane')

# parser("5-methylhexan-2-ol")
