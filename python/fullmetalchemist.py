import re
# for _ in range(3) : print()

table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1], 'I':[1, 0.0], 'As':[3, 0.0] }

RADICALS = ["meth",  "eth", "prop", "but", "pent", "hex", "hept", "oct", "non", "dec", "undec", "dodec", "tridec", "tetradec", "pentadec", "hexadec", "heptadec", "octadec", "nonadec"]
MULTIPLIERS = ["--","di", "tri", "tetra", "penta", "hexa", "hepta", "octa", "nona", "deca", "undeca", "dodeca", "trideca", "tetradeca", "pentadeca", "hexadeca", "heptadeca", "octadeca", "nonadeca"]

MISC = ['di','meth','an ', 'cyclo']
HALOGEN = ['fluoro','chloro','bromo','iodo']
PREFIX = ['hydroxy','mercapto','imino','oxo','formyl','amide']
SUFFIX  = ['ol','thiol','imine','one','al', 'amido','amine', 'phosphine','arsine']
HYDROCARBON = ['ane','ene', 'yne' , 'yl' ]
KEYWORDS = (HYDROCARBON + HALOGEN + PREFIX + SUFFIX + MISC)
# atom = [id, element, edge]
# branch = [ unused, atom1, atom2 ...] ie : [[0, 'C', [0]], [1, 'C', [2,2]], [2,'O', [1,1]], ...]
# the first atom (unused) is necessary for the 1-indexed position of all atoms, its edge indicates tthe number of bonds between this branch and another

def suffix(code) : # retreat each suffix which is not part of a main chain
    # source : [suffix, position, alkane, radical]
    # expect : [alkane, radical, suffix, position]
    i = 0
    while i < len(code):
        if code[i] in SUFFIX :
            ov = 2 if re.findall(r'\d+', code[i + 1]) else 1
            pos, rad = i + ov, i + 2 + ov
            code[i : pos],code[pos : rad] = code[pos : rad],code[i : pos] 
            i += (2 + ov)
        i += 1

def tokenize (name) :
    name = name.replace('[', ' { ').replace(']', ' } ')
    name = name.replace('{', ' ] ').replace('}', ' [ ')
    name = name.replace('-', ' ')
    name = name.replace('thiol', ' thi@l ')
    for sub in KEYWORDS : name = name.replace(sub, ' ' + sub + ' ')
    name = name.replace('thi@l', ' thiol ')
    name = re.findall(r'\S+', name)
    name.reverse()
    suffix(name)
    return name
def bond (a, b) :
    a[2].append(b[0])
    b[2].append(a[0])

def brancher(radical) :
    branch = [[0, 'C', [0]]] + [[i + 1, 'C', []] for i in range(radical)]

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

def represent(atom) :
    id, elt, edge = atom
    cell = elt
    rest = table[elt][0] - len(edge)

    match rest :
       case 0 : pass
       case 1 : cell += 'H'
       case _ : cell += 'H' + str(rest)
    return cell

def smile(molecule) :
    # display
    formula, links = '', ''

    for y in range(len(molecule)) :
        branch = molecule[y]
        # hist = set([it[0] for it in branch ])
        # print(hist)
        for i in range(1, len(branch)) :
            id, element, edge = branch[i]
            nb = [0] if i == 0 else [ 1 for j in range(len(edge)) if edge[j] == branch[i-1][0] ].count(1)

            link = ''
            match nb :
                case 0 : link += '  '
                case 1 : link += '-'
                case 2 : link += '='
                case 3 : link += '{=}'

            if i > 1 and i < len(branch) - 1 and edge[-1] != branch[i+1][0] and edge[-1] != branch[i-1][0] :
                hist = { edge.count(x):x for x in set(edge) }
                nn = '||' if 2 in hist else '|'
                links +=  len(link) * ' ' + nn + (len(represent(branch[i])) - len(nn)) * ' '
            else :
                links += len(link + represent(branch[i])) * ' '
            formula += link + represent(branch[i])

    return formula + '\n' + links

def adjency(molecule) :
    for y in range(len(molecule)) :
        branch = molecule[y]
        for i in range(1, len(branch)) :
            id, element, edge = branch[i]
            nb = [0] if i == 0 else [ 1 for j in range(len(edge)) if edge[j] == branch[i-1][0] ].count(1)
            if nb == 0 : print()
            print(branch[i])
        

def identify_proto(name) :
    if re.findall(r'\d+', name) : return 'position'
    elif name == 'cyclo': return 'cycle'
    elif name in RADICALS : return 'radical'
    elif name in MULTIPLIERS : return 'multiplier'
    return 'element'

def getarr(code, index, array) :
    while index < len(code) :
        if code[index] in array :
            return (array.index(code[index]) + 1, index)
        index += 1
    return (0,0)

def getpos(code, index) :
    while index < len(code) :
        if re.findall(r'\d+', code[index]) :
            return re.findall(r'\d+', code[index]), index
        index += 1
    return [],0

def position(molecule, branch, position) :
    ix = max([atom[0] for branch in molecule for atom in branch])
    main = molecule[-1]

    for p in position :
        reindex(branch, ix)
        ix += len(alky) - 1
        # the number of edges of the first atom of the branch indicates the number of bonds
        for _ in range(len(branch[0][2])) :
            bond(branch[1], main[int(p)])

        main += [branch[i] for i in range(1,len(branch))]

def ast(name) :
    code = tokenize(name)
    index = 0
    molecule = []

    print(code)
    while index < len(code) :
        name = code[index]
        pose = re.findall(r'\d+', name)

        if pose != [] :
            ix = max([atom[0] for branch in molecule for atom in branch])
            alky = molecule.pop()
            main = molecule.pop()

            for p in pose :
                branch = [ [at[0], at[1], [edge for edge in at[2]]] for at in alky ] # because python is unable to make a deep copy !
                reindex(branch, ix)
                ix += len(alky) - 1
                # the number of edges of the first atom of the branch indicates the number of bonds
                for _ in range(len(branch[0][2])) :
                    bond(branch[1], main[int(p)])
                # for i in range(1, len(branch)) :
                #     main.append(branch[i])
                main += [branch[i] for i in range(1,len(branch))]

            molecule.append(main)
        elif name == 'cyclo' :
            branch = molecule[-1]
            bond(branch[1], branch[-1])
        else :
            match name :
                case 'ane' | 'an' : # radical + ane => single bond
                    radical, last = getarr(code, index, RADICALS)
                    molecule.append(brancher(radical))
                case 'ene' :# (cyclo) + radical + positions + "-" + multiplier + "ene" => double bond
                    radical, last = getarr(code, index, RADICALS)
                    position, index = getpos(code, index)
                    branch = brancher(radical)

                    for pos in position :
                        bond(branch[int(pos)], branch[int(pos) + 1])

                    molecule.append(branch)
                case 'yne' :# (cyclo) + radical + positions + "-" + multiplier + "yne" => triple bond
                    radical, last = getarr(code, index, RADICALS)
                    position, index = getpos(code, index)
                    branch = brancher(radical)

                    for pos in position :
                        bond(branch[int(pos)], branch[int(pos) + 1])
                        bond(branch[int(pos)], branch[int(pos) + 1])

                    molecule.append(branch)
                case 'yl' : # positions + "-" + multiplier + (cyclo) + radical + () + "yl" => ramified alkane single bond
                    radical, last = getarr(code, index, RADICALS)
                    molecule.append(brancher(radical))
                case 'en' :
                    position, index = getpos(code, index)
                    branch = molecule[-1]

                    for pos in position :
                        bond(branch[int(pos)], branch[int(pos) + 1])
                case 'benz' :
                    branch = [ [0,'C',[]], [1,'C',[2,6,6]], [2,'C',[1,3,3]], [3,'C',[2,2,4]],
                    [4,'C',[3,5,5]], [5,'C',[4,4,6]], [6,'C',[5,1,1]] ]
                case 'phen' :
                    branch = [ [0,'C',[]], [1,'C',[2,6,6]], [2,'C',[1,3,3]], [3,'C',[2,2,4]],
                    [4,'C',[3,5,5]], [5,'C',[4,4,6]], [6,'C',[5,1,1]] ]
                case 'iodo' :
                    molecule.append([[0,'C',[0]], [1,'I',[]] ])
                case 'bromo' :
                    molecule.append([[0,'C',[0]], [1,'Br',[]] ])
                case 'fluoro' :
                    molecule.append([[0,'C',[0]], [1,'F',[]] ])
                case 'chloro' :
                    molecule.append([[0,'C',[0]], [1,'Cl',[]] ])
                case 'ol' : # suffix
                    molecule.append([[0,'C',[0]], [1,'O',[]] ])
                case 'hydroxy' : # prefix
                    molecule.append([[0,'C',[0]], [1,'O',[]] ])
                case 'thiol' : # suffix
                    molecule.append([[0,'C',[0]], [1,'S',[]] ])
                case 'mercapto' :
                    molecule.append([[0,'C',[0]], [1,'S',[]] ])
                case 'imine' : # suffix double bond
                    molecule.append([[0,'C',[0,0]], [1,'N',[]] ])
                case 'imino' :
                    molecule.append([[0,'C',[0,0]], [1,'N',[]] ])
                case 'one' : # suffix double bond
                    molecule.append([[0,'C',[0,0]], [1,'O',[]] ])
                case 'oxo' : # prefix double bond
                    molecule.append([[0,'C',[0,0]], [1,'O',[]] ])
                case 'formyl' : # prefix
                    molecule.append([[0,'C', [0]], [1, 'C', [2,2]], [2, 'O', [1,1]]])
                case 'al' : # suffix double bond
                    ix = max([atom[0] for branch in molecule for atom in branch])
                    branch = [[ix + 1, 'O', []] ] # molecule.append([[0, 'C', [0,0]], [1, 'O', []] ]) 
                    for _ in range(2) : bond(branch[0], molecule[-1][-1])

                    molecule[-1] += branch
                case 'amine' :
                    molecule.append([[0,'C', [0]], [1, 'N', []]])
                case 'phosphine' :
                    molecule.append([[0,'C', [0]], [1, 'P', []]])
                case 'arsine' :
                    molecule.append([[0,'C', [0]], [1, 'As', []]])
                case 'amide' : # suffix
                    molecule.append([[0,'C',[0]], [1,'N',[]] ])
                case 'amido' : # prefix
                    molecule.append([[0,'C',[0]], [1,'N',[]] ])
                case 'acid' :
                    pass
                case _ :
                    # print("unidentified : ", code[index])
                    pass
            pass

        index += 1

    adjency(molecule)
    print(smile(molecule))

def parser(name) :
    ast(name)

# # alkanes
# parser('methane')
# parser('ethane')
# parser('propane')
# parser('butane')
# parser('cyclobutane')
#
# # alkyls
# parser('2-methylbutane')
# parser('2,3,5-trimethylhexane')
# parser('3-ethyl-2,5-dimethylhexane')
# parser('2,7-dicyclobutyloctane')
#
# # alkynes and alkenes
# parser("hex-2-ene")
# parser("hex-2-yne")
# parser("tridec-4,10-dien-2,6,8-triyne")
# parser("4-hept-2,4-dienylundec-9-en-2,5-diyne")
#
# # halogenes
# parser('1-fluoropentane')
# parser("1,1-dibromo-4-chloropentane")
#
# parser("pentan-2-ol")
# parser("pentan-2-thiol")
# parser("pentan-2-imine")
# parser("pentan-2-one")
# parser("2-iminopentan-3-ol")
# parser('pentanal')

# parser("benzene")

# parser("methan-1-amine")
# parser("methan-1-arsine")
# parser("methan-1-phosphine")

# parser("5-methylhexan-2-ol")
#
# parser("diethylether")
# parser('1-[3-propyl]heptylcyclobutane')
#
# parser("3-[1-hydroxy]methylpentan-1,4-diol")
# parser('1,2-di[1-ethyl-3-[2-methyl]propyl]heptylcyclobutane')
