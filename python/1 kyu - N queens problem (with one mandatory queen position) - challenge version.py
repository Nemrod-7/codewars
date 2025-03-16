import random

def showboard(state) :
    n = len(state)
    for y in range(0,n) :
        for x in range(0,n) :
            if state[y] == x :
                print('[x]',end='')
            else :
                print('[ ]',end='')
        print()
    pass

class board :
    def __init__(self, n) :
        self.col = [0 for _ in range(0,n)]
        self.d1 = [0 for _ in range(0, 2 * n - 1)]
        self.d2 = [0 for _ in range(0, 2 * n - 1)]

    def reset(self, state) :
        self.__init__(len(state))

        for i in range(0,len(state)) :
            self.col[state[i]] += 1
            self.d1[len(state) + i - state[i] - 1] += 1
            self.d2[i + state[i]] += 1

    def attack(self, x,y) :
        return self.col[y] + self.d1[len(self.col) + x - y - 1] + self.d2[x + y]

def rnd_walk(hist,val) :
    V = [i for i in range(0,len(hist)) if hist[i] == val]
    return V[random.randint(0, max(len(V) - 1, 0))]

def min_conflict3(n, pos) :
    limit = 50 * n
    state, hist = [0] * n, [0] * n

    data = board(n)
    data.reset(state)
    state[pos[0]] = pos[1]

    while limit :
        for y in range(0,n) :            # count conflicts for each row
            hist[y] = 0 if y == pos[0] else data.attack(y, state[y]) - 3

        if sum(hist) == 0 : return state # if there's no conflict return a solution
        y = rnd_walk(hist, max(hist))    # select the row which has the max value

        for x in range(0,n) :            # count conflict by col for the column
            hist[x] = data.attack(y,x)
        
        state[y] = rnd_walk(hist, min(hist))    # select the col wich create the min conflict
        data.reset(state)
        limit -= 1

    return []

def reset(state) :
    n = len(state)
    col, d1, d2 = [0] * n, [0] * (2 * n - 1), [0] * (2 * n - 1)
    for i in  range(0,n):
        col[state[i]] += 1
        d1[len(state) + i - state[i] - 1] += 1
        d2[i + state[i]] += 1
    return [col,d1,d2]    

def min_conflict2(n, pos) :
    limit = 50 * n                                    # set the maximum number of steps to run
    hist = [0] * n
    
    state = [random.randrange(n) for _ in range(0,n)] # generate a random grid
    state[pos[1]] = pos[0]

    while limit :
        [col,d1,d2] = reset(state)

        for y in range(0,n) :            # count conflicts for each row
            x = state[y]
            hist[y] = 0 if y == pos[1] else col[x] + d1[n + y - x - 1] + d2[x + y] - 3

        if sum(hist) == 0 : return state # if there's no conflict return a solution
        y = rnd_walk(hist, max(hist))    # select the row which has the max value

        for x in range(0,n) :            # count conflict by col for the selected
            hist[x] = n if x == pos[0] else col[x] + d1[n + y - x - 1] + d2[x + y] 
        
        state[y] = rnd_walk(hist, min(hist)) # select the col wich create the min conflict
        limit -= 1
        
    return []

def format(state) :
    oss = [['.'  for _ in range(0,len(state))] for _ in range(0,len(state))]
    for i in range(0,len(state)): oss[state[i]][i] = 'Q'
    return '\n'.join (''.join(line) for line in oss) +'\n'

def solve_n_queens(size, pos):
    if size == 1 : return 'Q\n'
    if size <= 3 : return None
   """
    An instance represents a complete-state formulation of n-queens.

    Each instance has a fixed queen that cannot move.
    The state-space is restricted so that only 1 queen is allowed in
    each row of the chessboard. The board with positions of each
    queen is represented by a 1 dimensional array of length `n`.

    Each entry of the board is the position of the queen in the
    corresponding row. That is, `board[y] = x` represents a queen
    on the `x`th column of row `y`, where both rows and columns are
    zero-indexed.

    There are three lists used to allow O(n) conflict checking.
    `row` has length `n`.
    `d1` and `d2` are the number of queens on
    each of the main and cross diagonals, respectively.
    Each of these lists has length `2 * n - 1`.
    A queen `(r, c)` is on `d1[i]` if `n - 1 - x + y == i`.
    A queen `(r, c)` is on `d2[i]` if `x + y == i`.

    The solution uses a min-conflicts algorithm, with the following
    steps:
    1. Randomly generate a complete-state assignment. This consists of
    generating a random column for each non-fixed row to be occupied by
    a queen.
    2. Until the max number of steps have been run:
        a. If the current assignment is a solution, return it. (O(n)
        conflict checking).
        b. Randomly select a queen wich makes the maximum conflicts.
        c. For each unoccupied tile in the conflicted queen's row,
        determine the number of queens that can attack that tile. O(n).
        d. Randomly select a tile among all with the minimum number of
        conflicts and move that row's queen to that tile.
    3. Return `None` if no solution is found withing the max steps.
    """
    for _ in range(0,10):
        state = min_conflict2(size, pos)
        if state != [] : 
            showboard(state)
            return format(state)

    return None

state = [1,0,3,2]

showboard(state)

solve_n_queens(4, (2,2))

print()



        

