

grid = ["     ", "  X  ", "  |  ", "  |  ", "  X  "]
grid = ["                    ", "     +--------+     ", "  X--+        +--+  ", "                 |  ", "                 X  ", "                    "]
grid = ["                     ", "    +-------------+  ", "    |             |  ", " X--+      X------+  ", "                     "]
grid = ["X-----|----X"]
grid = ["   |--------+    ", "X---        ---+ ", "               | ", "               X "]
grid = ["              ", "   +------    ", "   |          ", "X--+      X   ", "              "]
grid = ["      +------+", "      |      |", "X-----+------+", "      |       ", "      X       "]


def direction(dir) :
    match dir :
        case 'multi' : return [(1,0),(0,1),(-1,0),(0,-1)]
        case 'horiz' : return [(1,0),(-1,0)]
        case 'verti' : return [(0,1),(0,-1)]
def getdir(cell) :
    if cell == '-' : return 'horiz'
    elif cell == '|' : return 'verti'
    elif cell == '+' : return 'verti' if dir == 'horiz' else 'horiz'
    else : return 'multi'

def line(grid) :
    height, width = len(grid), len(grid[0])
    grid = [ [grid[y][x] for x in range(width)] for y in range(height) ]

    visit = [ [0 for x in range(width)] for y in range(height) ]
    XS = [(x,y) for y in range(height) for x in range(width) if grid[y][x] == 'X']
    x,y = XS[0]
    start = [x, y, 'multi']
    heap = [start]
    grid[y][x] = 'x'

    while heap :
        x,y, dir = heap.pop()
        # print(x,y, grid[y][x])

        visit[y][x] = True

        if grid[y][x] == 'X' :
            print('exit')
            break

        for dx,dy in direction(dir) :
            nx, ny = x + dx, y + dy

            if nx >= 0 and ny >= 0 and nx < width and ny < height :
                if not visit[ny][nx] :
                    ndir = getdir(grid[ny][nx])

                    if dir == 'verti' and grid[ny][nx] == '-' : continue
                    if dir == 'horiz' and grid[ny][nx] == '|' : continue

                    if grid[ny][nx] != ' ' :
                        heap.append([nx,ny, ndir])

    return False

actual = line(grid)
print(actual)
