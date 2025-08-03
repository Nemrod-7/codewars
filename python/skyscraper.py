import time
start = time.time()

N = 7

def powerof2 (x) : return x != 0 and (x & (x - 1)) == 0
def exist (x, bit) : return (x >> bit &1) == 1
def decompose (cell) :
    dig, vs = 0, []

    while cell :
        if (cell &1 and dig > 0) : vs.append(dig)
        dig += 1
        cell >>= 1
    return vs

def vertical (clues, i) :
    south, north = ((N * 4) - 1) - i - N, i
    return (clues[north], clues[south])
def horizont (clues, i) :
    west, east = ((N * 4) - 1) - i, N + i
    return (clues[west], clues[east])
def equals (a, b) :
    if b[0] == 0 and b[1] == 0 :
        return True
    elif b[0] == 0 and a[1] == b[1] :
        return True
    elif b[1] == 0 and a[0] == b[0] :
        return True
    elif a[0] == 0 and a[1] == b[1] :
        return True
    elif a[1] == 0 and a[0] == b[0] :
        return True
    elif a == b :
        return True
    return False
def check_line (now) :
    end, first, sec, index = N - 1, 0, 0, N - 1
    head, tail = 0,0

    while index >= 0 :
        if now[index] == 0 : return (-1,-1)
        if now[index] > sec :
            sec = now[index]
            tail += 1
        if now[N - 1 - index] > first :
            first = now[end - index]
            head += 1
        index -= 1
    return (head,tail)
def mk_cell (mask, clue, p, dir) :
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
def mk_mask (clues) :
    mask = [ [254 for x in range(N)] for y in range(N) ]

    for i in range(N) :
        [north,south] = vertical(clues, i)
        [west, east] = horizont(clues, i)
        mk_cell(mask, west, (0,i), (1, 0))
        mk_cell(mask, north, (i,0), (0,1))
        mk_cell(mask, east, (N-1, i), (-1,0))
        mk_cell(mask, south, (i, N - 1), (0,-1))
    return mask
def eliminate (line) : # search for uniques digit in a row or a column and reajust the mask
    uniq, hist = [False] * (N + 1), [0] * (N + 1)

    for i in range(N) :
        cell, bit = line[i], 0

        if powerof2(cell) :
            uniq[decompose(cell)[0]] = True

        while cell :
            if (cell & 1) : hist[bit] += 1
            bit += 1
            cell >>= 1

    for i in range(N) :
        if not powerof2(line[i]) : # if bit count > 1
            for j in range(1, 1 + N) :
                if exist(line[i], j) :
                    if uniq[j] == 1 : line[i] &= ~(1 << j) # clear bit
                    if hist[j] == 1 : line[i] = 0 | (1 << j) # mark bit
    return line
def subset_permutation (mask, clues) :
    start = [0] * N
    heap = [(0,0, start.copy())]

    while heap :
        [index, visit, comb] = heap.pop()

        if index == N :
            if equals(check_line(comb), clues) :
                for i in range(N) : start[i] |= 1 << comb[i]
        else :
            for dig in decompose(mask[index]) :
                if not exist(visit, dig) :
                    comb[index] = dig
                    heap.append( ( index + 1, (visit | 1 << dig), comb.copy()))
    return start
def reduce (mask) : # apply the search for unique digits on the grid
    for i in range(N) :
        col = eliminate([mask[j][i] for j in range(N)])
        for j in range(N) :
            mask[j][i] = col[j]

    for i in range(N) :
        row = eliminate([mask[i][j] for j in range(N)])
        for j in range(N) :
            mask[i][j] = row[j]
def filtering (mask, clues) : # apply the search for right combinations on the grid
    m1, m2 = mask.copy(), mask.copy()
    grid = [ [0 for x in range(N)] for y in range(N) ]

    for i in range(N) :
        vert = vertical(clues, i)
        hori = horizont(clues, i)

        if hori[0] != 0 or hori[1] != 0 :
            m1[i] = subset_permutation(mask[i], hori)

        if vert[0] != 0 or vert[1] != 0 :
            array = subset_permutation([mask[j][i] for j in range(N)], vert)
            for j in range(N) : m2[j][i] = array[j]

    for y in range(N) :
        for x in range(N) :
            grid[y][x] = m1[y][x] & m2[y][x]
    return grid

def backtrack (grid, cells, clues, row, col, index) :
    if index == N * N :
        return True
        # return valid_grid(grid, clues)

    x, y = index % N, index // N
    # if grid[y][x] != 0 : return backtrack(grid, cells, clues, row, col, index + 1)
    for dig in decompose(cells[y][x]) :
        if not exist(row[y], dig) and not exist(col[x], dig) :
            row[y] ^= 1 << dig; col[x] ^= 1 << dig
            grid[y][x] = dig

            if backtrack(grid, cells, clues, row, col, index + 1) :
                return True

            grid[y][x] = 0
            row[y] ^= 1 << dig; col[x] ^= 1 << dig
    return False
def valid_grid(grid, clues) :
    for i in range(N) :
        column = [grid[j][i] for j in range(N)]

        if not equals(check_line(grid[i]), horizont(clues, i)) :
            return False

        if not equals(check_line(column), vertical(clues, i)) :
            return False
    return True

def solve_puzzle (clues):
    row, col = [0] * N, [0] * N
    grid = [ [0 for x in range(N)] for y in range(N) ]
    mask = mk_mask(clues)

    for _ in range(8) :
        for i in range(4) : 
            reduce(mask)
        mask = filtering(mask, clues)

    heap = [0]

    while heap :
        index = heap.pop()
        x, y = index % N, index // N

        if index == N * N :
            valid_grid(grid, clues)

        for dig in decompose(mask[y][x]) :
            row[y] ^= 1 << dig; col[x] ^= 1 << dig
            grid[y][x] = dig

    # backtrack(grid, mask, clues, row, col, 0) # eventually fill the unsolved cells
    # display_mask(mask)
    # display_grid(clues,mask)
    return grid

def backtrack (grid, cells, clues, row, col, index) :
    if index == N * N :
        return valid_grid(grid, clues)

    x, y = index % N, index // N
    # if grid[y][x] != 0 : return backtrack(grid, cells, clues, row, col, index + 1)
    for dig in decompose(cells[y][x]) :
        if not exist(row[y], dig) and not exist(col[x], dig) :
            row[y] ^= 1 << dig; col[x] ^= 1 << dig
            grid[y][x] = dig

            if backtrack(grid, cells, clues, row, col, index + 1) :
                return True

            grid[y][x] = 0
            row[y] ^= 1 << dig; col[x] ^= 1 << dig
    return False

def showmask(cell) :
    for i in range(1,N+1) :
        bit = (cell >> i&1)
        # print(bit, end=' ')
        if bit == 1 :
            print(i, end=' ')
        else :
            print(' ', end=' ')
def display_mask(mask) :
    print()

    for y in range(N) :
        for x in range(N) :
            showmask(mask[y][x])
            print('|', end=' ')
        print()
    print()
def display_grid(clues,mask) :
    grid = [ [0 for x in range(N + 2)] for y in range(N + 2) ]

    for i in range(N) :
        [north,south] = vertical(clues, i)
        [west, east] = horizont(clues, i)

        grid[0][i + 1] = north
        grid[N + 1][i + 1] = south
        grid[i + 1][0] = west
        grid[i + 1][N + 1] = east

        for j in range(N) :
            if powerof2( mask[i][j]) :
                grid[i + 1][j + 1] = decompose(mask[i][j])[0]

    for i in range(N + 2)  :
        for j in range(N + 2) :
            if grid[i][j] :
                print(grid[i][j], end=' ')
            elif i > 0 and j > 0 and i <= N and j <= N:
                print('.', end=' ')
            else :
                print(' ', end=' ')
        print()
    print()

def do_test(actual, expected) :
    print(actual, "\n", expected)
    pass

# medved
actual = solve_puzzle( [3,3,2,1,2,2,3, 4,3,2,4,1,4,2, 2,4,1,4,5,3,2, 3,1,4,2,5,2,3])
expect = [
 [2, 1, 4, 7, 6, 5, 3], 
 [6, 4, 7, 3, 5, 1, 2],
 [1, 2, 3, 6, 4, 7, 5],
 [5, 7, 6, 2, 3, 4, 1], 
 [4, 3, 5, 1, 2, 6, 7],
 [7, 6, 2, 5, 1, 3, 4],
 [3, 5, 1, 4, 7, 2, 6]]

do_test(actual, expect)

end = time.time()
print( "elapsed : ", end - start)
