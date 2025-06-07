import re

class board :
    def __init__(self, grid) :
        self.width = len(grid[0])
        self.height = len(grid)
        self.grid = grid

    def is_inside(self, p) : return p[0] >= 0 and p[1] >= 0 and p[0] < self.width and p[1] < self.height
    def is_free(self, p) : return self.is_inside(p) and self.grid[p[1]][p[0]] != '0'
    def at(self, p) : return '.' if self.is_free(p) else ' '


height, width = 4, 5
graph = [[[99, ['']] for x in range(width)] for y in range(height)]


x, y = 0,0

path = graph[y][x][1][-1]
graph[y][x][1][-1] = path + 'L'


path = graph[y][x][1][-1]
graph[y][x][1][-1] = path + 'L'

path = graph[y][x][1][-1]
graph[y][x][1][-1] = path + 'D'

print(graph[y][x][1])
