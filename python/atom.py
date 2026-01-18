import re
for _ in range(3) : print()

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
def prefix(text, arr) :
    max = [i for i in range(len(arr)) if arr[i] == text ]
    return 1 if max == [] else max[-1] + 1

def mainchain(code) :
    (maxv,inxv) = (0,0)

    for index in range( len(code) ) :
        name = code[index]
        pose = re.findall(r'\d+', name)

        if pose == [] and name != 'cyclo':
            radical = prefix(code[index], RADICALS)
            # print(name, '=>', radical, multipl, index )
            # if multipl > radical :
            #     ii = len(MULTIPLIERS[multipl-1])
            #     name = name[ii:]
            #     radical = RADICALS.index(code[index]) + 1
            if radical >= maxv :
                (maxv,inxv) = (radical,index)
    return (maxv, inxv)

def getposition(code, index) :
    position = re.findall(r'\d+', code[index + 1])

    if position == [] :
        index += 1
        position = re.findall(r'\d+', code[index + 1])

    index += 1
    return index, position
def parser(name) :
    code = tokenize(name)
    radical, index = mainchain(code)
    molecule = [brancher(radical)]
    code = [ code[it] for it in range(len(code)) if it != index  ]
    index = 0
    print(code)
    while index < len(code) :
        pos = re.findall(r'\d+', code[index])
        # print(code[index], end=' ')
        if pos != [] :
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
            match code[index] :
                case 'cyclo' :
                    branch = molecule[-1]
                    bond(branch[1], branch[-1])
                case 'ane' | 'yl' :
                    radical = RADICALS.index(code[index + 1]) + 1
                    molecule.append(brancher(radical))
                    index += 1
                case 'yne' :
                    branch = molecule[-1]
                    index,position = getposition(code, index)

                    for pos in position :
                        bond(branch[int(pos)], branch[int(pos) + 1])
                        bond(branch[int(pos)], branch[int(pos) + 1])
                case 'ene' | 'en' : # double bond
                    branch = molecule[-1]
                    index,position = getposition(code, index)

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
        index += 1

    print(smile(molecule))


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

# parser("1,1-dibromo-4-chloropentane")
# parser('1-fluoropentane')

# parser("hex-2-ene")
# parser("hex-2-yne")

# parser("tridec-4,10-dien-2,6,8-triyne")
# parser("4-hept-2,4-dienylundec-9-en-2,5-diyne")
parser("4-hept-2,4-dienyl")

# #
# parser("pentan-2-ol")
# parser("diethylether")
#
# parser('1-[3-propyl]heptylcyclobutane')
# parser('1,2-di[1-ethyl-3-[2-methyl]propyl]heptylcyclobutane')
#
# parser("5-methylhexan-2-ol")
