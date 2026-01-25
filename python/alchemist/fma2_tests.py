from structure import parser

# alkanes
parser('methane')
parser('ethane')
parser('propane')
parser('butane')
parser('cyclobutane')

# alkyls
parser('2-methylbutane')
parser('2,3,5-trimethylhexane')
parser('3-ethyl-2,5-dimethylhexane')
parser('2,7-dicyclobutyloctane')

# alkynes and alkenes
parser("hex-2-ene")
parser("hex-2-yne")
parser("tridec-4,10-dien-2,6,8-triyne")
parser("4-hept-2,4-dienylundec-9-en-2,5-diyne")

# # ramifications
parser('1,2-di[3-propyl]heptylcyclobutane')
parser('1-[3-propyl]heptylcyclobutane')
parser('1,2-di[1-ethyl-3-[2-methyl]propyl]heptylcyclobutane')

# # Simples functions # # #
# halogenes
parser('1-fluoropentane')
parser("1,1-dibromo-4-chloropentane")

# alcools
parser("pentan-2-ol")
parser("3-[1-hydroxy]methylpentan-1,4-diol")

# thiols
parser("pentan-2-thiol")
parser("3-[2-mercapto]ethylpentan-1,4-diol")

# imines
parser("pentan-2-imine")
parser("2-iminopentan-3-ol")

# # a bit less simple function # # #
# ketones
parser("pentan-2-one")
parser("4-[1-oxo]ethylheptan-2,6-dione")

# aromatic cycles
parser('benzene')
parser('2-ethyl-1-formylbenzene')
# # aldehydes
parser('pentanal')

# # amines, phosphenes, arsines
parser("methylamine")
parser("methan-1-amine")
parser("methylphosphine")
parser("methan-1-phosphine")
parser("methylarsine")
parser("methan-1-arsine")
parser("ethyldimethylamine")

# ethers
parser("diethylether")

###############################################################################

# # aldehydes
# parser("4-[1-formyl]methylheptandial")
#
# # arboxylic acids
# parser('ethanoic acid')
# parser('methancarboxylic acid')
# parser('4-carboxyheptan-1,7-dioic acid')

# # amides
# parser("methanamide")
# parser("pent-3-enamide")
# parser("5-amidopentanoic acid")
#
# # amines, phosphenes, arsines
# parser("ethan-1-[dimethyl]amine")
# parser("hexan-1,6-diamine")
# parser("1,6-diaminohexan-3-ol")
# parser("1-amino-6-[diethyl]arsinohexan-3-ol")
#
# # ethers
# parser("methylprop-1-enylether")
#
# # esters...
#
# parser("benzene")
