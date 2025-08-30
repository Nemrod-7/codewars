top = ((1, 1), (4,), (1, 1, 1), (3,), (1,))
left = ((1,), (2,), (3,), (2, 1), (4,))

width, height = len(top), len(left)

def place (blocks, total) : # ie : https://stackoverflow.com/questions/47159012/recursive-algorithm-to-find-all-possible-solutions-in-a-nonogram-row? 
    if not blocks: return [0 * total]
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
    x,y = 2,0

    for i in range(width) :
        if grid[y][i] != ' ' :
            # next = []
            #
            # for comb in north[i] :
            #     print(comb, grid[y][i], comb[y])
            # print()
            # print(north[i])
            north[i] = [ comb for comb in north[i] if comb[y] != grid[y][x] ]
        
        if len(north[i]) == 1 :
            for j in range(height) :
                grid[j][i] = north[i][0][j]



    for i in range(height) :
        if grid[i][x] != ' ' :
            east[i] = [ comb for comb in east[i] if comb[x] == grid[i][x] ]

        if len(east[i]) == 1 :
            for j in range(width) :
                grid[i][j] = east[i][0][j]
        
    for y in range(height) :
        level = east[y]
        print(grid[y])





grid = [ [' '  for _ in range(width) ] for _ in range(height) ]

north = [ place(cell, width) for cell in top ]
east = [ place(cell, height) for cell in left ]


reduce(grid, north, east)
reduce(grid, north, east)
