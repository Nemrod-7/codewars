import heapq

def is_inside(x,y, grid) : return x >= 0 and y >= 0 and x < len(grid[0]) and y < len(grid)
def is_border(x,y, grid) : return x == 0 or y == 0 or x == ( len(grid[0]) - 1 ) or y == ( len(grid) - 1)

def volume (grid) :
    compass = [(0,-1), (1,0), (0,1),(-1,0)]

    heap = []
    row, col = len(grid), len(grid[0])
    top = max([ max(it) for it in grid ])
    water = [[top for x in range(col)] for y in range(row)]
    visit = [[False for x in range(col)] for y in range(row)]

    heapq.heapify(heap);

    for y in range(row) : 
        for x in range(col) :
            if is_border(x,y, grid) and grid[y][x] < top :
                visit[y][x] = True
                water[y][x] = grid[y][x]
                heapq.heappush(heap, (grid[y][x],x,y))

    while heap :
        dist, x, y = heapq.heappop(heap)
        visit[y][x] = True
        water[y][x] = dist

        for [dx,dy] in compass :
            nx, ny = x + dx, y + dy

            if is_inside(nx,ny,grid)  :
                alt = max (grid[ny][nx], min (water[ny][nx], dist))

                if alt < water[ny][nx] and not visit[ny][nx]: 
                    heapq.heappush(heap, (alt,nx,ny))

 
    return sum([ water[y][x] - grid[y][x] for y in range(row) for x in range(col)])


grid =[
[ 0, 10,  0, 20,  0],
[20,  0, 30,  0, 40],
[ 0, 40,  0, 50,  0],
[50,  0, 60,  0, 70],
[ 0, 60,  0, 70,  0]]


total = volume(grid)
print(total)
