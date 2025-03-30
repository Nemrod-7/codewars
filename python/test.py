table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1] }

order = ['C','H','O','B','Br','Cl','F','Mg','N','P','S']

class Atom() :
    def __init__(self, id, elt, adj) :
        self.id = id
        self.element = elt
        self.edge = adj
        self.str = ''

    def __hash__(self):      return self.id
    def __eq__(self, other): return self.id == other.id

    def __str__(self) :
        prefix = 'Atom(' + self.element + '.' + str(self.id)
        edge =  ['H' if bond.element == 'H' else bond.element + str(bond.id) for bond in self.edge]

        if len(edge) > 0 : prefix += ': '

        return prefix + ','.join(edge) + ')'

class InvalidBond(Exception):
    def __init__(self, *args: object) -> None:
        super().__init__(*args)


class Molecule() :
    def __init__(self) :
        self.data = 0
        self.lock = False

    def make(self) :
        raise InvalidBond('This is a test')

    def func(self) :
        self.data = 2;
        return self

    @property
    def formula(self) :
        return 'molecule formula'

m = Molecule().make()
m.func()

print(m.formula)
