import re
from display import *
# import fma2_tests
for _ in range(3) : print()

table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1], 'I':[1, 0.0], 'As':[3, 0.0] }

RADICALS = ["meth",  "eth", "prop", "but", "pent", "hex", "hept", "oct", "non", "dec", "undec", "dodec", "tridec", "tetradec", "pentadec", "hexadec", "heptadec", "octadec", "nonadec"]
MULTIPLIERS = ["--","di", "tri", "tetra", "penta", "hexa", "hepta", "octa", "nona", "deca", "undeca", "dodeca", "trideca", "tetradeca", "pentadeca", "hexadeca", "heptadeca", "octadeca", "nonadeca"]

AMARPHOS = ['amine', 'phosphine','arsine', 'amino', 'phosphino', 'arsino']
MISC = ['di','meth','an ', 'cyclo','oate', 'benz', 'phen']
PREFIX = ['hydroxy','mercapto','imino','oxo','oxy','formyl','amide', 'fluoro','chloro','bromo','iodo']
SUFFIX  = ['ol','thiol','imine','one','al', 'amido']
HYDROCARBON = ['ane','ene', 'yne' , 'yl' ]
KEYWORDS = (HYDROCARBON + PREFIX + SUFFIX + MISC + AMARPHOS)

def get_rad(name, index) :
    while index < len(name) :
        if name[index] in RADICALS :
            return (RADICALS.index(name[index]) + 1, index)
        index += 1
    return 0,0

def get_pos(code, index) :
    while index < len(code) :
        if re.findall(r'\d+', code[index]) :
            return re.findall(r'\d+', code[index]), index
        index += 1

    return [],0

def suffix(code) : # retreat each suffix which is not part of a main chain
    # source : [suffix, (multiplier), (positions), alkane, radical]
    # expect : [alkane, radical, suffix, (multiplier), (positions)]
    i = 0

    while i < len(code):
        if code[i] in SUFFIX :
            _, end = get_rad(code, i)
            rad = i + end + 1
            pos = rad - 2
            code[i:rad] = code[pos : rad] + code[i : pos]
            i += end
        i += 1

def tokenize (name) :
    name = name.replace('[', ' { ').replace(']', ' } ')
    name = name.replace('{', ' ] ').replace('}', ' [ ')
    name = name.replace('-', ' ')
    name = name.replace('thiol', ' thi@l ')
    name = name.replace('formyl', ' formul ')
    for sub in KEYWORDS : name = name.replace(sub, ' ' + sub + ' ')
    name = name.replace('thi@l', ' thiol ')
    name = name.replace('formul', ' formyl ')
    name = re.findall(r'\S+', name)
    name.reverse()
    suffix(name)
    return name

def bond (a, b) :
    a[2].append(b[0])
    b[2].append(a[0])

def reindex(molecule, index) :
    ix = index + [1 for i in range(len(molecule)) for j in range(1, len(molecule[i]))].count(1)

    for i in range(len(molecule) - 1, -1, -1) :
        for j in range(len(molecule[i]) - 1, 0, -1) :
            nid = ix
            pid = molecule[i][j][0]

            for br in molecule :
                for at in range(1, len(br)) :
                    a2 = br[at]
                    for k in range(len(a2[2])) :
                        if a2[2][k] == pid :
                            a2[2][k] = nid

            molecule[i][j][0] = nid
            ix -= 1

def append(molecule, stack, position) :
    for p in position :
        subc = stack.pop()
        ix = molecule[-1][-1][0]
        info = subc[0][0]
        reindex(subc, ix)
        for _ in range(len(info[2])) :
            bond(molecule[0][p], subc[0][1])
        molecule += subc

def get_element(name) :
    if name == 'ol' : return([ [[0,'C',[0]], [1,'O',[]]]])
    if name == 'al' : return([ [[0, 'C',[0,0]], [1,'O',[]]]])
    if name == 'oxo' : return([ [[0,'C',[0,0]], [1,'O',[]]]])
    # if name == 'oxy' : return([ [[0,'C',[0,0]], [1,'O',[]]]])
    if name == 'one' : return([ [[0,'C',[0,0]], [1,'O',[]]]])
    if name == 'ether' : return([ [[0,'C',[0]], [1,'O',[]]]])
    if name == 'formyl' : return([ [[0,'C', [0]], [1, 'C', [2,2]], [2, 'O', [1,1]]]])
    if name == 'hydroxy' : return([ [[0,'C',[0]], [1,'O',[]]]])

    if name == 'thiol' : return([ [[0,'C',[0]], [1,'S',[]]]])
    if name == 'mercapto' : return([ [[0,'C',[0]], [1,'S',[]]]])

    if name == 'iodo' : return([ [[0,'C',[0]], [1,'I',[]]]])
    if name == 'bromo' : return([ [[0,'C',[0]], [1,'Br',[]]]])
    if name == 'fluoro' : return([ [[0,'C',[0]], [1,'F',[]]]])
    if name == 'chloro' : return([ [[0,'C',[0]], [1,'Cl',[]]]])

    if name == 'imine' : return([ [[0,'C',[0,0]], [1,'N',[]]]])
    if name == 'imino' : return([ [[0,'C',[0,0]], [1,'N',[]]]])

    if name == 'amide' : return([ [[0,'C',[0,0]], [1,'C',[2]], [2,'N',[1,3,3]], [3, 'O', [2,2]] ]])
    if name == 'amido' : return([ [[0,'C',[0]], [1,'N',[]]]])

    if name == 'amine' : return([ [[0,'C', [0]], [1, 'N', []]]])
    if name == 'amino' : return([ [[0,'C', [0]], [1, 'N', []]]])
    if name == 'arsine' : return([ [[0,'C', [0]], [1, 'As', []]]])
    if name == 'arsino' : return([ [[0,'C', [0]], [1, 'As', []]]])
    if name == 'phosphine' : return([ [[0,'C', [0]], [1, 'P', []]]])
    if name == 'phosphino' : return([ [[0,'C', [0]], [1, 'P', []]]])
    return []

# def reindex2(structure, index) :
#
#     for chain in structure :
#         for i in range(1, len(chain)) :
#             index += 1
#             nid = index
#             pid = chain[i][0]
#
#             for br in structure :
#                 for at in range(1, len(br)) :
#                     a2 = br[at]
#                     for j in range(len(a2[2])) :
#                         if a2[2][j] == pid :
#                             a2[2][j] = nid
#             chain[i][0] = nid

    # print(structure)
# def append2(molecule, stack, position) :
#     print('*********first*************')
#     show.stack(stack)
#
#     for p in position :
#         subc = stack.pop()
#         ix = molecule[-1][-1][0]
#         info = subc[0][0]
#         reindex2(subc, ix)
#
#         for _ in range(len(info[2])) :
#             bond(molecule[0][p], subc[0][1])
#         molecule += subc
#
#     print('********second*************')
#     show.stack(stack)
def mk_branch(code, index) :

    while index < len(code) :
        # if code[index] == 'benz' or code[index] == 'phen' :
        #     return [ [0,'C',[]], [1,'C',[2,6,6]], [2,'C',[1,3,3]], [3,'C',[2,2,4]], [4,'C',[3,5,5]], [5,'C',[4,4,6]], [6,'C',[5,1,1]] ]
        if code[index] in RADICALS :
            radical = RADICALS.index(code[index]) + 1
            branch = [[0, 'C', [0]]] + [[i + 1, 'C', []] for i in range(radical)]

            for i in range(2, len(branch)) :
                bond(branch[i-1], branch[i-0])
            return branch
        index += 1
    return [[0, 'C', [0]]]

def parser(name) :
    index = 0
    stkpos, stack = [], []
    code = tokenize(name)

    print(code)
    while index < len(code) :
        match code[index] :
            case '[' : pass
            case ']' : pass
            case 'cyclo' :
                branch = stack[-1][-1]
                bond(branch[1], branch[-1])
            case 'ane' | 'an' : # radical + ane => single bond
                stack.append([mk_branch(code,index)])
            case 'ene' :# (cyclo) + radical + positions + "-" + multiplier + "ene" => double bond
                if code[index + 1] == 'benz' :
                    branch = [ [0,'C',[]], [1,'C',[2,6,6]], [2,'C',[1,3,3]], [3,'C',[2,2,4]], [4,'C',[3,5,5]], [5,'C',[4,4,6]], [6,'C',[5,1,1]] ]
                else :
                    position, index = get_pos(code, index)
                    branch = mk_branch(code, index)

                    for pos in position :
                        bond(branch[int(pos)], branch[int(pos) + 1])

                stack.append([branch])
            case 'yne' :# (cyclo) + radical + positions + "-" + multiplier + "yne" => triple bond
                position, index = get_pos(code, index)
                branch = mk_branch(code,index)

                for pos in position :
                    bond(branch[int(pos)], branch[int(pos) + 1])
                    bond(branch[int(pos)], branch[int(pos) + 1])

                stack.append([branch])
            case 'yl' : # positions + "-" + multiplier + (cyclo) + radical + () + "yl" => ramified alkane single bond
                stack.append([mk_branch(code,index)])
            case 'en' :
                position, index = get_pos(code, index)
                branch = stack[-1][0]
                for pos in position :
                    bond(branch[int(pos)], branch[int(pos) + 1])
            case _ :
                element = get_element(code[index])
                position = re.findall(r'\d+', code[index])

                if element != [] :
                    stack.append(element)
                if position != [] :
                    if len(stack) > len(position) :
                        position = [int(it) for it in position]
                        molecule = stack[-(len(position) + 1)]

                        # if code[index] == '4' :
                        #     append2(molecule, stack, position)
                        # else :
                        append(molecule, stack, position)
                    else :
                        stkpos.append(position)

                if code[index] in MULTIPLIERS :
                    molecule = stack.pop()
                    multiple = MULTIPLIERS.index(code[index]) + 1

                    for _ in range(multiple) :
                        stack.append([ [ [at[0], at[1], [edge for edge in at[2]]] for chain in molecule for at in chain ] ])

                    if index == (len(code) - 1) or not re.findall(r'\d+', code[index + 1]) :
                        molecule = stack[-(multiple + 1)]
                        end = [len(molecule)] * multiple

                        append(molecule, stack, end)
                        # print(molecule, end)
                        # print (code, multiple)

        # print('****************************')
        # print(code[index])
        # show.stack(stack)
        index += 1

    if stkpos != [] :
        stack[0], stack[-1] = stack[-1],stack[0]

    molecule = stack[0]
    main = molecule[0]
    end = [len(main) - 1]

    while (len(stack) > 1) :
        pos = end if stkpos == [] else [int(it) for it in stkpos.pop()]
        append(molecule, stack, pos)

    # print('****************************')
    show.stack(stack)
    # print(show.smile(molecule))
    # return main

# parser('4-[1-oxo]ethylheptan-2,6-dione')

# parser('4-[1-formyl] methyl heptan dial') # the 'di'al with no position
# parser('2-ethyl-1-formylbenzene')
# parser('3-prop-2-enoxypropan-1-ol')

print('end')
