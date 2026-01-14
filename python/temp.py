import re
#####################################################################################################
# Symbol:           H     B     C     N     O     F    Mg     P     S    Cl    Br      I            #
# Valence number:   1     3     4     3     2     1     2     3     2     1     1      1            #
# Atomic weight:  1.0  10.8  12.0  14.0  16.0  19.0  24.3  31.0  32.1  35.5  80.0  126.9  (in g/mol)#
#####################################################################################################

order = ['C','H','O','B','Br','Cl','F','Mg','N','P','S']
table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1], 'I':[1, 126.9] }

RADICALS = ["meth",  "eth", "prop", "but", "pent", "hex", "hept", "oct", "non", "dec", "undec", "dodec", "tridec", "tetradec", "pentadec", "hexadec", "heptadec", "octadec", "nonadec"]
MULTIPLIERS = ["--","di", "tri", "tetra", "penta", "hexa", "hepta", "octa", "nona", "deca", "undeca", "dodeca", "trideca", "tetradeca", "pentadeca", "hexadeca", "heptadeca", "octadeca", "nonadeca"]

HYDROCARBON = ['ane','ene', 'yne' , 'yl' ] # => ['C']
HALOGEN = ['fluoro','chloro','bromo','iodo'] # => ['F','Cl','Br','I']

class UnlockedMolecule(Exception) :
    def __init__(self) :
        super().__init__('Unlocked Molecule.')
class LockedMolecule(Exception):
    def __init__(self) :
        super().__init__('Locked Molecule.')
class EmptyMolecule(Exception) :
    def __init__(self) :
        super().__init__('Empty Molecule.')
class InvalidBond(Exception):
    def __init__(self) :
        super().__init__('Invalid Bond')

class Atom() :
    def __init__(self, id, elt, adj) :
        self.id = id
        self.element = elt
        self.edge = adj

    def __hash__(self):      return self.id
    def __eq__(self, other): return self.id == other.id

    def __str__(self) :
        self.edge.sort(key = lambda x: x.id  )
        self.edge.sort(key = lambda x: order.index(x.element) if x.element != 'H' else len(order)  )

        prefix = 'Atom(' + self.element + '.' + str(self.id)
        edge =  ['H' if bond.element == 'H' else bond.element + str(bond.id) for bond in self.edge]

        if len(edge) > 0 : prefix += ': '
        return prefix + ','.join(edge) + ')'

    def bond(atom1, atom2) :
        a1, a2 = atom1, atom2

        if a1 == a2 : raise InvalidBond
        if not a1.valid_valence() or not a2.valid_valence() : raise InvalidBond

        atom1.edge.append(a2)
        atom2.edge.append(a1)

    def valid_valence(self) :
        return len(self.edge) < table[self.element][0]
# parser('ethane')
# parser('propane')
# parser('butane')
# parser('cyclobutane')

# parser('1-fluoropentane')
# parser('2-methylbutane')

# parser('2,3,5-trimethylhexane')
# parser('3-ethyl-2,5-dimethylhexane')


# parser('3,7-dicyclobutyloctane')
# parser('1-[3-propyl]heptylcyclobutane')
# print('1,2-di[1-ethyl-3-[2-methyl]propyl]heptylcyclobutane')
# parser(name)
