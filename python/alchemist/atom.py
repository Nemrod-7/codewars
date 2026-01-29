import re
# import fma2_tests

order = ['C','H','O','B','Br','Cl','F','Mg','N','P','S','I','As']
table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1], 'I':[1, 0.0], 'As':[3, 0.0] }

RADICALS = ["meth",  "eth", "prop", "but", "pent", "hex", "hept", "oct", "non", "dec", "undec", "dodec", "tridec", "tetradec", "pentadec", "hexadec", "heptadec", "octadec", "nonadec"]
MULTIPLIERS = ["--","di", "tri", "tetra", "penta", "hexa", "hepta", "octa", "nona", "deca", "undeca", "dodeca", "trideca", "tetradeca", "pentadeca", "hexadeca", "heptadeca", "octadeca", "nonadeca"]


# 'oic acid' => ethanoic acid
#      OH
#      |
# ...-(C)=O
#
# [ [ [0,'C',[0]], [1,'O',[]] ],  # branch 1
# [ [0,'C',[0,0]], [0,'O',[]] ]]  # branch 2

# 'carboxylic acid' => methancarboxylic acid | 4-carboxyheptan-1,7-dioic acid
#     OH
#     |
# ...-C=O
#
# [0,'C',[0]], [1,'C',[2,3,3]], [2,'O',[1]], [3,'O',[1,1]]

# 'amide' | 'amido' => methanamide | 5-amidopentanoic acid
#      NH2
#      |
# ...-(C)=O

# [ [ [0,'C',[0]], [1,'N',[]] ],  # branch 1
# [ [0,'C',[0,0]], [0,'O',[]] ]]  # branch 2


chain = [0,'C',[0]], [1,'C',[2,3,3]], [2,'O',[1]], [3,'O',[1,1]]

hist = {}

for it in chain :
    hist[it[1]] = 1 if not it[1] in hist else hist[it[1]] + 1

arr = [(key,hist[key]) for key in hist]
arr.sort(key = lambda x: order.index(x[0]))
formula = [ it[0] + (str(it[1]) if it[1] > 1 else '')for it in arr]

print( ''.join(formula) )
