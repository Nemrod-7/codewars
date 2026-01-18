import re
# for _ in range(3) : print()

table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1] }

RADICALS = ["meth",  "eth", "prop", "but", "pent", "hex", "hept", "oct", "non", "dec", "undec", "dodec", "tridec", "tetradec", "pentadec", "hexadec", "heptadec", "octadec", "nonadec"]
MULTIPLIERS = ["--","di", "tri", "tetra", "penta", "hexa", "hepta", "octa", "nona", "deca", "undeca", "dodeca", "trideca", "tetradeca", "pentadeca", "hexadeca", "heptadeca", "octadeca", "nonadeca"]

HYDROCARBON = ['ane','ene', 'yne' , 'yl' ] # => ['C']
HALOGEN = ['fluoro','chloro','bromo','iodo'] # => ['F','Cl','Br','I']

def tokenize (name) :
    name = name.replace('cyclo', ' cyclo ')
    name = name.replace('[', ' { ').replace(']', ' } ')
    name = name.replace('{', ' ] ').replace('}', ' [ ')
    name = name.replace('-', ' ')
    for sub in HYDROCARBON : name = name.replace(sub,' ' + sub + ' ')
    for sub in HALOGEN : name = name.replace(sub, ' ' + sub + ' ')
    for sub in ['di','meth','an '] : name = name.replace(sub, ' ' + sub + ' ')
    name = re.findall(r'\S+', name)
    name.reverse()
    return name

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
    # width = max([len(branch) for branch in molecule])
    # height = len(molecule)
    # graph = [[ [] for _ in range(width) ] for _ in range(height + 1)]
    formula = ''
    links = ''
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
                links +=  len(link) * ' ' + '|' + (len(represent(branch[i])) - 1) * ' '
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

def prefix(code, index, array) :
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

def parser(name) :
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
                bond(branch[1], main[int(p)])
                ix += len(alky) - 1

                for i in range(1, len(branch)) :
                    main.append(branch[i])

            molecule.append(main)
        elif name == 'cyclo' :
            branch = molecule[-1]
            bond(branch[1], branch[-1])
        else :
            match name :
                case 'ane' | 'an' : # radical + ane => single bond
                    radical, last = prefix(code, index, RADICALS)
                    molecule.append(brancher(radical))
                case 'ene' :# (cyclo) + radical + positions + "-" + multiplier + "ene" => double bond
                    radical, last = prefix(code, index, RADICALS)
                    position, index = getpos(code, index)
                    branch = brancher(radical)

                    for pos in position :
                        bond(branch[int(pos)], branch[int(pos) + 1])

                    molecule.append(branch)
                case 'yne' :# (cyclo) + radical + positions + "-" + multiplier + "yne" => triple bond
                    radical, last = prefix(code, index, RADICALS)
                    position, index = getpos(code, index)
                    branch = brancher(radical)

                    for pos in position :
                        bond(branch[int(pos)], branch[int(pos) + 1])
                        bond(branch[int(pos)], branch[int(pos) + 1])

                    molecule.append(branch)
                case 'yl' : # positions + "-" + multiplier + (cyclo) + radical + () + "yl" => ramified alkane single bond
                    radical, last = prefix(code, index, RADICALS)
                    # multipl = prefix(code, last, MULTIPLIERS)
                    molecule.append(brancher(radical))
                case 'en' :
                    position, index = getpos(code, index)
                    branch = molecule[-1]

                    for pos in position :
                        bond(branch[int(pos)], branch[int(pos) + 1])
                case 'iodo' :
                    molecule.append([[0,'C',[]], [1,'I',[]] ])
                case 'bromo' :
                    molecule.append([[0,'C',[]], [1,'Br',[]] ])
                case 'fluoro' :
                    molecule.append([[0,'C',[]], [1,'F',[]] ])
                case 'chloro' :
                    molecule.append([[0,'C',[]], [1,'Cl',[]] ])
                case 'ol' : # suffix
                    code[index + 1], code[index + 2] = code[index + 2], code[index + 1] 

                    molecule.append([[0,'C',[]], [1,'O',[2]], [2,'H',[1]] ])
                case 'hydroxy' :
                    molecule.append([[0,'C',[]], [1,'O',[2]], [2,'H',[1]] ])
                case 'thiol' : # suffix
                    molecule.append([[0,'C',[]], [1,'S',[2]], [2,'H',[1]] ])
                case 'mercapto' :
                    molecule.append([[0,'C',[]], [1,'S',[2]], [2,'H',[1]] ])
                case 'imine' : # double bond
                    molecule.append([[0,'C',[]], [1,'N',[2]], [2,'H',[1]] ])
                case 'imino' :
                    molecule.append([[0,'C',[]], [1,'N',[2]], [2,'H',[1]] ])

                case _ :
                    print("unidentified : ", code[index])
                    pass
            pass

        index += 1

    adjency(molecule)
    print(smile(molecule))
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

# halogenes
# parser('1-fluoropentane')
# parser("1,1-dibromo-4-chloropentane")

parser("pentan-2-ol")
# parser("3-[1-hydroxy]methylpentan-1,4-diol")
# parser("5-methylhexan-2-ol")
#
# parser("diethylether")
#
# parser('1-[3-propyl]heptylcyclobutane')
# parser('1,2-di[1-ethyl-3-[2-methyl]propyl]heptylcyclobutane')
