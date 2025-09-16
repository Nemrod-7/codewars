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

def exist(mask, bit) :
    return mask &1 << bit

def mk_hist(col, row, sub, tape, it) :

    for i in range(len(tape)) :
        x,y, _ = tape[i]
        z, cnt = y // 3 * 3 + x // 3, 0

        for j in range(1,10) :
            ex, ey, ez = col[x] &1 << j, row[y] &1 << j, sub[z] &1 << j

            if not ex and not ey and not ez :
                cnt += 1

        tape[i][2] = cnt
    tape[it:] = sorted(tape[it:], key=lambda x:x[2])

def backtrack (board, col, row, sub, tape, it) :
    if it == len(tape) : return True

    mk_hist(col, row, sub, tape, it)

    x, y, _ = tape[it]
    z = y // 3 * 3 + x // 3

    for dig in range(1,10) :
        if not exist(col[x], dig) and not exist(row[y], dig) and not exist(sub[z], dig) :
            col[x] ^= 1 << dig; row[y] ^= 1 << dig; sub[z] ^= 1 << dig
            board[y][x] = dig

            if backtrack(board, col, row, sub, tape, it + 1) :
                return True

            board[y][x] = 0
            col[x] ^= 1 << dig; row[y] ^= 1 << dig; sub[z] ^= 1 << dig

    return False

def solve (board) :
    col, row, sub = [0] * 10, [0] * 10, [0] * 10
    # grid = [ dig for line in board for dig in line ]
    cnt = 0
    # if len(board) != 9 : return 'error'

    for y in range(9) :
        # if len(board[y]) != 9 : 'error'

        for x in range(9) :
            dig, z = board[y][x], y // 3 * 3 + x // 3
            # if dig < 0 or dig > 9 : return 'error'

            if dig != 0 :
                col[x] |= 1 << dig; row[y] |= 1 << dig; sub[z] |= 1 << dig
                cnt += 1

    if cnt < 17 : return 'error'

    tape = [[x,y, 0] for y in range(9) for x in range(9) if board[y][x] == 0 ]
    backtrack(board, col, row, sub, tape, 0)

    return board

puzzle = [[5,3,0,0,7,0,0,0,0],
          [6,0,0,1,9,5,0,0,0],
          [0,9,8,0,0,0,0,6,0],
          [8,0,0,0,6,0,0,0,3],
          [4,0,0,8,0,3,0,0,1],
          [7,0,0,0,2,0,0,0,6],
          [0,6,0,0,0,0,2,8,0],
          [0,0,0,4,1,9,0,0,5],
          [0,0,0,0,8,0,0,7,9]]
grid = solve(puzzle)


puzzle = [
    [0,0,6,1,0,0,0,0,8],
    [0,8,0,0,9,0,0,3,0],
    [2,0,0,0,0,5,4,0,0],
    [4,0,0,0,0,1,8,0,0],
    [0,3,0,0,7,0,0,4,0],
    [0,0,7,9,0,0,0,0,3],
    [0,0,8,4,0,0,0,0,6],
    [0,2,0,0,5,0,0,8,0],
    [1,0,0,0,0,2,5,0,0]]
grid = solve(puzzle)
display(grid)

puzz1 = [
    [4,0,5,0,1,0,7,0,8],
    [0,0,7,0,0,5,0,0,0],
    [0,3,0,7,0,0,0,5,0],
    [0,0,3,0,0,0,0,0,5],
    [0,4,0,2,0,8,0,6,0],
    [5,0,0,0,0,0,1,0,0],
    [0,7,0,0,2,3,0,1,0],
    [0,0,0,4,0,0,2,0,0],
    [9,0,6,0,7,0,4,0,3]]
grid = solve(puzzle)
display(grid)

end = time.time()
print( "elapsed : ", end - start)
