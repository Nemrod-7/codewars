import random

def showboard(board) :
    N = len(board)
    for y in range(0,N) :
        for x in range(0,N) :
            if board[x] == y :
                print('[x]',end='')
            else :
                print('[ ]',end='')
        print()
    pass

class Qdata :
    def __init__(self, N) :
        self.row = [0 for _ in range(0,N)]
        self.d1 = [0 for _ in range(0, 2 * N - 1)]
        self.d2 = [0 for _ in range(0, 2 * N - 1)]

    def reset(self, board) :
        self.__init__(len(board))

        for i in range(0,len(board)) :
            self.row[board[i]] += 1
            self.d1[len(board) + i - board[i] - 1] += 1
            self.d2[i + board[i]] += 1

    def attack(self, x,y) :
        return self.row[y] + self.d1[len(self.row) + x - y - 1] + self.d2[x + y]

def rnd_walk(hist,val) :
    V = []
    for i in range(0,len(hist)) :
        if hist[i] == val :
            V.append(i)

    return V[random.randint(0, max(len(V) - 1, 0))]

def min_conflict(N, pos) :
    limit = 50 * N
    board, hist = [0] * N, [0] * N

    data = Qdata(N)
    data.reset(board)
    board[pos[0]] = pos[1]

    while limit :
        for x in range(0,N) :            # count conflicts for each column
            hist[x] = 0 if x == pos[0] else data.attack(x, board[x]) - 3

        if sum(hist) == 0 : return board # if there's no conflict return a solution
        x = rnd_walk(hist, max(hist))    # select the column which has the max value

        for y in range(0,N) :            # count conflict by row for the column
            hist[y] = data.attack(x,y)
        
        board[x] = rnd_walk(hist, min(hist))    # select the row wich create the min conflict
        data.reset(board)
        limit -= 1

    return []

def format(board) :
    oss = ''
    for y in range(0,len(board)) :
        for x in range(0,len(board)) :
            oss += 'Q' if board[x] == y else '.'
        oss += '\n'
    return oss

def solveNQueens(n,pos) :
    if n == 1 : return 'Q'
    if n <= 4 : return ''

    res = min_conflict(n,pos)


board = [7,4,3,4,0,1,0,5]
board = min_conflict(8, [1,1])


res = format(board)
# showboard(board)
print(res)

print()



        

