
atom1 = [0, 'C', []]
atom2 = [1, 'O', []]
atom3 = copy.deepcopy(atom1)

atom1[2].append('H')

print(atom3)
