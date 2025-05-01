import re
##############################################################################################
# Symbol:           H     B     C     N     O     F    Mg     P     S    Cl    Br            #
# Valence number:   1     3     4     3     2     1     2     3     2     1     1            #
# Atomic weight:  1.0  10.8  12.0  14.0  16.0  19.0  24.3  31.0  32.1  35.5  80.0  (in g/mol)#
##############################################################################################

for _ in range(3): print()

order = ['C','H','O','B','Br','Cl','F','Mg','N','P','S']
table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1] }

RADICALS = ["meth",  "eth",   "prop",   "but",      "pent",     "hex",     "hept",     "oct",     "non",    "dec",  "undec", "dodec", "tridec", "tetradec", "pentadec", "hexadec", "heptadec", "octadec", "nonadec"]
MULTIPLIERS = ["--","di",     "tri",     "tetra",     "penta",     "hexa",     "hepta",     "octa",     "nona",     "deca", "undeca", "dodeca", "trideca", "tetradeca", "pentadeca", "hexadeca", "heptadeca", "octadeca", "nonadeca"]
HYDROCARBON = ['ane','ene', 'yne' , 'yl' ] # => ['C']
HALOGEN = ['fluoro','chloro','bromo','iodo'] # => ['F','Cl','Br','I']

def identify(chain) :
    atom = ''

    if chain[-3:] == 'ane' : # alkane
        atom = 'alkane'
    elif chain[-3:] == 'ene' : # alkene
        atom = 'alkene'
    elif chain[-3:] == 'yne' : # print(token)alkyne
        atom = 'alkyne'
    elif chain[:8] == 'hydroxy' or chain[-2:] == 'ol' : # alcool
        atom = 'alcool'
    elif chain[:8] == 'mercapto' or chain[-5:] == 'thiol' : # thiol
        atom = 'thiol'
    elif chain[:5] == 'imino' or chain[-5:] == 'imine' : # imine
        atom = 'imine'
    elif chain[:3] == 'oxo' or chain[-3:] == 'one' : # ketone
        atom = 'ketone'
    elif chain[:5] == 'amido' or chain[-5:] == 'amide' : # amide
        atom = 'amide'
    elif chain[:6] == 'formyl' or chain[-2:] == 'al':  # aldehyde
        atom = 'aldehyde'
    elif chain[-5:] == 'acid' : # carboxylic acid
        atom = 'carboxylic acid'

    return ''

# methane = meth + ane = 1 carbon   ->  CH4
# ethane  = eth + ane  = 2 carbons  ->  CH3-CH3
# propane = ...                     ->  CH3-CH2-CH3
# butane  = ...                     ->  CH3-CH2-CH2-CH3

# alkane : simple bound between carbons. CnH2n+2 => radical + "ane"
# alkene : double bound between carbons. CnH2n   => radical + "-" + positions + "-" + multiplier + "ene"
# alkyne : triple bound between carbons. CnH2n-2 => radical + "-" + positions + "-" + multiplier + "yne"
# alkyl  : positions + "-" + multiplier + radical + "yl"

molecule = '1-fluoropentane'
molecule = 'cyclobutane'
molecule = 'heptylcyclobutane'
molecule = '3-ethyl-2,5-dimethylhexane'
molecule = 'tridec-4,10-dien-2,6,8-triyne'
molecule = '1,2-di[1-ethyl-3-[2-methyl]propyl]heptylcyclobutane'

def getradical(chain, array) :
    mul = 0
    for i in range(len(array)) :
        if array[i] == chain[:len(array[i])] :
            mul = i + 1

    return mul

molecule = molecule.replace('cyclo', ' ' + 'cyclo'  + ' ')

for sub in HALOGEN : molecule = molecule.replace(sub, ' ' + sub + ' ')
# mult = prefix(molecule, MULTIPLIERS)
# molecule = prefix(molecule, RADICALS)
# molecule = prefix(molecule, HYDROCARBON)

# for sub in RADICALS : molecule = molecule.replace(sub,  sub +' ')
# for sub in HYDROCARBON : molecule = molecule.replace(sub, sub + ' ')

token  = re.findall(r"[0-9]+|[\[\],-]|[a-z]+", molecule)

print(token)

chain = token[0]
type = identify(chain)

match type :
    case 'alkane' : # radical + 'ane'
        atom = 'C'
        carbon = [i for i in range(len(RADICALS)) if RADICALS[i] == chain[:len(RADICALS[i])]][-1] + 1
        formula = atom + (str(carbon) if carbon > 1 else '') + 'H' + str(2 * carbon + 2)

        print(chain, ':',formula)
        pass


# for cell in token : print(cell, end=' ')
