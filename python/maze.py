
COMPASS = [ [[1,0],'E'], [[0,1],'S'], [[-1,0],'W'], [[0,-1],'N'] ]

def is_inside(x,y, grid) : return x >= 0 and y >= 0 and x < len(grid[0]) and y < len(grid)

def rotate (grid) :
    height, width = len(grid), len(grid[0])

    for y in range(height) :
        for x in range(width) :
            cell = grid[y][x]

            if cell > 0 :
                if cell < 8 :
                    grid[y][x] *= 2
                else :
                    grid[y][x] = cell - (15 - cell)

def dfs (grid, graph, cycle) :
    alt, way = 0, 1
    height, width = len(grid), len(grid[0])
    queue = [[x,y] for y in range(height) for x in range(width) if graph[y][x][0] <= cycle]

    for y in range(height) :
        for x in range(width) :
            if graph[y][x][0] <= cycle :
                graph[y][x][1].append('')

    while queue :
        x, y = queue.pop()
        curr, route = grid[y][x], graph[y][x][1]

        if curr == -2 : return True
        
        for i in range(4) :
            [dx,dy], letter = COMPASS[i]
            nx, ny = x + dx, y + dy

            if is_inside (nx, ny, grid) :
                nxc = grid[ny][nx]

                nxt = 0 if nxc < 0 else nxc >> ((i + 2) % 4)&1
                out = 0 if curr < 0 else curr >> i&1

                if out == 0 and nxt == 0 and graph[ny][nx][0] == 99 :
                    graph[ny][nx][alt] = cycle + 1
                    graph[ny][nx][way] = list(route)
                    graph[ny][nx][way][-1] += letter

                    queue.append([nx,ny])
    return False


def maze_solver (grid) :
    height, width = len(grid), len(grid[0])
    start = [(x,y) for y in range(height) for x in range(width) if grid[y][x] == 'B'][0]
    exit = [(x,y) for y in range(height) for x in range(width) if grid[y][x] == 'X'][0]

    graph = [[[99, []] for x in range(width)] for y in range(height)]
    grid = [[grid[y][x] for x in range(width)] for y in range(height) ]

    for y in range(height) :
        for x in range(width) :
            match grid[y][x] :
                case 'X' : grid[y][x] = -2
                case 'B' : grid[y][x] = 0; graph[y][x][0] = 0

    # display(grid)
    for cycle in range(23) :
        if dfs(grid, graph, cycle) == True :
            return graph[exit[1]][exit[0]][1]
        rotate(grid)

    return None

def display(grid) :
    height, width = len(grid), len(grid[0])

    for y in range(height) :
        for x in range(width) :
            print(grid[y][x], end=' ')
        print()
    print()


example = (
    (  4,  2,  5,  4),
    (  4, 15, 11,  1),
    ('B',  9,  6,  8),
    ( 12,  7,  7,'X')
)

example = (
    (6,3,10,4,11),
    (8,10,4,8,5),
    ('B',14,11,3,'X'),
    (15,3,4,14,15),
    (14,7,15,5,5)
)
path = maze_solver(example) # ['NNE','EE','S','SS']
print(path)

# example[0][0] = 0
