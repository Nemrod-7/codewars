
def is_pnj (ch) : return ch == 'M' or ch == 'D' or ch == 'E' 
def is_inside(x,y, width, height) : return x >= 0 and y >= 0 and x < width and y < height

def rpg( field: list[list[str]], actions: list[str]) : 
    imprint ='^>v<'
    compass = [(0,-1),(1,0),(0,1),(-1,0)]
    width, height = len(field[0]), len(field)
    
    pnj = [[0 for x in range(width)] for y in range(height)]
    map = [[field[y][x] for x in range(width)] for y in range(height)]

    x,y,direction = 0,0,0
    potion,coin,key = 0,0,0
    health,attack,defense,level = 3,1,1,0

    # preprocess
    for i in range(height) :
        for j in range(width) :
            match field[i][j] :
                case 'E' : pnj[i][j] = 1
                case 'M' : pnj[i][j] = 3
                case 'D' : pnj[i][j] = 10
                case '^' : x,y,direction = j, i, 0
                case '>' : x,y,direction = j, i, 1
                case 'v' : x,y,direction = j, i, 2
                case '<' : x,y,direction = j, i, 3
    
    map[y][x] = ' '
    # player action
    for action in actions :
        dx,dy = compass[direction]
        sx,sy = x + dx, y + dy
        nx,ny = x,y
        
        match action :
            case 'F' :
                nx,ny = sx, sy
            case '^' :
                direction = 0
            case '>' :
                direction = 1
            case 'v' :
                direction = 2
            case '<' :
                direction = 3
            case 'A' :
                if map[sy][sx] == 'E' or map[sy][sx] == 'D' :
                    pnj[sy][sx] -= attack
                    
                    if pnj[sy][sx] <= 0 : 
                        level += 1
                else : return None
            case 'C' : # C => Coin [object].
                if map[sy][sx] == 'M' and coin :
                    coin -= 1
                    pnj[sy][sx] -= 1
                else : return None
            case 'K' : # K => Key [object].          
                if map[sy][sx] in '|-'and key :
                    key -= 1
                    map[sy][sx] = ' '
                else : return None
            case 'H' : # H => Health [object].
                if potion :
                    potion -= 1
                    health = 3
                else : return None
        
        # enemies turn
        for dx,dy in compass :
            sx,sy = x + dx, y + dy

            if is_inside(sx,sy, width, height) and is_pnj(map[sy][sx]) :
                if pnj[sy][sx] <= 0 :
                    map[sy][sx] = ' '
                    pnj[sy][sx] = 0

                if map[sy][sx] == 'E' :
                    health -= max(0, 2 - defense)
                if map[sy][sx] == 'D' :
                    health -= max(0, 3 - defense)
        
        if level == 3 : attack, level = attack + 1, 0
        if health < 0 or not is_inside(nx,ny,width,height) : return None

        match map[ny][nx] : 
            case 'K' : key += 1
            case 'C' : coin += 1
            case 'H' : potion += 1
            case 'X' : attack += 1
            case 'S' : defense += 1
            case ' ' : pass
            case _ : return None

        x,y = nx,ny  
        map[y][x] = ' '

    map[y][x] = imprint[direction]
    bag = ['C'] * coin + ['H'] * potion + ['K'] * key
    return (map, health, attack, defense, bag)

# map = [
# 'K   E     |    X#        C      E   S#  D  ',
# '    E     #######               ###### E E ',
# '##### #####                          ###-##',
# '     M                                 EEE ',
# '                     ###   ###             ',
# '                     #       ##E##E##      ',
# '     ^               #          C  K#      ',
# '              C      #   H   ##E##E##      ']
# path = []
# path += ['F'] * 2 + ['v'] + ['F'] * 4 + ['>'] + ['F'] * 15
# path += ['^'] + ['F'] * 15 + ['>'] + ['F'] * 18 + ['<']
# path += ['F'] * 11+ ['v'] + ['F'] * 16 + ['^']
# path += ['F'] * 1 + ['>'] + ['F'] * 13
#
# stats = rpg(map, path)
# print(stats[1:])
def display(m) :
    for i in range(len(m)) :
        for j in range(len(m[0])) :
            print(m[i][j], end='')
        print()
    print()

def test_case(fun, map, action) :
    actual = fun(map, action)

    if actual == None : return
    m = actual[0]
    stat = actual[1:]

    print(stat)
