N = 5

def display(grid) :
    for line in grid :
        print(line)
    print()
class Nonogram:
    def __init__(self, clues):
        self.top = clues[0]
        self.left = clues[1]
        self.grid = [ [' '  for _ in range(N) ] for _ in range(N) ]

    def place (self, blocks, total) : # ie : https://stackoverflow.com/questions/47159012/recursive-algorithm-to-find-all-possible-solutions-in-a-nonogram-row 
        if not blocks: return [0 * total]
        if blocks[0] > total: return []

        starts = total - blocks[0] # starts = 2 means possible starting indexes are [0,1,2]

        if len(blocks) == 1: # this is special case
            return [ ('0' * i + '1' * blocks[0] + '0' * (starts-i)) for i in range(starts + 1) ]

        ans = []
        for i in range(total - blocks[0]): # append current solutions
            for sol in self.place(blocks[1:], starts-i-1): # with all possible other solution
                ans.append('0' * i + '1' * blocks[0] + '0' + sol)

        return ans

    def reduce (self, grid, north, east) :
        for y in range(N) :
            for x in range(N) :
                if grid[y][x] != ' ' :    
                    east[y] = [ comb for comb in east[y] if comb[x] == grid[y][x] ]
                    north[x] = [ comb for comb in north[x] if comb[y] == grid[y][x] ]

                if len(north[x]) == 1 :
                    for i in range(N) :
                        grid[i][x] = north[x][0][i]

            if len(east[y]) == 1 :
                for i in range(N) :
                    grid[y][i] = east[y][0][i]

    def backtrack(self, north, east, x, y) :

        if x == N :
            x, y = 0, y + 1

        if y == N :


            return True

        for comb in north[x] :
            self.grid[y][x] = comb[y]
            if self.backtrack( north, east, x + 1, y) == True :
                return True
            self.grid[y][x] = ' '
        return False

    def solve(self):
        for _ in range(6) :
            north = [ self.place(cell, N) for cell in self.top ]
            east = [ self.place(cell, N) for cell in self.left ]

            self.reduce(self.grid, north, east)
            pass

        # print(north)
        # print(east)
        # display(self.grid)
        for line in east :
            for comb in line :
                print(comb)
            print()
        # self.backtrack(north, east, 0, 0)

        return self.grid


clues = (((1, 1), (4,), (1, 1, 1), (3,), (1,)), ((1,), (2,), (3,), (2, 1), (4,)))

clues = (((1,), (3,), (1,), (3, 1), (3, 1)), ((3,), (2,), (2, 2), (1,), (1, 2)))

ans = ((0, 0, 1, 1, 1),
       (0, 0, 0, 1, 1),
       (1, 1, 0, 1, 1),
       (0, 1, 0, 0, 0),
       (0, 1, 0, 1, 1))

clues = (((3,), (2,), (1, 1), (2,), (4,)), ((2,), (3, 1), (1, 2), (3,), (1,)))

ans = ((1, 1, 0, 0, 0),
       (1, 1, 1, 0, 1),
       (1, 0, 0, 1, 1),
       (0, 0, 1, 1, 1),
       (0, 0, 0, 0, 1))




curr = Nonogram(clues)
grid = curr.solve()


