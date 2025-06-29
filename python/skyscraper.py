import itertools

# {{4,1,2,3},{4,2,1,3},{4,1,3,2},{4,2,3,1},{4,3,1,2},{4,3,2,1}},
# {{3,1,2,4},{3,2,1,4},{1,4,2,3},{2,1,4,3},{2,4,1,3},{3,1,4,2},{3,2,4,1},{3,4,1,2},{1,4,3,2},{2,4,3,1},{3,4,2,1}},
# {{1,3,2,4},{2,1,3,4},{2,3,1,4},{1,2,4,3},{1,3,4,2},{2,3,4,1}},
# {{1,2,3,4}}};

N = 4
def check_num (data) :
    i, maxv = 0, 0
    head, tail = 0,0

    while (i < N) :
        if (data[i] > maxv) : head += 1
        maxv = max(maxv, data[i])
        i += 1

    i, maxv = N - 1, 0
    while (i >= 0) :
        if ( data[i] > maxv) : tail += 1
        maxv = max(maxv, data[i]);
        i -= 1
    
    print(head, data, tail)
    return [head, tail]

def recurse(grid, x, y) :



    for dig in range(1,N+1) :
        grid[y][x] = dig


def solve_puzzle (clues):
    grid = [ [0 for x in range(N)] for y in range(N) ] 
    line = [i + 1 for i in range(N)]
    comb = list(itertools.permutations(line))


    for line in comb :
        check_num(line)


    return grid





clues = (
( 2, 2, 1, 3,  
  2, 2, 3, 1,  
  1, 2, 2, 3,  
  3, 2, 1, 3 ),
( 0, 0, 1, 2,   
  0, 2, 0, 0,   
  0, 3, 0, 0, 
  0, 1, 0, 0 )
)

outcomes = (
( ( 1, 3, 4, 2 ),       
  ( 4, 2, 1, 3 ),       
  ( 3, 4, 2, 1 ),
  ( 2, 1, 3, 4 ) ),
( ( 2, 1, 4, 3 ), 
  ( 3, 4, 1, 2 ), 
  ( 4, 2, 3, 1 ), 
  ( 1, 3, 2, 4 ) )
)


res = solve_puzzle(clues[0])
print(res)
