import math

DIAGONAL = ( (-1,-1),(1,-1),(1,1),(-1,1) )
COMPASS = ( (0,-1),(1,0),(0,1),(-1,0) )

def from_str(src, color = 'white') :
    return [src[0], color, ord(src[1]) - 97, int(src[2]) - 1]

def to_str(player) :
    return player[0] + chr(player[2] + 97) + str(player[3] + 1 )

def manhatan(x1,y1, x2,y2) :
    return abs(x1-x2) + abs(y1-y2)

def euclidian(x1,y1, x2, y2) :
    a = abs(x1 - x2) 
    b = abs(y1 - y2)
    return math.hypot(a,b)
    # return sqrt(a**2 + b**2) 


def is_inside(x, y) :
    return x >= 0 and y >= 0 and x < 8 and y < 8

class WhitePlayer :
    def __init__(self, src) :
        white, black = src.split('-')
        white = white.split(',')

        a = from_str(black.strip(' '), 'black')
        b = from_str(white[0].strip(' '), 'white')
        c = from_str(white[1].strip(' '), 'white')
        
        self.board = [a,b,c]
        
        
    def check_rook(self, player, nx, ny) :

        for p in self.board :
            if p.color == player.color : continue
            if p.type == 'R' :
                if p.x == nx or p.y == ny : return False

        return True

    def check_king(self, player, nx, ny) :
        
        for dx, dy in COMPASS + DIAGONAL :
            sx, sy = nx + dx, ny + dy
            if sx == player.x and sy == player.y : continue

            for p in self.board :
                if p.color == player.color : continue
                if p.type == 'K' and sx == p.x and sy == p.y : return False

        return True

    def play(self, src) :
        player = from_str(src, 'black')
        # player = self.board[0]
        px, py = player[2], player[3]
        possible = []

        self.board[0] = player
        print(player)
        for piece in self.board :
            type, color, x, y = piece 
            if color == 'black' : continue 
            mult = 1
            move = COMPASS

            print(piece)
            if type == 'K' :
                move += DIAGONAL
            elif type == 'R' :
                mult = 8
    
            for dx,dy in move : 
                for i in range(1, mult + 1) :
                    nx = x + dx * i 
                    ny = y + dy * i
    
                    if is_inside(nx, ny) :
                        dist = manhatan(px,py,nx,ny)
                        # eucl = euclidian(px,py,nx,ny)

                        if dist == 0 :
                            print('check : ', nx, ny)
                            return to_str(['K', 'white,', nx, ny])
                        if dist == 2 :
                            for sx,sy in DIAGONAL :
                                if (nx + sx) == px and (ny + sy) == py :
                                    dist = 1
                                    break

                        if dist == 1 :


                            pass
                        else :

                            print(nx, ny, 'distance : ', dist)
                            possible.append(['K', 'white,', nx, ny])

            print()

        display(self.board)

def display (array) :
    white = { 'R':'♜', 'K':'♚', 'Q':'♛', 'P':'♟', 'N':'♞', 'B':'♝' }
    black = { 'R':'♖', 'K':'♔', 'N':'♘', 'B':'♗', 'Q':'♕', 'P':'♙', }

    board = [['◻' if (x + y) % 2 == 0 else ' ' for y in range(8)] for x in range(8)]
    for type,color,x,y in array :
        board[y][x] = white[type] if color == 'white' else black[type]

    for y in range(8) :
        for x in range(8) :
            print(board[y][x], end=' ')
        print()

src = 'Ke8,Rh7 - Kc8'
whiteplayer = WhitePlayer("Ke8,Rh7 - Kc8")
whitemove = whiteplayer.play("Kb8")



