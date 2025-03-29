table = {
    'C':[4, 12.0],
    'H':[1, 1.0 ],
    'O':[2, 16.0],
    'B':[3, 10.8],
    'Br':[1, 80.0],
    'Cl':[1, 35.5],
    'F':[1, 19.0],
    'Mg':[2, 24.3],
    'N':[3, 14.0],
    'P':[3, 31.0],
    'S':[2, 32.1],
}


order = ['C', 'H', 'O',  'B', 'Br',   ]
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

    def valid_valence(self) :
        return len(self.edge) < table[self.element][0]

# Testing raw formula with mutation: 'BH3' should equal 'H3B'
# Testing raw formula with mutation: 'FH' should equal 'HF'
# Testing raw formula with addition of Br: 'FBr' should equal 'BrF'
# Testing raw formula with addition of Br: 'NBr3' should equal 'Br3N'
# Testing raw formula with addition of Br: 'PBr3' should equal 'Br3P'
# Testing raw formula with addition of Br: 'SBr2' should equal 'Br2S'

atom = Atom(6, 'C', [ Atom(5, 'C', []), Atom(2, 'C', []), Atom(1, 'O', []), Atom(7, 'H', []), Atom(3, 'Br',[]) ])

# atom.edge.sort(key = lambda x: x.id  )
# atom.edge.sort(key = lambda x: ( len(x.element),x.element) if x.element != 'H' else (x.id, 'Z')  )
# atom.edge.sort(key = lambda x: table[x.element][1] )


#     Raw formula of organic compounds:
#
# The raw formula gives the number of each kind of chemical element in the molecule.
# There are several possible rules when it comes to the order of the atoms in the raw formula.
# Here we will use the following: C, H, O, then other elements in alphabetic order. Examples: "C4H10" for C4H10, "C5H10O2BrClS", ...
# Note that the ones are not appearing (Br and not Br1).

formula = 'C4H10'

for ch in formula :
    if ch in table :
        print(table.index[ch])

# print(atom)
