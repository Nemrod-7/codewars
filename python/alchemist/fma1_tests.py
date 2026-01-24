
class test :
    def assert_equals (actual, expect, description = '') :
        if actual != expect :
            print (description)
            print('actual : ', actual)
            print('expect : ', expect)

""" methane:
  H
  |
H-C-H   <=>  CH4
  |
  H
"""

""" octane:
CH3-CH2-CH2-CH2-CH2-CH2-CH2-CH3
"""

# methane = Molecule("methane").brancher(1).closer()
# octane = Molecule("octane").brancher(8).closer()

def test_methane() :
    methane = Molecule("methane").brancher(1)
    try:    atoms = methane.atoms
    except: test.expect(False, "Couldn't access to the list of atoms")

    test.assert_equals(str(atoms[0]), 'Atom(C.1)')

    methane.closer()
    atoms = methane.atoms;
    # for atom in atoms : print(atom)

    test.assert_equals(len(atoms), 5, "Wrong number of atoms")

    for x,elt in enumerate("CHHHH"):
        test.assert_equals(atoms[x].element, elt, "Wrong atom at the index {} in self.atoms".format(x))
        test.assert_equals(atoms[x].id, x+1, "Wrong id value {} in self.atoms".format(x+1))


    test.assert_equals(str(atoms[0]), "Atom(C.1: H,H,H,H)")
    test.assert_equals(str(atoms[-1]), "Atom(H.5: C1)")


def extractNoneHToStr(m): return [str(at) for at in m.atoms if at.element != "H"]

def testThisMolecule(m, formula, mm, strNoH):
    test.assert_equals(m.formula, formula, "Testing raw formula")
    test.assert_equals(m.molecular_weight, mm, "Testing molecular weight")
    test.assert_equals(extractNoneHToStr(m), strNoH, "Checking bonds (for non-hydrogens)")






range1 = [

(""" cyclohexane:
CH2-CH2-CH2
|       |
CH2-CH2-CH2
""",
"cyclohexane",
[6],
[(1,1,6,1)],
'C6H12',
84,
["Atom(C.1: C2,C6,H,H)", "Atom(C.2: C1,C3,H,H)", "Atom(C.3: C2,C4,H,H)", "Atom(C.4: C3,C5,H,H)", "Atom(C.5: C4,C6,H,H)", "Atom(C.6: C1,C5,H,H)"]),


(""" 1,1-dimethyl-2-propylcyclohexane:
            CH3   CH3
               \\ /
CH3-CH2-CH2-CH2-CH2-CH2
            |       |
            CH2-CH2-CH2
""",
"2-propyl-1,1-dimethylcyclohexane",
[9,1,1],
[(4,1,9,1), (5,1,1,2), (5,1,1,3)],
'C11H22',
154,
['Atom(C.1: C2,H,H,H)', 'Atom(C.2: C1,C3,H,H)', 'Atom(C.3: C2,C4,H,H)', 'Atom(C.4: C3,C5,C9,H)', 'Atom(C.5: C4,C6,C10,C11)', 'Atom(C.6: C5,C7,H,H)', 'Atom(C.7: C6,C8,H,H)', 'Atom(C.8: C7,C9,H,H)', 'Atom(C.9: C4,C8,H,H)', 'Atom(C.10: C5,H,H,H)', 'Atom(C.11: C5,H,H,H)']),


(""" cubane (something like that, with one more "CH" group at the back of the cube...):
       CH-----CH
      /      /|
     /      / |
    CH-----CH CH
    |      | /
    |      |/
    CH-----CH
""","cubane - one branch",
[8],
[(3,1,6,1), (2,1,7,1), (1,1,8,1), (4,1,1,1), (5,1,8,1)],
"C8H8",
104,
['Atom(C.1: C2,C4,C8,H)', 'Atom(C.2: C1,C3,C7,H)', 'Atom(C.3: C2,C4,C6,H)', 'Atom(C.4: C1,C3,C5,H)', 'Atom(C.5: C4,C6,C8,H)', 'Atom(C.6: C3,C5,C7,H)', 'Atom(C.7: C2,C6,C8,H)', 'Atom(C.8: C1,C5,C7,H)']),


(""" cubane again: same than the one above!
""",
"cubane - two branches",
[4,4],
[(1,1,4,1), (1,2,4,2), (1,1,1,2), (2,1,2,2), (3,1,3,2), (4,1,4,2)],
"C8H8",
104,
['Atom(C.1: C2,C4,C5,H)', 'Atom(C.2: C1,C3,C6,H)', 'Atom(C.3: C2,C4,C7,H)', 'Atom(C.4: C1,C3,C8,H)', 'Atom(C.5: C1,C6,C8,H)', 'Atom(C.6: C2,C5,C7,H)', 'Atom(C.7: C3,C6,C8,H)', 'Atom(C.8: C4,C5,C7,H)']),

(""" benzene:
 CH-CH
//   \\
CH    CH
\\    //
 CH=CH
""",
"benzene: double bonds",
[2,2,2],
[(1,1,2,1), (1,2,2,2), (1,3,2,3), (2,1,1,2), (2,2,1,3), (2,3,1,1)],
"C6H6",
78,
['Atom(C.1: C2,C2,C6,H)', 'Atom(C.2: C1,C1,C3,H)', 'Atom(C.3: C2,C4,C4,H)', 'Atom(C.4: C3,C3,C5,H)', 'Atom(C.5: C4,C6,C6,H)', 'Atom(C.6: C1,C5,C5,H)']),


]



range2 = [
(""" Furane:
    O
  /   \
CH     CH
 \\   //
  CH-CH
""",
"Furane: no additional hydrogens while closing after mutation",
[5],
[(5,1,1,1), (5,1,4,1), (2,1,3,1)],
[(1,1,'O')],
"C4H4O",
68,
['Atom(O.1: C2,C5)', 'Atom(C.2: C3,C3,O1,H)', 'Atom(C.3: C2,C2,C4,H)', 'Atom(C.4: C3,C5,C5,H)', 'Atom(C.5: C4,C4,O1,H)']),


(""" isopropylmagnesium bromide:
CH3
  \
   CH-Mg-Br
  /
CH3
""",
"isopropylmagnesium bromide",
[4, 1],
[(2,1,1,2)],
[(3,1,'Mg'), (4,1,'Br')],
"C3H7BrMg",
147.3,
['Atom(C.1: C2,H,H,H)', 'Atom(C.2: C1,C5,Mg3,H)', 'Atom(Mg.3: C2,Br4)', 'Atom(Br.4: Mg3)', 'Atom(C.5: C2,H,H,H)'])
]


def carbohydrates () :
    print( "Create carbohydrates and bound them correctly (id tracking, raw formula and molecular weight tested)")
    for occ in range1 :
        [description, name, arms, bonds, formula, weight, atoms] = occ

        mole = Molecule(name).brancher(*arms).bounder(*bonds).closer()

        test.assert_equals(mole.formula, formula, description)
        test.assert_equals(mole.molecular_weight, weight, description)

def mutations () :
    print("Mutating, adding and valence numbers consistencies")

    for tes in range2:
        [desc,name,branch,bonds,mut,formula,mm,carbToStr] = tes
        m = Molecule(name).brancher(*branch).bounder(*bonds).mutate(*mut).closer()

        test.assert_equals(m.formula, formula, "Testing raw formula")
        test.assert_equals(m.molecular_weight, mm, "Testing molecular weight")
        test.assert_equals(extractNoneHToStr(m), strNoH, "Checking bonds (for non-hydrogens)")

config = [
("No self-bonding",
[6],
[(1,1,1,1)]),

("Should fail when exceeding the valence number adding new alkyls to the same atom",
[3,1,1,1],
[(2,1,1,2), (2,1,1,3), (2,1,1,4)]),

("Should fail when exceeding the valence number with multiple bonds",
[4],
[(2,1,3,1), (2,1,3,1), (2,1,3,1)]),
]


# for descript,branch,bonds in config:
#     try :
#         m = Molecule("").brancher(*branch).bounder(*bonds).closer()
#         func = lambda: Molecule("").brancher(*branch).bounder(*bonds).closer()
#     except :
#         print('got error')


config = [
("Should fail when mutating a carbon with three atoms already linked to an oxygen",
[3,1],
[(2,1,1,2)],
("mutate", (2,1,'O'))),

("Should fail when mutating a carbon with two double bonds to nitrogen",
[3],
[(1,1,2,1), (3,1,2,1)],
("mutate", (2,1,'N'))),

("Should fail when adding a new hydrogen to a carbon with already 4 bonds",
[3],
[(1,1,2,1), (3,1,2,1)],
("add", (2,1,'H'))),


("Should fail when mutating an atom and then adding too much atoms on it",
[3],
[(1,1,2,1)],
("mutadd", [(2,1,'N'), (2,1,'O')]) ),

]


# for descript,branch,bonds,(mutadd,inp) in config:
#     m = Molecule("").brancher(*branch).bounder(*bonds)
#
#     if mutadd == "mutate":   func = lambda: m.mutate(inp).closer()
#     elif mutadd == "add":    func = lambda: m.add(inp).closer()
#     elif mutadd == "mutadd": func = lambda: m.mutate(inp[0]).add(inp[1]).closer()
#     else:
#         pass
