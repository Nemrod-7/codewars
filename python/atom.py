import re
##############################################################################################
# Symbol:           H     B     C     N     O     F    Mg     P     S    Cl    Br            #
# Valence number:   1     3     4     3     2     1     2     3     2     1     1            #
# Atomic weight:  1.0  10.8  12.0  14.0  16.0  19.0  24.3  31.0  32.1  35.5  80.0  (in g/mol)#
##############################################################################################


order = ['C','H','O','B','Br','Cl','F','Mg','N','P','S']
table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1] }


RADICALS = ["meth",  "eth",   "prop",   "but",      "pent",     "hex",     "hept",     "oct",     "non",    "dec",  "undec", "dodec", "tridec", "tetradec", "pentadec", "hexadec", "heptadec", "octadec", "nonadec"]

MULTIPLIERS = [         "di",     "tri",     "tetra",     "penta",     "hexa",     "hepta",     "octa",     "nona",     "deca", "undeca", "dodeca", "trideca", "tetradeca", "pentadeca", "hexadeca", "heptadeca", "octadeca", "nonadeca"]

HYDROCARBON = ['ane','ene', 'yne' , 'yl' ] # => ['C']

HALOGEN = ['fluoro','chloro','bromo','iodo'] # => ['F','Cl','Br','I']

def alkane(chain) :
    if chain[-3:] == 'ane' : return 'C'
    return ''

def alcool(chain) :
    if chain[:8] == 'hydroxy' or chain[-2:] == 'ol' : return 'OH'
    return ''

def thiol(chain) :
    if chain[:8] == 'mercapto' or chain[-5:] == 'thiol' : return 'SH'
    return ''

def imine(chain) :
    if chain[:5] == 'imino' or chain[-5:] == 'imine' : return 'NH'
    return ''

def ketone(chain) :
    if chain[:3] == 'oxo' or chain[-3:] == 'one' : return 'O'
    return ''

def aldehyde(chain) :
    if chain[:6] == 'formyl' : return 'CH=O'
    if chain[-2:] == 'al': return 'O'
    return ''

def Carboxylic_acids(chain) :
    if chain[-5:] == 'acid' : return 'C=O'
    return ''

def amide(chain) :
    if chain[:5] == 'amido' or chain[-5:] == 'amide' : return 'NH2'
    return ''

def identify(chain) :
    
    atom = ''

    if chain[-3:] == 'ane' : # alkane
        atom = 'C'
    else if chain[-3:] == 'ene' : # alkene
        atom = 'C'
    else if chain[-3:] == 'yne' : # alkyne
        atom = 'C'
    else if chain[:8] == 'hydroxy' or chain[-2:] == 'ol' : # alcool
        atom = 'OH'
    else if chain[:8] == 'mercapto' or chain[-5:] == 'thiol' : # thiol
        atom = 'SH'
    else if chain[:5] == 'imino' or chain[-5:] == 'imine' : # imine
        atom = 'NH'
    else if chain[:3] == 'oxo' or chain[-3:] == 'one' : # ketone
        atom = 'O'
    else if chain[:5] == 'amido' or chain[-5:] == 'amide' : # amide
        atom = 'NH2'
    else if chain[:6] == 'formyl' : 
        atom = 'CH=O'
    else if chain[-2:] == 'al': 
        atom = 'O'
    else if chain[-5:] == 'acid' : 
        atom = 'C=O'

    return atom

def halogen(chain) :
    HALOGEN = { {'fluoro': 'F'},{'chloro':'Cl'},{'bromo':'Br'},{'iodo':'I'} }

    for type in HALOGEN :
        if chain[:len(type)] == type : return HALOGEN[type]

# methane = meth + ane = 1 carbon   ->  CH4
# ethane  = eth + ane  = 2 carbons  ->  CH3-CH3
# propane = ...                     ->  CH3-CH2-CH3
# butane  = ...                     ->  CH3-CH2-CH2-CH3

# alkane : simple bound between carbons. CnH2n+2 => radical + "ane"
# alkene : double bound between carbons. CnH2n   => radical + "-" + positions + "-" + multiplier + "ene"
# alkyne : triple bound between carbons. CnH2n-2 => radical + "-" + positions + "-" + multiplier + "yne"
# alkyl  : positions + "-" + multiplier + radical + "yl"


chain = '3-ethyl-2,5-dimethylhexane'
chain = 'tridec-4,10-dien-2,6,8-triyne'

chain = '1-fluoropentane'
chain = '1,2-di[1-ethyl-3-[2-methyl]propyl]heptylcyclobutane' 
chain = 'heptylcyclobutane'

def prefix(chain, array) :
    token = []
    for pre in array : 
        if chain[:len(pre)] == pre :
            token = pre

    return token

for sub in RADICALS :
    if sub == chain[:len(sub)] :
        # print(sub, chain[:len(sub)])
        pass

for i in range(len(MULTIPLIERS)) :
    if MULTIPLIERS[i] == chain[:len(MULTIPLIERS[i])] :
        print(mul, i+1 )



# chain = chain.replace('cyclo', ' ' + 'cyclo'  + ' ')
# mult = prefix(chain, MULTIPLIERS)
# chain = prefix(chain, RADICALS)
# chain = prefix(chain, HYDROCARBON)


# for sub in RADICALS : chain = chain.replace(sub,  sub +' ')
# for sub in HYDROCARBON : chain = chain.replace(sub, sub + ' ')


print(chain)
# token  = re.findall(r"[0-9]+|[\[\],-]|[a-z]+", chain)
# for cell in token : print(cell, end=' ')
