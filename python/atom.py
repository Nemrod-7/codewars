
from molecules import *
##############################################################################################
# Symbol:           H     B     C     N     O     F    Mg     P     S    Cl    Br            #
# Valence number:   1     3     4     3     2     1     2     3     2     1     1            #
# Atomic weight:  1.0  10.8  12.0  14.0  16.0  19.0  24.3  31.0  32.1  35.5  80.0  (in g/mol)#
##############################################################################################

table = { 'H':[1, 1.0 ], 'B':[3, 10.8], 'C':[4, 12.0], 'N':[3, 14.0], 'O':[2, 16.0], 'F':[1, 19.0], 'P':[3, 31.0], 'S':[2, 32.1], 'Mg':[2, 24.3], 'Cl':[1, 35.5], 'Br':[1, 80.0] }

InvalidBond = Exception('Invalid bond.')
UnlockedMolecule = Exception('Unlocked molecule.')
LockedMolecule = Exception('Locked molecule.')
EmptyMolecule = Exception('Empty molecule.')

class Atom() :
    def __init__(self, id, elt, adj) :
        self.id = id
        self.element = elt
        self.edge = adj

    def __hash__(self):      return self.id
    def __eq__(self, other): return self.id == other.id

    def __str__(self) :
        prefix = 'Atom(' + self.element + '.' + str(self.id)
        edge =  ['H' if bond.element == 'H' else bond.element + str(bond.id) for bond in self.edge]

        if len(edge) > 0 : prefix += ': '

        return prefix + ','.join(edge) + ')'

    def valid_valence(self) :
        return len(self.edge) < table[self.element][0]

class Molecule() :
    def __init__ (self, name='') :
        self.name = name
        self.formula = ''
        self.molecular_weight = 0.0

        self.lock = False
        self.atoms = []
        self.arms = [[0]]

    def bond(self, i1, i2) :
        a1, a2 = self.atoms[i1], self.atoms[i2]
        self.atoms[i1].edge.append(a2)
        self.atoms[i2].edge.append(a1)

    def brancher(self, *arg) :
        # Adds new "branches" to the current molecule.
        # Can take any number of arguments (positive integers).
        # Each argument gives the number of carbons of the new branch.
        if self.lock == True : raise UnlockedMolecule

        for nc in arg :
            branch = [0] + [len(self.atoms) + i for i in range(0, nc)]
            self.atoms += [Atom((len(self.atoms) + i + 1), 'C',[]) for i in range(0,nc)]
            self.arms.append(branch)

            for i in range(2, nc + 1) :
                self.bond(branch[i-1], branch[i-0])

        return self

    def bounder(self,*arg) :
        # Creates new bounds between two atoms of existing branches.
        # Each argument is a tuple of four integers giving:
        #     c1 & b1: carbon and branch positions of the first atom
        #     c2 & b2: carbon and branch positions of the second atom
        # All positions are 1-indexed, meaning (1,1,5,3) will bound the first carbon of the first branch with the fifth of the third branch.
        if self.lock == True : raise LockedMolecule

        for (c1,b1,c2,b2) in arg :
            i1, i2 = self.arms[b1][c1], self.arms[b2][c2]
            a1, a2 = self.atoms[i1], self.atoms[i2]

            if i1 == i2 or not a1.valid_valence() or not a2.valid_valence() :
                raise InvalidBond

            self.bond(i1, i2)

        return self

    def mutate(self, *arg) :
        # Mutates the carbon nc in the branch nb to the chemical element elt(given as a string).
        # Don't forget that carbons and branches are 1-indexed.
        # This is mutation: the id number of the Atom instance stays the same. See the Atom class specs about that.
        if self.lock == True : raise LockedMolecule

        for (nc,nb,elt) in arg :
            [valence, weight] = table[elt]
            i1 = self.arms[nb][nc]

            if len(self.atoms[i1].edge) > valence :
                raise InvalidBond

            self.atoms[i1].element = elt

        return self

    def add(self, *arg) :
        # Adds a new Atom of kind elt (string) on the carbon nc in the branch nb.
        # Atoms added this way are not considered as being part of the branch they are bounded to and aren't considered a new branch of the molecule.
        if self.lock == True : raise LockedMolecule

        for (nc,nb,elt) in arg :
            a1 = Atom(len(self.atoms) + 1, elt, [])
            a2 = self.atoms[self.arms[nb][nc]]

            if not a1.valid_valence() or not a2.valid_valence() :
                raise InvalidBond


            self.atoms.append(a1)
            self.bond(self.arms[nb][nc], len(self.atoms) - 1)

        return self

    def add_chaining(self, nc, nb, *elts) :
        # Adds on the carbon nc in the branch nb a chain with all the provided elements, in the specified order. Meaning:
        # m.add_chaining(2, 5, "N", "C", "C", "Mg", "Br") will add the chain ...-N-C-C-Mg-Br to the atom number 2 in the branch 5.
        # As for the add method, this chain is not considered a new branch of the molecule.
        if self.lock == True : raise LockedMolecule

        a1 = self.atoms[self.arms[nb][nc]]
        for elt in elts :
            if not a1.valid_valence() : raise InvalidBond
            a2 = Atom(len(self.atoms) + 1, elt, [a1])
            a1.edge.append(a2)
            self.atoms.append(a1)

        return self

    def closer(self) : # add missing missing hydrogen and lock molecule
        if self.lock == True : raise LockedMolecule
        self.lock = True
        hist = {}

        for atom in self.atoms :
            while atom.valid_valence() :
                hydr = Atom(len(self.atoms) + 1, 'H', [atom])
                atom.edge.append(hydr)
                self.atoms.append(hydr)
            atom.edge.sort(key = lambda x: x.id  )
            atom.edge.sort(key = lambda x: ( len(x.element),x.element) if x.element != 'H' else (x.id, 'Z')  )

            hist[atom.element] = 1 if atom.element not in hist else hist[atom.element] + 1

        arr = [(key,hist[key]) for key in hist]
        arr.sort(key = lambda x: (len(x[0]), x[0]))

        for elt, nb in arr :
            self.formula += elt + (str(nb) if nb > 1 else '')
            self.molecular_weight += table[elt][1] * nb

        return self

    def unlock (self) :
        # makes the molecule mutable again and remove hydrogen atoms
        if self.lock == False : raise UnlockedMolecule
        self.lock = False
        self.atoms = [ atom for atom in self.atoms if atom.element != 'H' ]

        for atom in self.atoms :
            atom.edge = [nxt for nxt in atom.edge if nxt.element != 'H']

        return self

################################################################################
class display :
    def branches(molecule) :
        print(molecule.name, molecule.formula, molecule.molecular_weight)
        m = molecule
        for i in range(1, len(m.arms)) :
            for j in range(1, len(m.arms[i])) :
                atom = m.atoms[m.arms[i][j]]

                print(atom)
            print()

    def atoms(molecule) :
        for atom in molecule.atoms :
            print(atom)

m = Molecule('').brancher( *(1,) ) .mutate( *((1, 1, 'B'),) ).closer()
print(m.formula, m.molecular_weight)

display.branches(m)

# for i in range(0,len(expect)) :
#     if actual[i] != expect[i] :
#         print('actuel : ', actual[i])
#         print('expect : ', expect[i])


print("end")
