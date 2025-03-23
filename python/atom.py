
from molecules import *

# Symbol:           H     B     C     N     O     F    Mg     P     S    Cl    Br
# Valence number:   1     3     4     3     2     1     2     3     2     1     1
# Atomic weight:  1.0  10.8  12.0  14.0  16.0  19.0  24.3  31.0  32.1  35.5  80.0  (in g/mol)

table = { 'H':[1, 1.0 ], 'B':[3, 10.8], 'C':[4, 12.0], 'N':[3, 14.0], 'O':[2, 16.0], 'F':[1, 19.0], 'P':[3, 31.0], 'S':[2, 32.1], 'Mg':[2, 24.3], 'Cl':[1, 35.5], 'Br':[1, 80.0] }

class Atom() :
    def __init__(self, id, elt, adj) :
        self.id = id
        self.element = elt
        self.edge = adj

    def show(self) :
        at = self.element + str(self.id)
        return [at, [ix.id for ix in self.edge]]

class Molecule () :
    def __init__ (self, name='') :
        self.name = name
        self.formula = ''
        self.molecular_weight = 0.0

        self.lock = False
        self.arms = [[0]]
        self.atoms = [Atom(0,'C',[])]

    def getcarbons(self, nb) :
        return [ i for i in range(0, len(self.arms[nb])) if self.arms[nb][i].element == 'C']

    def bond(self, i1, i2) :
        a1, a2 = self.atoms[i1], self.atoms[i2]
        # self.atoms[i1].edge.append(a2)
        # self.atoms[i2].edge.append(a1)
        a1.edge.append(a2.id)
        a2.edge.append(a1.id)
        

    def brancher(self, *arg) :
        # Adds new "branches" to the current molecule.
        # Can take any number of arguments (positive integers).
        # Each argument gives the number of carbons of the new branch.
        if self.lock == True : raise 'error'
        
        for nc in arg :
            branch = [0] + [len(self.atoms) + i for i in range(0, nc)]
            self.atoms += [Atom(len(self.atoms) + i, 'C',[]) for i in range(0,nc)]
            self.arms.append(branch)

            for i in range(2, nc + 1) : 
                self.bond(branch[i-1], branch[i-0])
        
        for i in range(1,len(self.atoms)) :
            atom = self.atoms[i]
            # print(atom.element, atom.id, len(atom.edge))

        return self

    def bounder(self,*arg) :
        # Creates new bounds between two atoms of existing branches.
        # Each argument is a tuple (python) of four integers giving:
        #     c1 & b1: carbon and branch positions of the first atom
        #     c2 & b2: carbon and branch positions of the second atom
        # All positions are 1-indexed, meaning (1,1,5,3) will bound the first carbon of the first branch with the fifth of the third branch.
        if self.lock == True : raise 'error'

        for (c1,b1,c2,b2) in arg :
            # i1 = [ix for ix in self.arms[b1] if self.atoms[ix].element == 'C']
            # i2 = [ix for ix in self.arms[b2] if self.atoms[ix].element == 'C']
            # self.bond(i1[c1], i2[c2])

            i1, i2 = self.arms[b1][c1], self.arms[b2][c2]
            self.bond(i1, i2)

        return self

    def mutate(self, *arg) :
        # Mutates the carbon nc in the branch nb to the chemical element elt(given as a string).
        # Don't forget that carbons and branches are 1-indexed.
        # This is mutation: the id number of the Atom instance stays the same. See the Atom class specs about that.

        for (nc,nb,elt) in arg :
            # carbon = [ix for ix in self.arms[nb] if self.atoms[ix].element == 'C']
            # if nc < len(carbon) : self.atoms[carbon[nc]].element = elt

            self.atoms[self.arms[nb][nc]].element = elt

        return self

    def add(self, *arg) :
        # Adds a new Atom of kind elt (string) on the carbon nc in the branch nb.
        # Atoms added this way are not considered as being part of the branch they are bounded to and aren't considered a new branch of the molecule.

        for (nc,nb,elt) in arg :
            self.atoms.append( Atom(len(self.atoms), elt, []))
            self.bond(i1, self.atoms[-1].id)


        return self

    def add_chaining(self) :

        return self

    def closer(self) : # add missing missing hydrogen and lock molecule
        self.lock = True
        hist = {}

        for ix in range (1, len(self.atoms)) :
            atom = self.atoms[ix]

            while len(atom.edge) < table[atom.element][0] :
                edge = Atom(len(self.atoms), 'H', [atom.id])
                atom.edge.append(len(self.atoms))

                self.atoms.append(edge)

        for ix in range (1, len(self.atoms)) :
            atom = self.atoms[ix]
            hist[atom.element] = 1 if atom.element not in hist else hist[atom.element] + 1

        arr = [(key,hist[key]) for key in hist]
        arr.sort()

        for [elt, nb] in arr :
            self.formula += elt + (str(nb) if nb > 1 else '')
            self.molecular_weight += table[elt][1] * nb

        return self

    def unlock (self) : 
        # makes the molecule mutable again and remove hydrogen atoms
        self.lock = False
        atoms = [ atom for atom in self.atoms if atom.element != 'H' ]

        return self

    def show(self) :
        for i in range(1, len(self.arms)) :
            arm = self.arms[i]
            for atom in arm : atom.show()
            print()


def display(molecule) :
    print(molecule.name, molecule.formula, molecule.molecular_weight)

    for i in range(1, len(molecule.arms)) :
        for j in range(1, len(molecule.arms[i])) :
            atom = molecule.atoms[molecule.arms[i][j]]
            curr = atom.element + '.' + str(atom.id)
            edge = ','.join([edge.element + (str(edge.id) if edge.element != 'H' else '') for edge in atom.edge])
            print(curr,':', edge)
        print()


# for elt in table : print(elt, table[elt])
formula = 'C10H16N2O3S'

biotin = Molecule("biotin")
biotin.brancher(14,1,1)
biotin.bounder((2,1,1,2), (2,1,1,2), (10,1,1,3), (10,1,1,3), (8,1,12,1), (7,1,14,1))
biotin.mutate( (1,1,'O'),  (1,2,'O'), (1,3,'O'), (11,1,'N'), (9,1,'N'), (14,1,'S'))
biotin.closer()

# display(biotin)
m = biotin

for atom in m.atoms :
    curr = atom.element + '.' + str(atom.id)
    edge = ','.join([m.atoms[edge].element + (str(m.atoms[edge].id) if m.atoms[edge].element != 'H' else '') for edge in atom.edge])

    if atom.element != 'H' : print(curr,':', edge)

        

print("end")
