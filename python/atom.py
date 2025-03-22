

# Symbol:           H     B     C     N     O     F    Mg     P     S    Cl    Br
# Valence number:   1     3     4     3     2     1     2     3     2     1     1
# Atomic weight:  1.0  10.8  12.0  14.0  16.0  19.0  24.3  31.0  32.1  35.5  80.0  (in g/mol)

table = { 'H':[1, 1.0 ], 'B':[3, 10.8], 'C':[4, 12.0], 'N':[3, 14.0], 'O':[2, 16.0], 'F':[1, 19.0], 'P':[3, 31.0], 'S':[2, 32.1], 'Mg':[2, 24.3], 'Cl':[1, 35.5], 'Br':[1, 80.0] }

class Atom() :
    def __init__(self, id, elt) :
        self.id = id
        self.element = elt
        self.edge = []

    def show(self) :
        print('[',self.id, self.element, ']', end='')

class Molecule () :
    def __init__ (self, name='') :
        self.name = name
        self.formula = ''
        self.molecular_weight = 0.0

        self.arms = [[Atom(0,'C')]]
        self.atoms = [Atom(0,'C')]

    def getcarbons(self, nb) :
        return [ i for i in range(0, len(self.arms[nb])) if self.arms[nb][i].element == 'C']

    def brancher(self, *arg) :
        # Adds new "branches" to the current molecule.
        # Can take any number of arguments (positive integers).
        # Each argument gives the number of carbons of the new branch.
        
        for nc in arg :
            branch = [Atom(len(self.atoms) + i, 'C') for i in range(0, nc)]
            self.atoms += branch
            self.arms.append(branch)

        self.show()
        
    def bounder(self,*arg) : # Creates new bounds between two atoms of existing branches.

        # Creates new bounds between two atoms of existing branches.
        # Each argument is a tuple (python), array (ruby/JS), or T object (java) of four integers giving:
        #     c1 & b1: carbon and branch positions of the first atom
        #     c2 & b2: carbon and branch positions of the second atom
        # All positions are 1-indexed, meaning (1,1,5,3) will bound the first carbon of the first branch with the fifth of the third branch.

        for (c1,b1,c2,b2) in arg :
            i1 = self.getcarbons(b1)
            i2 = self.getcarbons(b2)

            a1 = self.arms[b1][i1[c1]]
            a2 = self.arms[b2][i2[c2]]
            #
            # self.arms[b1][c1].edge += a2
            # self.arms[b2][c2].edge += a1

        pass

    def mutate(self) :

        pass

    def add(self) :

        pass

    def add_chaining(self) :

        pass

    def closer(self) : # add missing missing hydrogen and lock molecule


        self.lock = True

    def unlock (self) : # makes the molecule mutable again and remove hydrogen atoms
        self.lock = False

    def show(self) :
        for i in range(1, len(self.arms)) :
            arm = self.arms[i]
            for atom in arm : atom.show()
            print()

biotin = Molecule("biotin")
biotin.brancher(14,1,1)
# biotin.bounder((2,1,1,2), (2,1,1,2),
#                (10,1,1,3), (10,1,1,3),
#                (8,1,12,1), (7,1,14,1))

# biotin.mutate( (1,1,'O'),  (1,2,'O'), (1,3,'O'),
#                (11,1,'N'), (9,1,'N'), (14,1,'S'))
biotin.closer()

# for elt in table : print(elt, table[elt])
