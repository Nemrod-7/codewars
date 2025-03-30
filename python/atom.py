
from molecules import *
##############################################################################################
# Symbol:           H     B     C     N     O     F    Mg     P     S    Cl    Br            #
# Valence number:   1     3     4     3     2     1     2     3     2     1     1            #
# Atomic weight:  1.0  10.8  12.0  14.0  16.0  19.0  24.3  31.0  32.1  35.5  80.0  (in g/mol)#
##############################################################################################


table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1] }

order = ['C','H','O','B','Br','Cl','F','Mg','N','P','S']

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
        prefix = 'Atom(' + self.element + '.' + str(self.id)
        edge =  ['H' if bond.element == 'H' else bond.element + str(bond.id) for bond in self.edge]

        if len(edge) > 0 : prefix += ': '
        return prefix + ','.join(edge) + ')'

    def bond(atom1, atom2) :
        a1, a2 = atom1, atom2
        atom1.edge.append(a2)
        atom2.edge.append(a1)

    def valid_valence(self) :
        return len(self.edge) < table[self.element][0]

class Molecule() :
    def __init__ (self, name='') :
        self.name = name

        self.lock = False
        self.atoms = []
        self.arms = [[0]]

        print('Molecule(\'\')',end='')
    def bond(self, i1, i2) :
        a1, a2 = self.atoms[i1], self.atoms[i2]
        self.atoms[i1].edge.append(a2)
        self.atoms[i2].edge.append(a1)

    @property
    def formula(self) :
        if not self.lock : raise UnLockedMolecule
        hist = {}
        formula = ''
        
        for atom in self.atoms :
            hist[atom.element] = 1 if atom.element not in hist else hist[atom.element] + 1

        arr = [(key,hist[key]) for key in hist]
        arr.sort(key = lambda x: order.index(x[0]))

        for elt, nb in arr :
            formula += elt + (str(nb) if nb > 1 else '')
     
        return formula
    
    @property
    def molecular_weight (self) :
        if not self.lock : raise UnLockedMolecule
        return sum( table[atom.element][1] for atom in self.atoms)
    
    def brancher(self, *arg) :
        if self.lock : raise UnlockedMolecule
        print('.brancher(*',arg,')',end='')

        for nc in arg :
            branch = [0] + [len(self.atoms) + i for i in range(0, nc)]
            self.atoms += [Atom((len(self.atoms) + i + 1), 'C',[]) for i in range(0,nc)]
            self.arms.append(branch)

            for i in range(2, nc + 1) :
                Atom.bond(self.atoms[branch[i-1]], self.atoms[branch[i-0]])
                # self.bond(branch[i-1], branch[i-0])

        return self

    def bounder(self, *arg) :
        if self.lock : raise LockedMolecule
        print('.bounder(*',arg,')',end='')
        for c1,b1,c2,b2 in arg :
            # i1, i2 = self.arms[b1][c1], self.arms[b2][c2]
            a1, a2 = self.atoms[self.arms[b1][c1]], self.atoms[self.arms[b2][c2]]

            if a1 == a2 or not a1.valid_valence() or not a2.valid_valence() :
                raise InvalidBond

            Atom.bond(a1,a2)
            # self.bond(i1, i2)

        return self

    def mutate(self, *arg) :
        if self.lock : raise LockedMolecule

        print('.mutate(*',arg,')', end='')
        for nc,nb,elt in arg :
            [valence, weight] = table[elt]
            i1 = self.arms[nb][nc]

            if len(self.atoms[i1].edge) > valence :
                raise InvalidBond

            self.atoms[i1].element = elt

        return self

    def add(self, *arg) :
        if self.lock : raise LockedMolecule
        print('.add(*',arg,')', end='')
        for nc,nb,elt in arg :
            a1 = Atom(len(self.atoms) + 1, elt, [])
            a2 = self.atoms[self.arms[nb][nc]]

            if not a1.valid_valence() or not a2.valid_valence() :
                raise InvalidBond

            self.atoms.append(a1)
            self.bond(self.arms[nb][nc], len(self.atoms) - 1)

        return self

    def add_chaining(self, nc, nb, *elts) :
        if self.lock : raise LockedMolecule
        print('.add_chaining(',nc,',',nb,',*', elts, ')',end='')

        i1 = self.arms[nb][nc]
        i2 = len(self.atoms) + 1

        self.atoms += [Atom(i2 + i, elts[i], []) for i in range(0,len(elts))]
        for i in range(i2, len(self.atoms)) : self.bond(i-1, i)

        self.bond(i1, i2-1)
        return self

    def closer(self) : 
        if self.lock : raise LockedMolecule
        self.lock = True
        hist = {}
        print('.closer()')
        for atom in self.atoms :
            while atom.valid_valence() :
                hydr = Atom(len(self.atoms) + 1, 'H', [atom])
                atom.edge.append(hydr)
                self.atoms.append(hydr)
            atom.edge.sort(key = lambda x: x.id  )
            atom.edge.sort(key = lambda x: order.index(x.element) if x.element != 'H' else len(order)  )

        return self

    def unlock (self) :
        if self.lock : raise UnlockedMolecule

        print('.unlock()')
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

try :
    m = Molecule('').brancher(* (4,) ).closer()


    print(m.formula, m.molecular_weight)
    display.atoms(m)

except Exception as x:
    print('error :', x)


# display.branches(m)

# for i in range(0,len(expect)) :
#     if actual[i] != expect[i] :
#         print('actuel : ', actual[i])
#         print('expect : ', expect[i])


print("end")
