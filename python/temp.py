import re

table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1], 'I':[1, 126.9] }

RADICALS = ["--","meth",  "eth", "prop", "but", "pent", "hex", "hept", "oct", "non", "dec", "undec", "dodec", "tridec", "tetradec", "pentadec", "hexadec", "heptadec", "octadec", "nonadec"]
MULTIPLIERS = ["--","--","di", "tri", "tetra", "penta", "hexa", "hepta", "octa", "nona", "deca", "undeca", "dodeca", "trideca", "tetradeca", "pentadeca", "hexadeca", "heptadeca", "octadeca", "nonadeca"]

HYDROCARBON = ['ane','ene', 'yne' , 'yl' ]
HALOGEN = ['fluoro','chloro','bromo','iodo'] 

# radical + "ane"
# radical + "-" + positions + "-" + multiplier + "ene"
# radical + "-" + positions + "-" + multiplier + "yne"
# positions + "-" + multiplier + "cyclo" + [" + subramification + "]" + radical + "yl"

def prefix(text, arr) :
    max = [i for i in range(len(arr)) if arr[i] == text[:len(arr[i])] ]
    return [text, 1] if max == [] else [text[len(arr[max[-1]]):] , max[-1] + 1]

def search(text, arr) :
    max = [i for i in range(len(arr)) if arr[i] == text[:len(arr[i])] ]
    return 0 if max == [] else max[-1] + 1


def detect(token) :
    # print(chain, end = ' >> ')
    # chain = chain.replace('-', ' ')
    # chain = chain.replace('cyclo', ' cyclo ')
    #
    # token = re.findall(r'\S+', chain)
    position = []
    multipl, radical, cyclo = 1, 1, True if 'cyclo' in token else False

    for cell in token :
        num, rad, mul = re.findall(r'\d+', cell), search(cell, RADICALS), search(cell, MULTIPLIERS)

        if num != [] : position = num
        if rad != 0 : radical = rad 
        if mul != 0 : multipl = mul

    return position, multipl, radical, cyclo 

name = 'hex-1,4-diene'
name = '1-fluoropentane'
name = '2-methylbutane'
name = '1-heptylcyclobutane'
name = '2,3,5-triethylhexane'
name = '2,7-dicyclobutyl'
name = '3-ethyl -2,5-dimethylhexane'

# positions + "-" + multiplier + "cyclo" + [" + subramification + "]" + radical + "yl"
name = '1-[3-propyl]heptylcyclobutane'
name = '1,2-di[1-ethyl-3-[2-methyl]propyl]heptylcyclobutane'


name = name.replace('cyclo', ' cyclo ')
name = name.replace('[', ' [ ').replace(']', ' ] ')
name = name.replace('-', ' ')
for sub in HYDROCARBON : name = name.replace(sub, sub + ' ')
for sub in HALOGEN : name = name.replace(sub, sub + ' ')
name = re.findall(r'\S+', name)

index = 0
position = []

while index < len(name) :
    pos = re.findall(r'\d+', name[index])
    
    if pos != [] :
        if len(pos) > 1 : index += 1
        position.append(pos)
    else :
        print(name[index])

    index += 1

# index = 0
# chain = []

# while index < len(name) :
#     chain.append( name[index] )
#
#     if name[index] == 'ane' :
#         position, multipl, radical, cyclo = detect(chain)
#         # print(chain, end=' >> ')
#         # print(position, multipl, radical, cyclo)
#         chain = []
#     elif name[index] == 'yl' :
#         position, multipl, radical, cyclo = detect(chain)
#         # print(chain, end=' >> ')
#         # print(position, multipl, radical, cyclo)
#         chain = []
#     elif name[index] in HALOGEN:
#         print(chain)
#         chain = []
#
#     index += 1

