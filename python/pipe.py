from cwar import grid

import time
start = time.time()

import heapq

direct = [(0,-1, 'up'),(1,0,'right'),(0,1,'down'),(-1,0,'left')]

def is_inside (nx, ny, grid) :
   return nx >= 0 and ny >= 0 and ny < len(grid) and nx < len(grid[0])    

def cheapest_path(grid, start, exit) :
    height, width = len(grid), len(grid[0])
    visit = [ [ False for x in range(width) ] for y in range(height) ]

    start, exit = (start[1], start[0]), (exit[1], exit[0])

    heap = [ [0, start, []] ]
    heapq.heapify(heap)

    while heap :
        func, (x,y), path = heapq.heappop(heap)

        if (x,y) == exit : 
            return path
    
        for dx,dy, dir in direct :
            nx, ny = x + dx, y + dy

            if is_inside(nx, ny, grid) and not visit[ny][nx] :
                visit[ny][nx] = True
                weight = func + grid[ny][nx]

                heapq.heappush(heap, [ weight, (nx,ny), path + [dir] ] )
                
    return []

def display(queue) :
    heap = queue.copy()

    while heap :
        curr = heapq.heappop(heap)
        print(curr[0], curr[1])



strt = [0,0]
exit = [len(grid) - 1, len(grid[0]) - 1]
# res = cheapest_path(grid, strt, exit)

res = cheapest_path([[1,9,1],[2,9,1],[2,1,1]], (0,0), (0,2))
print(res)

end = time.time()
print( "elapsed : ", end - start)
