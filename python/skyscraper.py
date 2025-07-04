import itertools
import time
start = time.time()

N = 6
comb = list(itertools.permutations([i + 1 for i in range(N)]))

# def hasonebit (x) : return x != 0 and (x & (x - 1)) == 0
def exist (x, bit) : return (x >> bit &1) == 1
def bit2int (x) :
    dig = 0
    while x :
        if (x &1) : return dig
        dig += 1
        x >>= 1
    return 0

def filter (line) :
    uniq, hist = [False] * (N + 1), [0] * (N + 1)

    for i in range(N) :
        cell, bit = line[i], 0

        if (cell &(cell - 1)) == 0 :
            uniq[bit2int(cell)] = True

        while cell :
            if (cell & 1) : hist[bit] += 1
            bit += 1
            cell >>= 1

    for i in range(N) :
        cell = line[i]

        if (cell &(cell - 1)) : # if bit count > 1
            for j in range(1, 1 + N) :
                if exist(cell, j) :
                    if uniq[j] == 1 : line[i] &= ~(1 << j) # clear bit
                    if hist[j] == 1 : line[i] = 0 | (1 << j) # mark bit

    return line

def reduce (mask) :
    for i in range(N) :
        col = filter([mask[j][i] for j in range(N)])
        for j in range(N) :
            mask[j][i] = col[j]

    for i in range(N) :
        row = filter([mask[i][j] for j in range(N)])
        for j in range(N) :
            mask[i][j] = row[j]

def equals (a, b) :
    if b[0] == 0 and b[1] == 0 :
        return True
    elif b[0] == 0 and a[1] == b[1] :
        return True
    elif b[1] == 0 and a[0] == b[0] :
        return True
    elif a == b :
        return True
    return False

def check_num (now) :
    end, first, sec, index = N - 1, 0, 0, N - 1
    head, tail = 0,0

    while index >= 0 :
        if now[index] > sec :
            sec = now[index]
            tail += 1
        if now[N - 1 - index] > first :
            first = now[end - index]
            head += 1
        index -= 1

    return (head,tail)

def valid_grid(grid, clues) :
    for i in range(N) :
        west, east = ((N * 4) - 1) - i, N + i
        south, north = ((N * 4) - 1) - i - N, i
        column = [grid[j][i] for j in range(N)]

        if not equals(check_num(grid[i]), (clues[west], clues[east])) :
            return False

        if not equals(check_num(column), (clues[north], clues[south])) :
            return False

    return True

def backtrack(grid, cells, clues, row, col, index) :
    if index == N * N :
        return valid_grid(grid, clues)

    dig = 0
    x, y = index % N, index // N
    cell, mask = grid[y][x], cells[y][x]

    if cell != 0 : return backtrack(grid, cells, clues, row, col, index + 1)

    for dig in mask :
        if not exist(row[y], dig) and not exist(col[x], dig) :
            row[y] ^= 1 << dig; col[x] ^= 1 << dig
            grid[y][x] = dig

            if backtrack(grid, cells, clues, row, col, index + 1) :
                return True

            grid[y][x] = 0
            row[y] ^= 1 << dig; col[x] ^= 1 << dig

    return False

def mkmask2(clues) :
    mask1 = [ [0 for x in range(N)] for y in range(N) ]
    mask2 = [ [0 for x in range(N)] for y in range(N) ]
    grid = [ [0 for x in range(N)] for y in range(N) ]

    for i in range(N) :
        west, east = ((N * 4) - 1) - i, N + i
        south, north = ((N * 4) - 1) - i - N, i

        horiz = (clues[west], clues[east])
        verti = (clues[north], clues[south])

        for actual in comb :
            if equals(check_num(actual), horiz) :
                for j in range(N) : mask1[i][j] |= 1 << actual[j]

            if equals(check_num(actual), verti) :
                for j in range(N) : mask2[j][i] |= 1 << actual[j]

    for y in range(N) :
        for x in range(N) :
            grid[y][x] = mask1[y][x] & mask2[y][x]

    return grid

def solve_puzzle (clues):
    grid = [ [0 for x in range(N)] for y in range(N) ]
    comb = [ [[] for x in range(N)] for y in range(N) ]
    row, col = [0] * N, [0] * N
    mask = mkmask2(clues)

    # for i in range(3) : reduce(mask)
    for i in range(4) :
        reduce(mask)

    for y in range(N) :
        for x in range(N) :
            for i in range(1,N+1) :
                if (mask[y][x] >> i&1) :
                    comb[y][x].append(i)

    backtrack(grid, comb, clues, row, col, 0)

    # print(clues)
    # print()
    # display(mask)
    # print()
    #
    # for line in grid :
    #     print(line)

    return tuple(tuple(line) for line in grid)

def mk_cell(mask, clue, p, dir) :
    x,y = p

    if clue == 1 :
        mask[y][x] = 0 | 1 << N
    elif clue == N :
        for i in range(1, N+1) :
            mask[y][x] = 0 | 1 << i
            x += dir[0]; y += dir[1]
    else :
        maxv = N - clue + 2

        for i in range(clue-1) :
            for j in range(maxv + i, N + 1) :
                mask[y][x] &= ~(1 << j)
            mask[y][x] &= ~(1 << N)
            x += dir[0]; y += dir[1]
def showmask(cell) :
    for i in range(1,N+1) :
        bit = (cell >> i&1)
        # print(bit, end=' ')
        if bit == 1 :
            print(i, end=' ')
        else :
            print(' ', end=' ')

def display(grid) :
    print()

    for y in range(N) :
        for x in range(N) :
            showmask(grid[y][x])
            print('|', end=' ')
        print()


expected = (
    (2, 1, 4, 3, 5, 6),
    (1, 6, 3, 2, 4, 5),
    (4, 3, 6, 5, 1, 2),
    (6, 5, 2, 1, 3, 4),
    (5, 4, 1, 6, 2, 3),
    (3, 2, 5, 4, 6, 1)
)
clues = (3,2,2,3,2,1, 1,2,3,3,2,2, 5,1,2,2,4,3, 3,2,1,2,2,4)
actual = solve_puzzle(clues)

expected = (
    (5, 6, 1, 4, 3, 2),
    (4, 1, 3, 2, 6, 5),
    (2, 3, 6, 1, 5, 4),
    (6, 5, 4, 3, 2, 1),
    (1, 2, 5, 6, 4, 3),
    (3, 4, 2, 5, 1, 6)
)
clues = (0,0,0,2,2,0, 0,0,0,6,3,0, 0,4,0,0,0,0, 4,4,0,3,0,0)
actual = solve_puzzle(clues)

expected = (
    (5, 2, 6, 1, 4, 3),
    (6, 4, 3, 2, 5, 1),
    (3, 1, 5, 4, 6, 2),
    (2, 6, 1, 5, 3, 4),
    (4, 3, 2, 6, 1, 5),
    (1, 5, 4, 3, 2, 6)
)
clues = (0,3,0,5,3,4, 0,0,0,0,0,1, 0,3,0,3,2,3, 3,2,0,3,1,0)
actual = solve_puzzle(clues)




end = time.time()
print( "elapsed : ", end - start)
