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
    a[2].append(b[0])
    b[2].append(a[0])

def brancher(radical) :
    branch = [[0, 'C', []]] + [[i + 1, 'C', []] for i in range(radical)]

    for i in range(2, len(branch)) :
        bond(branch[i-1], branch[i-0])
    return branch

def reindex(branch, index) :
    for i in range(1, len(branch)) :
        nid = index + i
        pid = branch[i][0]

        for atom in branch :
            for j in range(len(atom[2])) :
                if atom[2][j] == pid : atom[2][j] = nid

        branch[i][0] = nid

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
    elif name[-3:] == 'ene' or name[-2:] == 'en' : return 'alkene'
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
################################################################################
def display(molecule) :
    for branch in molecule :
        for atom in branch :
            print(atom)
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

def represent(atom) :
    id, elt, edge = atom
    valence = table[elt][0]
    rest = valence - len(edge)
    cell = elt

    match rest :
       case 0 : pass
       case 1 : cell += 'H'
       case _ : cell += 'H' + str(rest)
    return cell

def smile(molecule) :
    # display
    width = max([len(branch) for branch in molecule])
    height = len(molecule)
    graph = [[ [] for _ in range(width) ] for _ in range(height)]
    formula = ''

    for y in range(len(molecule)) :
        branch = molecule[y]
        for i in range(1, len(branch)) :
            id, element, edge = branch[i]
            link = ''
            nb = [0] if i == 0 else [ 1 for j in range(len(edge)) if edge[j] == branch[i-1][0] ].count(1)

            match nb :
                case 0 : link += '  '
                case 1 : link += '-'
                case 2 : link += '='
                case 3 : link += '{=}'

            formula += link + represent(branch[i])
    return formula
################################################################################
def mainchain(code) :
    (maxv,inxv) = (0,0)

    for index in range( len(code) ) :
        name = code[index]
        pose = re.findall(r'\d+', name)

        if pose == [] and name != 'cyclo':
            radical = prefix(name, RADICALS)
            multipl = prefix(name, MULTIPLIERS)
            # print(name, '=>', radical, multipl, index )
            if multipl > radical :
                ii = len(MULTIPLIERS[multipl-1])
                name = name[ii:]
                radical = prefix(name, RADICALS)
            if radical >= maxv :
                (maxv,inxv) = (radical,index)
    return (maxv, inxv)

def ast(code, molecule) :
    index = 0

    while index < len(code) :
        pos = re.findall(r'\d+', code[index])
        # print(code[index], pos, len(molecule), end='\n')
        if code[index] == '[' :
            # print(' => sub')
            next = getsub(code, index + 1)
            # parser(code[index + 1 : next])
            index = next
        if code[index] == 'cyclo' :
            # print(' => cycle')
            branch = molecule[-1]
            bond(branch[1], branch[-1])

            # display(branch)
        elif pos != [] and len(molecule) > 1 :
            # print(' => position')
            ix = max([atom[0] for branch in molecule for atom in branch])
            alky = molecule.pop()
            main = molecule.pop()
            # display(main)
            # display(alky)
            for p in pos :
                branch = [ [at[0], at[1], [edge for edge in at[2]]] for at in alky ] # because python is unable to make a deep copy !
                reindex(branch, ix)
                bond(branch[1], main[int(p)])
                ix += len(alky) - 1

                for i in range(1, len(branch)) :
                    main.append(branch[i])

            molecule.append(main)
        else :
            type = identify(code[index])

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
                    branch = molecule[-1]
                    position = re.findall(r'\d+', code[index + 1])

                    for pos in position :
                        bond(branch[int(pos)], branch[int(pos) + 1])
                    index += 1
                case 'alkyne' : # triple bound
                    branch = molecule[-1]
                    position = re.findall(r'\d+', code[index + 1])
                    # print(code[index], " => " , identify(code[index]))

                    for pos in position :
                        bond(branch[int(pos)], branch[int(pos) + 1])
                        bond(branch[int(pos)], branch[int(pos) + 1])
                    index += 1
                case 'halogen' :
                    match code[index] :
                        case 'iodo' :
                            molecule.append([[0,'C',[]], [1,'I',[]] ])
                        case 'bromo' :
                            molecule.append([[0,'C',[]], [1,'Br',[]] ])
                        case 'fluoro' :
                            molecule.append([[0,'C',[]], [1,'F',[]] ])
                        case 'chloro' :
                            molecule.append([[0,'C',[]], [1,'Cl',[]] ])
                case 'alcool' :
                    radical = prefix(code[index + 2], RADICALS)
                    branch = brancher(radical)
                    position = re.findall(r'\d+', code[index + 1])

                    molecule.append([ [0,'C',[]], [1,'O',[2]], [2,'H',[1]]  ])
                    index += 3
                case 'thiol'  :
                    molecule.append([ [0,'C',[]], [1,'S',[2]], [2,'H',[1]]  ])
                case 'imine'  : # double bond
                    # molecule.append([ [0,'C',[]], [1,'S',[2]], [2,'H',[1]]  ])
                    pass
                case 'ketone' :
                    pass
                case 'amide'  :
                    pass
                case 'aldehyde' :
                    pass
        index += 1

    return molecule
def parser(name) :
    code = tokenize(name)
    radical, index = mainchain(code)
    molecule = [brancher(radical)]
    code = [ code[it] for it in range(len(code)) if it != index  ]

    print(name, code)
    molecule = ast(code, molecule)

    display(molecule)
    print(smile(molecule))
    # print(formula(molecule))
    return []

# parser('methane')
# parser('ethane')
# parser('propane')
# parser('butane')
# parser('cyclobutane')
#
# parser('2-methylbutane')
# parser('2,3,5-trimethylhexane')
# parser('3-ethyl-2,5-dimethylhexane')
# parser('2,7-dicyclobutyloctane')
#
# parser("hex-2-ene")
# parser("hex-2-yne")
# parser("tridec-4,10-dien-2,6,8-triyne")
# parser("4-hept-2,4-dienylundec-9-en-2,5-diyne")

# parser('1-fluoropentane')
# parser("1,1-dibromo-4-chloropentane")
#
# parser("pentan-2-ol")
#
# parser("diethylether")
#
# parser('1-[3-propyl]heptylcyclobutane')
# parser('1,2-di[1-ethyl-3-[2-methyl]propyl]heptylcyclobutane')

# parser("5-methylhexan-2-ol")
