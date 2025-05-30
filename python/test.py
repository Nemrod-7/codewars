import heapq

class board :
    def __init__(self, grid) :
        self.width = len(grid[0])
        self.height = len(grid)
        self.grid = grid

    def is_inside(self, p) :
        if p[0] >= 0 and p[1] >= 0 and p[0] < self.width and p[1] < self.height :
            if self.grid[p[1]][p[0]] != '0' : return True
        return False

start = [0, [0,0], '']
n2 = [5, [0,0], '']
n3 = [2, [0,0], '']
n4 = [3, [0,0], '']

queue = [start]
heapq.heapify(queue);

heapq.heappush(queue, n2)
heapq.heappush(queue, n3)
heapq.heappush(queue, n4)


dist, curr, path = heapq.heappop(queue)

dir = (0,1)
curr = ((2,3),(0,0))
standing = True

def add (p1, p2, mul) :
    return (p1[0] * mul + p2[0], p1[1] * mul + p2[1])


# block = add(dir, curr[0], 2) if standing else add(dir, curr[1], 1)


next = dir + curr[0]


print(next)
