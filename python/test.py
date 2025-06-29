
N = 4

def showmask(cell) :
    for i in range(1,N+1) :
        if (cell >> i&1) == 1 :
            print(i, end=' ')
        else :
            print(' ', end=' ')

cell = 16
row, col = 2, 0
row |= cell
# col |= 1 << 1

# cell = 30 ^ (row | col)

showmask(row)
print()
showmask(col)
print()
print('----------')
showmask(cell)
