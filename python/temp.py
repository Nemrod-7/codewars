import re

table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1], 'I':[1, 126.9] }

RADICALS = ["--","meth",  "eth", "prop", "but", "pent", "hex", "hept", "oct", "non", "dec", "undec", "dodec", "tridec", "tetradec", "pentadec", "hexadec", "heptadec", "octadec", "nonadec"]
MULTIPLIERS = ["--","--","di", "tri", "tetra", "penta", "hexa", "hepta", "octa", "nona", "deca", "undeca", "dodeca", "trideca", "tetradeca", "pentadeca", "hexadeca", "heptadeca", "octadeca", "nonadeca"]

HYDROCARBON = ['ane','ene', 'yne' , 'yl' ]
HALOGEN = ['fluoro','chloro','bromo','iodo']

name = '1,2-di[1-ethyl-3-[2-methyl]propyl]heptylcyclobutane'

def tokenize (name) :
    name = name.replace('cyclo', ' cyclo ')
    name = name.replace('[', ' [ ').replace(']', ' ] ')
    name = name.replace('-', ' ')
    for sub in HYDROCARBON : name = name.replace(sub, sub + ' ')
    for sub in HALOGEN : name = name.replace(sub, sub + ' ')
    name = re.findall(r'\S+', name)
    
    return name

def getsub(name, index) :
    pile = 1

    while True :
        if name[index] == '[' : pile += 1
        if name[index] == ']' : pile -= 1
        
        if pile == 0 : return index
        index += 1

    return 0
    

def interpret(name) :
    index = 0
    position = []
    molecule = []

    # print(name)
    while index < len(name) :
        pos = re.findall(r'\d+', name[index])

        if name[index] == '[' :
            next = getsub(name, index + 1)
            interpret(name[index + 1 : next])

            index = next
        elif pos != [] :
            position.append(pos)
        else 
            molecule.append(name[index])

        index += 1

    while molecule :
        curr = molecule.pop()


    return []
    

name = '2-methyl'
name = tokenize(name)

interpret(name)



