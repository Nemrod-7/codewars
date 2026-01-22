
def display(grid) :
    for line in grid :
        for cell in line :
            if cell == 1 : print('1 ', end='')
            if cell == 0 : print('  ', end='') 
            if cell == ' ' : print('. ', end='')
        print()

def checkcell(comb, x, y) :
    hist = [0,0]
    for curr in comb[x] :
        hist[int(curr[y])] += 1

    if hist[0] == 0 : return 1
    if hist[1] == 0 : return 0
    return ' '

def place (blocks, total) : # source : https://stackoverflow.com/questions/47159012/recursive-algorithm-to-find-all-possible-solutions-in-a-nonogram-row
    if not blocks: return ['0' * total]
    if blocks[0] > total: return []

    starts = total - blocks[0] # starts = 2 means possible starting indexes are [0,1,2]

    if len(blocks) == 1: # this is special case
        return [ ('0' * i + '1' * blocks[0] + '0' * (starts-i)) for i in range(starts + 1) ]

    ans = []
    for i in range(total - blocks[0]): # append current solutions
        for sol in place(blocks[1:], starts-i-1): # with all possible other solution
            ans.append('0' * i + '1' * blocks[0] + '0' + sol)

    return ans

def reduce (grid, north, east) :
    cnt = 0
    for y in range(len(east)) :
        for x in range(len(north)) :
            if grid[y][x] != ' ' :
                east[y] = [ comb for comb in east[y] if int(comb[x]) == grid[y][x] ]
                north[x] = [ comb for comb in north[x] if int(comb[y]) == grid[y][x] ]

            a, b = checkcell(north, x, y), checkcell(east, y, x)
            if a != ' ' : grid[y][x] = a
            if b != ' ' : grid[y][x] = b
            if grid[y][x] == ' ' : cnt += 1
    return cnt

def solve(clues) :
    top, left = clues
    north = [ place(cell, len(top)) for cell in top ]
    east = [ place(cell, len(left)) for cell in left ]
    grid = [[' ' for _ in range(len(top))] for _ in range(len(left))]

    for _ in range(70) :
        if not reduce(grid, north, east) : break
    # reduce(grid, north, east)

    return tuple(tuple(line) for line in grid )


clues = (
    ((4, 4), (3, 1, 1), (2, 3, 1), (1, 6, 2), (2, 4, 1), (3, 4, 1), (3, 3, 2), (6, 4), (4, 3), (2, 3, 1), (2, 3, 1, 1), (1, 2, 1, 1, 1), (2, 1, 1, 1, 1), (3, 2, 1, 1, 1), (4, 1, 1)),
    ((4, 3), (3, 1, 2), (2, 3, 1), (1, 6, 2), (2, 4, 1), (3, 4, 1), (3, 3, 2), (6, 4), (4, 3), (2, 3, 1), (2, 3, 1, 1), (1, 2, 1, 1, 1), (1, 1, 1, 1, 1), (4, 2, 1, 1, 1), (4, 1, 1))
)

clues = (
((), (4,), (1, 1), (1, 3, 1), (1, 5, 2), (3, 1, 1), (5, 2, 1), (6, 2, 1, 1), (5, 2, 1), (3, 1, 1), (1, 5, 2), (1, 3, 1), (1, 1), (4,), ()) ,
((), (3,), (5,), (5,), (9,), (1, 3, 1), (1, 1, 1, 1, 1), (1, 2, 2, 1), (1, 2, 2, 1), (1, 5, 1, 1), (3, 7), (1, 1), (5,), (1, 1), (1, 1, 1))
)

clues = (
((7,), (1, 1, 1), (1, 1, 1), (1, 1, 1), (2, 1, 1, 2), (1, 1, 2, 5), (2, 2, 2), (1, 1, 1, 2), (1, 2, 4, 1), (2, 1, 2), (1, 2, 1, 1), (1, 5, 1), (2, 2, 2), (3, 4), (1, 6)) ,
((2, 2), (1, 1, 1, 1), (1, 3, 1, 1), (1, 1, 1), (1, 1, 1, 1), (1, 1, 1, 2), (1, 1, 1, 1), (2, 2, 1), (3, 5, 1, 1), (1, 2, 1, 3), (1, 1, 1, 2), (1, 3, 1, 1, 1), (3, 1, 1, 1, 2), (1, 2, 6), (6,))
 )

grid = solve(clues)
display(grid)
