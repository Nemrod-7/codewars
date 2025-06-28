
def sweep(water, grid, i, j, height) :
    new = max(grid[i][j], height)

    if new < water[i][j] :
        water[i][j] = new

        if i > 0 : sweep(water, grid, i - 1, j, new)
        if j > 0 : sweep(water, grid, i, j - 1, new)
        if i < len(water) - 1 : sweep(water, grid, i + 1, j, new)
        if j < len(water[0]) - 1 : sweep(water, grid, i, j + 1, new)

def volume(grid) :
    height, width = len(grid), len(grid[0])

    top = max([ max(it) for it in grid ])
    water = [[top for x in range(width)] for y in range(height)]

    for i in range(height) :
        sweep(water, grid, i, 0, grid[i][0])
        sweep(water, grid, i, width - 1, grid[i][width - 1])
    
    for j in range(width) :
        sweep(water, grid, 0, j, grid[0][j])
        sweep(water, grid, height - 1, j, grid[height - 1][j])
    
    return sum([ water[y][x] - grid[y][x] for y in range(height) for x in range(width)])

grd = [

[8, 8, 8, 8, 6, 6, 6, 6],
[8, 0, 0, 8, 6, 0, 0, 6],
[8, 0, 0, 8, 6, 0, 0, 6],
[8, 8, 8, 8, 6, 6, 6, 0]

]

volume(grd)
