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

def get (mask) :
    match (mask) :
      case 2 : return 1 
      case 4 : return 2
      case 8 : return 3
      case 16 : return 4
      case 32 : return 5
      case 64 : return 6
      case 128 : return 7
      case 256 : return 8
      case 512 : return 9

def exist(mask, bit) :
    return mask &1 << bit

def NumberOfSetBits(n) :
    count = 0;

    while (n) :
        count += 1
        n = (n - 1) & n;

    return count

def mk_hist(col, row, sub, tape, it) :

    for i in range(len(tape)) :
        x,y, _ = tape[i]
        z, cnt = y // 3 * 3 + x // 3, 0
        mask = 1022 ^ (row[y] | col[x] | sub[z])
        tape[i][2] = NumberOfSetBits(mask)
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

def ispowerof2(num) :
    return num > 0 and (num & (num - 1)) == 0

def sudoku_solver (board) :
    cnt = 0
    col, row, sub = [0] * 10, [0] * 10, [0] * 10
    if len(board) != 9 : raise RuntimeError('invalid size.')
    # print(board)

    for y in range(9) :
        if len(board[y]) != 9 : raise RuntimeError('invalid size.')
        for x in range(9) :
            z = y // 3 * 3 + x // 3
            mask = 1022 ^ (row[y] | col[x] | sub[z])

            if board[y][x] < 0 or board[y][x] > 9 : raise RuntimeError('invalid number.')

            if board[y][x] :
                if (col[x] >> board[y][x] &1) or (row[y] >> board[y][x] &1) or (sub[z] >> board[y][x] &1) : 
                    raise RuntimeError('invalid grid.')
                col[x] |= 1 << board[y][x]; row[y] |= 1 <<  board[y][x]; sub[z] |= 1 << board[y][x]
                cnt += 1

    if cnt < 17 : raise RuntimeError('unsolvable : multiple solutions.')

    for _ in range(2) :
        for y in range(9) :
            for x in range(9) :
                z = y // 3 * 3 + x // 3
                mask = 1022 ^ (row[y] | col[x] | sub[z])

                if not board[y][x] and ispowerof2(mask) :
                    board[y][x] = get(mask)
                    col[x] |= 1 << board[y][x]; row[y] |= 1 <<  board[y][x]; sub[z] |= 1 << board[y][x]

                if not board[y][x] and mask == 0 : 
                    raise RuntimeError('unsolvable grid.')


    display(board)
    tape = [[x,y, 0] for y in range(9) for x in range(9) if board[y][x] == 0 ]

    if backtrack(board, col, row, sub, tape, 0) == False :
        raise RuntimeError('unsolvable grid.')

    col, row, sub = [0] * 10, [0] * 10, [0] * 10
    for y in range(9) :
        for x in range(9) :
            z = y // 3 * 3 + x // 3

    if cnt < 17 : raise RuntimeError('unsolvable : multiple solutions.')

    for _ in range(2) :
        for y in range(9) :
            for x in range(9) :
                z = y // 3 * 3 + x // 3
                mask = 1022 ^ (row[y] | col[x] | sub[z])

                if not board[y][x] and ispowerof2(mask) :
                    board[y][x] = get(mask)
                    col[x] |= 1 << board[y][x]; row[y] |= 1 <<  board[y][x]; sub[z] |= 1 << board[y][x]

                if not board[y][x] and mask == 0 : 
                    raise RuntimeError('unsolvable grid.')


    tape = [[x,y, 0] for y in range(9) for x in range(9) if board[y][x] == 0 ]

    if backtrack(board, col, row, sub, tape, 0) == False :
        raise RuntimeError('unsolvable grid.')

    # col, row, sub = [0] * 10, [0] * 10, [0] * 10
    # for y in range(9) :
    #     for x in range(9) :
    #         z = y // 3 * 3 + x // 3
    #         col[x] |= 1 << board[y][x]; row[y] |= 1 <<  board[y][x]; sub[z] |= 1 << board[y][x]
    #
    # for y in range(9) :
    #     for x in range(9) :
    #         z = y // 3 * 3 + x // 3
    #         print(col[x], sub[z], row[y], end=' ')
    #
    # display(board)
    return board

puzzle = [
[0, 2, 3,  4, 5, 6,  7, 8, 9],
[1, 5, 6,  7, 8, 9,  0, 2, 3],
[7, 8, 9,  1, 2, 3,  4, 5, 6],

[2, 3, 4,  5, 6, 7,  8, 9, 1],
[5, 6, 7,  8, 9, 1,  2, 3, 4],
[8, 9, 1,  2, 3, 4,  5, 6, 7],

[3, 4, 5,  6, 7, 8,  9, 1, 2],
[6, 7, 8,  9, 1, 2,  3, 4, 5],
[9, 1, 2,  3, 4, 5,  6, 7, 8]]

puzzle = [
[0, 9, 6, 5, 0, 4, 0, 7, 1], 
[0, 2, 0, 1, 0, 0, 0, 0, 0], 
[0, 1, 4, 0, 9, 0, 6, 2, 3], 
[0, 0, 3, 0, 6, 0, 0, 8, 0], 
[0, 0, 8, 0, 5, 0, 4, 0, 0], 
[9, 0, 0, 4, 0, 0, 0, 0, 5], 
[7, 0, 0, 0, 0, 9, 0, 0, 0], 
[0, 0, 1, 0, 7, 5, 3, 4, 9], 
[2, 3, 0, 0, 4, 8, 1, 0, 7]]

puzzle = [
 [0, 9, 6, 5, 0, 4, 0, 7, 1], 
 [0, 2, 0, 1, 0, 0, 0, 0, 0],
 [0, 1, 4, 0, 9, 0, 6, 2, 3], 
 [0, 0, 3, 0, 6, 0, 0, 8, 0],
 [0, 0, 8, 0, 5, 0, 4, 0, 0],
 [9, 0, 0, 4, 1, 0, 0, 0, 5],
 [7, 0, 0, 0, 0, 9, 0, 0, 0],
 [0, 0, 1, 0, 7, 5, 3, 4, 9],
 [2, 3, 0, 0, 4, 8, 1, 0, 7]]

puzzle = [
[5, 4, 0, 0, 0, 0, 0, 0, 3],
[0, 0, 7, 4, 0, 6, 8, 0, 0],
[0, 9, 0, 0, 8, 0, 0, 7, 0],
[0, 2, 0, 8, 0, 1, 0, 5, 0],
[0, 0, 4, 0, 0, 0, 9, 0, 0],
[0, 5, 0, 9, 0, 3, 0, 1, 0],
[0, 7, 0, 0, 1, 0, 0, 3, 0],
[0, 0, 3, 7, 0, 8, 2, 0, 0],
[9, 0, 0, 0, 0, 0, 0, 0, 7]]

puzzle = [
[0, 7, 0, 0, 2, 3, 0, 1, 0],
[0, 0, 0, 4, 0, 0, 2, 0, 0],
[9, 0, 6, 0, 7, 0, 4, 0, 3],
[0, 0, 3, 0, 0, 0, 0, 0, 5],
[0, 4, 0, 2, 0, 8, 0, 6, 0],
[5, 0, 0, 0, 0, 0, 1, 0, 0],
[4, 0, 5, 0, 1, 0, 7, 0, 8],
[0, 0, 7, 0, 0, 5, 0, 0, 0],
[0, 3, 0, 7, 0, 0, 0, 5, 0]]
grid = sudoku_solver(puzzle)
# display(grid)




# puzzle = [[5,3,0,0,7,0,0,0,0],
#           [6,0,0,1,9,5,0,0,0],
#           [0,9,8,0,0,0,0,6,0],
#           [8,0,0,0,6,0,0,0,3],
#           [4,0,0,8,0,3,0,0,1],
#           [7,0,0,0,2,0,0,0,6],
#           [0,6,0,0,0,0,2,8,0],
#           [0,0,0,4,1,9,0,0,5],
#           [0,0,0,0,8,0,0,7,9]]
# grid = sudoku_solver(puzzle)
# display(grid)
#
# puzzle = [
#     [0,0,6,1,0,0,0,0,8],
#     [0,8,0,0,9,0,0,3,0],
#     [2,0,0,0,0,5,4,0,0],
#     [4,0,0,0,0,1,8,0,0],
#     [0,3,0,0,7,0,0,4,0],
#     [0,0,7,9,0,0,0,0,3],
#     [0,0,8,4,0,0,0,0,6],
#     [0,2,0,0,5,0,0,8,0],
#     [1,0,0,0,0,2,5,0,0]]
# grid = sudoku_solver(puzzle)
# display(grid)
#
# puzz1 = [
#     [4,0,5,0,1,0,7,0,8],
#     [0,0,7,0,0,5,0,0,0],
#     [0,3,0,7,0,0,0,5,0],
#     [0,0,3,0,0,0,0,0,5],
#     [0,4,0,2,0,8,0,6,0],
#     [5,0,0,0,0,0,1,0,0],
#     [0,7,0,0,2,3,0,1,0],
#     [0,0,0,4,0,0,2,0,0],
#     [9,0,6,0,7,0,4,0,3]]
# grid = sudoku_solver(puzzle)
# display(grid)

end = time.time()
print( "elapsed : ", end - start)
