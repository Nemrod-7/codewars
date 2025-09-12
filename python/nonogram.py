import time
start = time.time()


def checkcell(comb, x, y) :
    hist = [0,0]

    for curr in comb[x] :
        hist[int(curr[y])] += 1

    if hist[0] == 0 : return 1
    if hist[1] == 0 : return 0
    return ' '

def place (blocks, total) : # source : https://stackoverflow.com/questions/47159012/recursive-algorithm-to-find-all-possible-solutions-in-a-nonogram-row
    if not blocks : return ['0' * total]
    if blocks[0] > total : return []

    starts = total - blocks[0] # starts = 2 means possible starting indexes are [0,1,2]

    if len(blocks) == 1 : # this is special case
        return [ ('0' * i + '1' * blocks[0] + '0' * (starts-i)) for i in range(starts + 1) ]

    ans = []
    for i in range(total - blocks[0]) : # append current solutions
        for sol in place(blocks[1:], starts-i-1): # with all possible other solution
            ans.append('0' * i + '1' * blocks[0] + '0' + sol)

    return ans

def solve(clues, width, height) :
    top, left = clues
    north = [ place(cell, height) for cell in top ]
    east = [ place(cell, width) for cell in left ]
    grid = [ [' ' for _ in range(width)] for _ in range(height) ]
    # print('(',top,',', left,')')

    for _ in range(20) : 
        cnt = 0
        x_axis = sorted([ (len(north[i]), i) for i in range(width) ])
        y_axis = sorted([ (len(east[i]), i) for i in range(height) ])

        # print(x_axis)
        # print(y_axis)
        for _,y in y_axis :
            for _,x in x_axis :
                if grid[y][x] != ' ' :
                    east[y] = [ comb for comb in east[y] if int(comb[x]) == grid[y][x] ]
                    north[x] = [ comb for comb in north[x] if int(comb[y]) == grid[y][x] ]
                else :
                    a = checkcell(north, x, y)
                    b = checkcell(east , y, x)

                    if a != ' ' : grid[y][x] = a
                    if b != ' ' : grid[y][x] = b
                    cnt += 1
        if cnt == 0 : break

    return tuple(tuple(line) for line in grid )

def display(grid) :
    for line in grid :
        for cell in line :
            if cell == 1 : print('1 ', end='')
            if cell == 0 : print('  ', end='') 
            if cell == ' ' : print('. ', end='')
        print()

def gettest(clues) :
    grid = solve(clues, len(clues[0]), len(clues[1]))
    # display(grid)

clues = ( ((2, 2, 1, 3), (1, 2, 2, 1, 1, 1, 3), (3, 1, 5, 1), (5, 7, 2), (2, 4, 13), (3, 5, 6, 1, 5), (4, 1, 3, 3, 5), (1, 9, 3, 3), (1, 1, 1, 2, 1, 3, 3, 1), (1, 2, 1, 1, 5), (1, 1, 3, 1, 4, 1), (1, 1, 2, 1, 1, 1, 3, 1), (4, 4, 7), (4, 4, 3, 3), (6, 1, 1, 3, 4, 2), (8, 1, 1, 2, 2, 1), (4, 3, 1, 3, 2), (3, 7, 1, 1, 1), (2, 9, 1), (2, 10, 2, 1), (5, 13), (1, 12), (4, 1, 5, 2, 1, 2, 1, 2), (6, 3, 3, 4, 2), (1, 11, 4, 2)) , ((2, 2, 2, 6, 1), (2, 7, 3), (1, 1, 2, 4, 1, 2), (3, 2, 5, 1, 2), (7, 6, 3, 2), (5, 1, 8, 4), (2, 5, 8, 2), (1, 2, 1, 3, 1, 5, 1, 1), (1, 7, 3, 3), (1, 1, 1, 1, 4, 3), (3, 4, 1, 4, 3), (2, 2, 1, 1, 6, 1), (2, 3, 4, 1, 4, 1), (3, 3, 6), (4, 1, 3, 5), (7, 1, 3, 2, 1), (3, 4, 2, 3), (5, 2, 2, 3, 2), (3, 3, 6, 6), (6, 6, 5), (11, 2, 2), (9, 1, 3), (2, 6, 2, 1, 2), (2, 4, 3, 1, 3), (5, 1, 1, 6, 3, 3)) )
gettest(clues)

clues = ( ((4, 2, 5, 6), (2, 4, 2, 2, 2), (7, 11), (4, 2, 1, 4, 2), (6, 8, 1), (7, 2, 3), (3, 6, 1, 2), (1, 1, 2, 1, 4), (2, 4, 6), (2, 3, 1, 2, 4, 2), (3, 9, 1, 3, 1), (1, 1, 7, 6), (6, 3, 6), (2, 7, 2, 1, 5), (1, 3, 3, 1, 1, 6), (1, 3, 15), (3, 2, 7, 3), (7, 1, 3), (4, 3, 3, 4, 1), (2, 1, 2, 4, 3, 1), (3, 3, 3, 4), (3, 1, 8, 1, 2), (3, 2, 3, 10, 2), (4, 1, 3, 3, 1), (2, 1, 3, 1, 3)) , ((4, 2, 3, 3, 5), (7, 4, 1, 6), (1, 6, 1, 1, 5), (1, 4, 1, 2, 1), (1, 2, 1, 2, 1, 2), (5, 1, 4, 3, 1), (5, 3, 4, 3), (2, 3, 3, 3), (2, 8, 2), (3, 1, 3, 5), (3, 13, 1), (1, 1, 3, 9), (1, 5, 2, 1, 1, 1, 2, 1), (3, 3, 9, 3), (6, 5, 3, 3), (1, 3, 3, 5, 3), (5, 1, 4, 1, 1), (4, 1, 1, 5, 1, 2), (1, 2, 2, 4, 4), (1, 1, 1, 9, 3, 3), (1, 1, 8, 5), (5, 9, 4), (4, 3, 8, 1), (1, 6, 1, 3, 1, 1, 1), (1, 1, 1, 4, 3)) )
gettest(clues)

clues = ( ((1, 1, 1), (1, 3, 1, 3), (2, 2, 3, 1), (4, 3, 1, 2, 3), (3, 6, 2), (6, 2, 5, 2, 1), (4, 1, 7, 4, 1), (4, 1, 5, 6), (3, 2, 2, 4, 6, 1), (3, 5, 1, 1, 2), (7, 3, 4, 3, 1), (7, 2, 3, 2, 4, 1), (5, 1, 5, 10), (5, 2, 6, 1, 1), (1, 2, 4, 3, 4, 1), (2, 4, 1, 10), (8, 3, 8, 1), (3, 2, 4, 9), (3, 2, 4, 4, 1), (2, 9, 4, 1), (2, 2, 1, 6, 6), (4, 5, 1, 4), (2, 3, 2, 3), (1, 4, 8, 1), (2, 4, 1, 3, 1, 1)) , ((1, 7, 4), (8, 3), (2, 1, 9, 5, 2), (7, 5, 1, 3, 1), (5, 11), (4, 4, 1, 3), (1, 1, 8, 1, 2), (1, 2, 3, 6), (7, 2, 8), (3, 10, 1, 2), (1, 3, 1, 1, 2, 2), (1, 6, 2, 4, 1, 2), (7, 2, 2, 5), (4, 1, 6, 7), (3, 1, 2, 1, 10), (5, 1, 2, 7, 2), (3, 9, 2, 1), (1, 5, 7, 1, 3), (6, 7, 3), (2, 2, 3, 7, 1), (5, 1, 3, 6), (1, 3, 4, 6), (2, 1, 2, 1, 4), (3, 1, 1, 3), (1, 1, 1, 4, 6)) )

gettest(clues)



end = time.time()
print( "elapsed : ", end - start)


