
direct = [(0,-1),(1,0),(0,1),(-1,0)] # north,east,south,west 
pipes = [['╋','┃','┫','┣','┏','┓','┳'], ['╋','━','┓','┛','┫','┳','┻'] , ['╋','┃','┫','┣','┗','┛','┻'] , ['╋','━','┏','┗','┣','┳','┻']]

def is_inside (nx, ny, grid) :
   return nx >= 0 and ny >= 0 and ny < len(grid) and nx < len(grid[0])    

def identify (cell) :
    match cell :
        case '━' : return [1,3]
        case '┓' : return [2,3]
        case '┃' : return [0,2]
        case '┛' : return [0,3]
        case '┏' : return [1,2]
        case '┗' : return [0,1]
        case '┳' : return [1,2,3]
        case '┫' : return [0,2,3]
        case '┣' : return [0,1,2]
        case '┻' : return [0,1,3]
        case '╋' : return [0,1,2,3]
    return []

def search (grid, visit) :
    ly = len(grid)-1

    for x in range(len(grid[0])) :
        if not visit[0][x] and grid[0][x] in pipes[2] : return (x,0)
    
    for y in range(len(grid)) :
        if not visit[y][0] and grid[y][0] in pipes[1] : return (0,y)

    for y in range(len(grid)) :
        x = len(grid[y])-1;
        if not visit[y][x] and grid[y][x] in pipes[3] : return (x,y)
    
    for x in range(len(grid[ly])) :
        if not visit[ly][x] and grid[ly][x] in pipes[0] : return (x,ly)

    return ()

def check_pipe (grid) :
    height, width = len(grid), len(grid[0])
    visit = [ [ False for x in range(width) ] for y in range(height) ]

    while True :
        start = search(grid, visit)
        if start == () : break
        heap = [start]
    
        while heap :
            x,y = heap.pop()
            cell = grid[y][x]

            visit[y][x] = True

            if cell == '.' : return False
        
            for i in identify(cell) :
                [dx,dy] = direct[i]
                nx, ny = x + dx, y + dy

                if is_inside(nx, ny, grid) :
                    if grid[ny][nx] not in pipes[i] : return False
                    if not visit[ny][nx] : heap.append((nx,ny))
                
    return True


table = ['╋━━┓', '┃..┃', '┛..┣']

res = check_pipe(table)
print(res)


