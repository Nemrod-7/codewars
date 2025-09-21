import time
start = time.time()

def display(grid) :
    for i in range(9) :
        if i % 3 == 0 : print()
        for j in range(9) :
            if j % 3 == 0 : print(' ', end='')
            print(grid[i][j], end=' ')
        print()
    print()
################################################################################

def cntsetbits(n) :
    count = 0;

    while (n) :
        count += 1
        n = (n - 1) & n;

    return count

def ispowerof2(num) :
    return num > 0 and (num & (num - 1)) == 0

def mk_hist(col, row, sub, tape, it) :

    for i in range(len(tape)) :
        x,y, _ = tape[i]
        z, cnt = y // 3 * 3 + x // 3, 0
        mask = 1022 ^ (row[y] | col[x] | sub[z])
        tape[i][2] = cntsetbits(mask)
    tape[it:] = sorted(tape[it:], key=lambda x:x[2])

def validate (grid) :
    cnt = 0
    col, row, sub = [0] * 10, [0] * 10, [0] * 10
    if len(grid) != 9 : raise RuntimeError('invalid size.')

    for y in range(9) :
        if len(grid[y]) != 9 : raise RuntimeError('invalid size.')
        for x in range(9) :
            z = y // 3 * 3 + x // 3
            mask = 1022 ^ (row[y] | col[x] | sub[z])

            if grid[y][x] < 0 or grid[y][x] > 9 : raise RuntimeError('invalid number.')

            if grid[y][x] :
                if (col[x] >> grid[y][x] &1) or (row[y] >> grid[y][x] &1) or (sub[z] >> grid[y][x] &1) :
                    raise RuntimeError('invalid grid.')
                col[x] |= 1 << grid[y][x]; row[y] |= 1 <<  grid[y][x]; sub[z] |= 1 << grid[y][x]
                cnt += 1

    if cnt < 17 : raise RuntimeError('unsolvable : multiple solutions.')
    return col, row, sub

def backtrack (grid, col, row, sub, tape, it, solution) :
    if it == len(tape) :
        if not 0 in grid and not grid in solution :
            solution.append([[grid[y][x] for x in range(9)] for y in range(9) ])
        return

    mk_hist(col, row, sub, tape, it)

    x, y, _ = tape[it]
    z = y // 3 * 3 + x // 3
    mask = 1022 ^ (row[y] | col[x] | sub[z])
    if mask == 0 : return False

    for dig in range(1,10) :
        if mask >> dig &1 :
            col[x] |= 1 << dig; row[y] |= 1 << dig; sub[z] |= 1 << dig
            grid[y][x] = dig

            backtrack(grid, col, row, sub, tape, it + 1, solution)

            grid[y][x] = 0
            col[x] &= ~(1 << dig); row[y] &= ~(1 << dig); sub[z] &= ~(1 << dig)

def sudoku_solver (grid) :
    power = {2:1, 4:2, 8:3, 16:4, 32:5, 64:6, 128:7, 256:8, 512:9}
    solution = []
    col, row, sub = validate(grid)
    # display(grid)
    for _ in range(2) :
        for y in range(9) :
            for x in range(9) :
                z = y // 3 * 3 + x // 3
                mask = 1022 ^ (row[y] | col[x] | sub[z])

                if grid[y][x] == 0 and ispowerof2(mask) :
                    grid[y][x] = power[mask]
                    col[x] |= 1 << grid[y][x]; row[y] |= 1 <<  grid[y][x]; sub[z] |= 1 << grid[y][x]

                if grid[y][x] == 0 and mask == 0 :
                    raise RuntimeError('unsolvable grid.')

    tape = [[x,y, 0] for y in range(9) for x in range(9) if grid[y][x] == 0 ]
    backtrack(grid, col, row, sub, tape, 0, solution)

    if len(solution) != 1 : raise RuntimeError('unsolvable grid.')
    return solution[0]


puzzle = [
    [0, 0, 6, 1, 0, 0, 0, 0, 8],
    [0, 8, 0, 0, 9, 0, 0, 3, 0],
    [2, 0, 0, 0, 0, 5, 4, 0, 0],
    [4, 0, 0, 0, 0, 1, 8, 0, 0],
    [0, 3, 0, 0, 7, 0, 0, 4, 0],
    [0, 0, 7, 9, 0, 0, 0, 0, 3],
    [0, 0, 8, 4, 0, 0, 0, 0, 6],
    [0, 2, 0, 0, 5, 0, 0, 8, 0],
    [1, 0, 0, 0, 0, 2, 5, 0, 0]
]

# board = [puzzle[y][x] for y in range(9) for x in range(9) ]
grid = sudoku_solver(puzzle)

end = time.time()
print( "elapsed : ", end - start)
