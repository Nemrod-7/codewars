import itertools

# {{4,1,2,3},{4,2,1,3},{4,1,3,2},{4,2,3,1},{4,3,1,2},{4,3,2,1}},
# {{3,1,2,4},{3,2,1,4},{1,4,2,3},{2,1,4,3},{2,4,1,3},{3,1,4,2},{3,2,4,1},{3,4,1,2},{1,4,3,2},{2,4,3,1},{3,4,2,1}},
# {{1,3,2,4},{2,1,3,4},{2,3,1,4},{1,2,4,3},{1,3,4,2},{2,3,4,1}},
# {{1,2,3,4}}};

N = 4

def hasonebit (x) : return x != 0 and (x & (x - 1)) == 0
def exist (x, bit) : return (x >> bit &1) == 1
def bit2int (x) :
    dig = 0
    while x :
        if (x &1) : return dig
        dig += 1
        x >>= 1
    return 0

class Skyscraper () :
    def __init__(self, clues) :
        self.clue = clues
        self.grid = [ [0 for x in range(N)] for y in range(N) ]
        self.cell = [ [30 for x in range(N)] for y in range(N) ]

        for i in range(N) :   # make cells for vertical borders
            west, east = ((N * 4) - 1) - i, N + i
            south, north = ((N * 4) - 1) - i - N, i

            right, down = (1, 0), (0,1)
            up, left = (0, -1), (-1,0)

            self.mk_cell (clues[west], (0, i), right)
            self.mk_cell (clues[east], (N - 1, i), left)
            self.mk_cell (clues[north], (i,0), down)
            self.mk_cell (clues[south], (i, N - 1), up)

    def mk_cell(self, clue, p, dir) :
        x,y = p

        if clue == 1 :
            self.cell[y][x] = 0 | 1 << N
        elif clue == N :
            for i in range(1, N+1) :
                self.cell[y][x] = 0 | 1 << i
                x += dir[0]; y += dir[1]
        else :
            maxv = N - clue + 2

            for i in range(clue-1) :
                for j in range(maxv + i, N + 1) :
                    self.cell[y][x] &= ~(1 << j)
                self.cell[y][x] &= ~(1 << N)
                x += dir[0]; y += dir[1]

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

def valid(grid, clues) :
    for i in range(N) :
        west, east = ((N * 4) - 1) - i, N + i
        south, north = ((N * 4) - 1) - i - N, i
        column = [grid[j][i] for j in range(N)]

        if not equals(check_num(grid[i]), (clues[west], clues[east])) :
            return False

        if not equals(check_num(column), (clues[north], clues[south])) :
            return False

    return True

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
def reduce (curr) :
    for i in range(N) :
        col = filter([curr.cell[j][i] for j in range(N)])
        row = filter([curr.cell[i][j] for j in range(N)])

        for j in range(N) :
            curr.cell[j][i] = col[j]
            curr.cell[i][j] = row[j]


def backtrack(curr, row, col, index) :
    if index == N * N :
        return valid(curr.grid, curr.clue)

    x, y = index % N, index // N
    dig = 0
    cell, mask = curr.grid[y][x], 30 ^ (row[y] | col[x]) # mask == not yet chosen
    # cell, mask = curr.grid[y][x], curr.cell[y][x] # mask == all possible

    if cell != 0 : return backtrack(curr, row, col, index + 1)
    # print(showmask(mask),   showmask(curr.cell[y][x]))
    while mask :
        if (mask & 1) :
            # if not exist(row[y], dig) and not exist(col[x], dig) :
            row[y] ^= 1 << dig; col[x] ^= 1 << dig
            curr.grid[y][x] = dig

            if backtrack(curr, row, col, index + 1) :
                return True

            curr.grid[y][x] = 0
            row[y] ^= 1 << dig; col[x] ^= 1 << dig
        dig += 1
        mask >>= 1

    return False

def solve_puzzle (clues):
    # grid = [ [0 for x in range(N)] for y in range(N) ]
    # comb = list(itertools.permutations([i + 1 for i in range(N)]))
    curr = Skyscraper(clues)
    row, col = [0] * N, [0] * N

    for i in range(3) :
        reduce(curr)

    for y in range(N) :
        for x in range(N) :
            if hasonebit(curr.cell[y][x]) :
                bit = bit2int(curr.cell[y][x])
                curr.grid[y][x] = bit
                row[y] |= 1 << bit
                col[x] |= 1 << bit

    print(clues)
    display(curr.cell)
    print()
    backtrack(curr, row, col, 0)

    print()
    for line in curr.grid :
        print(line)

    return curr.grid

def showmask(cell) :
    for i in range(1,N+1) :
        if (cell >> i&1) == 1 :
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

clues = (
( 2, 2, 1, 3,  2, 2, 3, 1,  1, 2, 2, 3,  3, 2, 1, 3 ),
( 0, 0, 1, 2,  0, 2, 0, 0,  0, 3, 0, 0,  0, 1, 0, 0 ) )

outcomes = (
( ( 1, 3, 4, 2 ),
  ( 4, 2, 1, 3 ),
  ( 3, 4, 2, 1 ),
  ( 2, 1, 3, 4 ) ),

( ( 2, 1, 4, 3 ),
  ( 3, 4, 1, 2 ),
  ( 4, 2, 3, 1 ),
  ( 1, 3, 2, 4 ) )
)

actual = solve_puzzle(clues[1])
# print(actual)
