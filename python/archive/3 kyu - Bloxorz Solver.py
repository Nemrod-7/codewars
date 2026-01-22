import heapq

COMPASS = [[[0,-1],'U'],[[1,0],'R'],[[0,1],'D'],[[-1,0],'L']]

def is_inside(p, grid) : return p[0] >= 0 and p[1] >= 0 and p[0] < len(grid[0]) and p[1] < len(grid)
def is_free(p, grid) : return is_inside(p, grid) and grid[p[1]][p[0]] != '0'

def blox_solver(grid):
    start = [(x,y) for y in range(len(grid)) for x in range(len(grid[0])) if grid[y][x] == 'B'][0]
    exit = [(x,y) for y in range(len(grid)) for x in range(len(grid[0])) if grid[y][x] == 'X'][0]

    visit = {}
    queue = [[0, (start,start), '']]
    heapq.heapify(queue)
    # show_map(grid)
    while queue :
        dist, curr, path = heapq.heappop(queue)
        standing = True if curr[0] == curr[1] else False

        if curr[0] == exit and standing :
            return path

        for (nx,ny), letter in COMPASS :
            dx,dy = curr[1][0] - curr[0][0], curr[1][1] - curr[0][1]
            b1 = (curr[0][0] + nx, curr[0][1] + ny)
            b2 = (curr[0][0] + nx * 2, curr[0][1] + ny * 2) if standing else (curr[1][0] + nx, curr[1][1] + ny)

            if is_free(b1, grid) and is_free(b2,grid) :
                route = path + letter

                if standing or (dx != nx and dy != ny) :
                    block = (b1, b2)
                else :
                    block = (b2, b2) if (dx == nx and dy == ny) else (b1, b1)

                if block not in visit :
                    visit[block] = True
                    heapq.heappush(queue, [len(route), block, route])

    return ''

def show_map(grid) :
    height, width = len(grid), len(grid[0])

    for y in range(height) :
        for x in range(width) :
            match grid[y][x] :
                case '0' : print(' ', end=' ')
                case '1' : print('.', end=' ')
                case 'X' : print('s', end=' ')
                case 'B' : print('x', end=' ')
                case  _  : print(' ', end=' ')
        print()
    print()


example_tests = [
	['1110000000', '1B11110000', '1111111110', '0111111111', '0000011X11', '0000001110'],
	['000000111111100', '111100111001100', '111111111001111', '1B11000000011X1', '111100000001111', '000000000000111'],
	['00011111110000', '00011111110000', '11110000011100', '11100000001100', '11100000001100', '1B100111111111', '11100111111111', '000001X1001111', '00000111001111'],
	['11111100000', '1B111100000', '11110111100', '11100111110', '10000001111', '11110000111', '11110000111', '00110111111', '01111111111', '0110011X100', '01100011100'],
	['000001111110000', '000001001110000', '000001001111100', 'B11111000001111', '0000111000011X1', '000011100000111', '000000100110000', '000000111110000', '000000111110000', '000000011100000']
]

example_sols = [['RRDRRRD','RDDRRDR','RDRRDDR'],['ULDRURRRRUURRRDDDRU','RURRRULDRUURRRDDDRU'],['ULURRURRRRRRDRDDDDDRULLLLLLD'],['DRURURDDRRDDDLD'],['RRRDRDDRDDRULLLUULUUURRRDDLURRDRDDR','RRRDDRDDRDRULLLUULUUURRDRRULDDRRDDR','RRRDRDDRDDRULLLUULUUURRDRRULDDRRDDR','RRRDDRDDRDRULLLUULUUURRRDDLURRDRDDR']]

for i,level in enumerate(example_tests):
    result = blox_solver(level)
    expect = example_sols[i]

    if result not in expect :
        print('wrong')

print('end')
