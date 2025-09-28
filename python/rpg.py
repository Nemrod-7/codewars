# ______                             _    _
# |  _  \                           | |  | |
# | | | |___ _ __ ___   ___  _ __   | |  | | __ _ _ __ ___
# | | | / _ \ '_ ` _ \ / _ \| '_ \  | |/\| |/ _` | '__/ __|
# | |/ /  __/ | | | | | (_) | | | | \  /\  / (_| | |  \__ \
# |___/ \___|_| |_| |_|\___/|_| |_|  \/  \/ \__,_|_|  |___/

# Run a tiny grid RPG simulation with a simple action grammar.
#
# Map legend (input `field` as list of equal-length strings):
#   - Walls:         '#'
#   - Doors:         '-' or '|' (can be unlocked with 'K')
#   - Empty space:   ' '
#   - Player:        one of '^', 'v', '<', '>'   (facing dir)
#   - Enemies:
#       'E' -> basic enemy (HP 1; atk 2)
#       'D' -> durable enemy (HP 10; atk 3)
#   - Merchants:     'M' (can be interacted with using coin 'C' from the tile AHEAD)
#   - Pickups (on the tile AHEAD, when moving 'F' onto them):
#       'C' -> coin (stored in bag; can be spent on 'M')
#       'K' -> key  (can unlock '-' or '|' in front with action 'K')
#       'H' -> potion (full heal to max HP)
#       'S' -> shield (permanent +1 defense)
#       'X' -> sword  (permanent +1 attack)
#
# Actions (characters in string `actions`, applied sequentially):
#   - '^', 'v', '<', '>' : rotate in place (change facing). After rotating,
#                          adjacent enemies make an opportunity attack.
#   - 'F'  : step forward to the cell ahead if it is walkable
#            (NOT one of '#', '-', '|', 'M', 'E'). Before stepping, adjacent
#            enemies (except one in the destination cell) make an opportunity attack.
#            If stepping onto a pickup tile, collect it and clear the tile.
#   - 'A'  : attack the cell ahead. If there is no enemy ahead, the action is invalid.
#            You strike first; if enemy HP drops to 0 or less, clear the tile.
#            Then adjacent enemies make an opportunity attack.
#            Every 3 basic enemy ('E') kills, your attack increases by +1.
#   - 'C'  : use a coin. Valid only if you have 'C' in the bag AND the tile ahead is 'M'.
#            Spends one coin; each merchant has 3 charges; when 0 → tile becomes ' '.
#   - 'K'  : use a key. Valid only if you have 'K' in the bag AND the tile ahead is '-' or '|'.
#            Consumes the key and clears the door tile.
#   - 'H'  : drink a potion (only if current HP < max HP). Heals to max HP, then
#            adjacent enemies make an opportunity attack.
#
# Combat details:
#   - Player stats: starting HP=3, Attack=1, Defense=1, max HP=3 (S and X boost def/atk respectively).
#   - Enemies attack as "opportunity" whenever you rotate, move, attack, or drink.
#   - Damage from a single enemy = max(0, (E? 2 : 3) - defense).
#   - If HP <= 0 at any time, return None (death).
#   - Basic enemy 'E' HP=1; durable enemy 'D' HP=10. (Both represented in an HP grid.)
#
# Movement/collision:
#   - You cannot step onto: '#', '-', '|', 'M', 'E'. Doors and merchants are interacted
#     with from the tile you’re standing on (i.e., in the cell AHEAD).
#   - You CAN step onto: ' ', 'C', 'K', 'H', 'S', 'X', 'D' is never stepped onto
#     (it’s represented in the enemies-HP grid; on the visible map, the tile remains 'D'
#     until killed and then becomes ' ').
#
# Notes:
#   - Duplicate items are allowed in the bag (e.g., multiple coins/keys).
#   - Merchants start with exactly 3 charges.
#   - Level-up: every 3 kills of basic enemies ('E') → +1 attack.

def display(m) :
    for i in range(len(m)) :
        for j in range(len(m[0])) :
            print(m[i][j], end='')
        print()
    print()

def is_wall (ch) : return ch == '#'
def is_pnj (ch) : return ch == 'M' or ch == 'D' or ch == 'E'
def is_object (ch) : return ch == 'C' or ch == 'K' or ch == 'H'
def is_inside(x,y, width, height) : return x >= 0 and y >= 0 and x < width and y < height

def player(field) :
    # stats = [3,1,1,0]
    # inventory = [0,0,0]
    position = [ (j, i, arrow.find(field[i][j])) for i in range(len(field)) for j in range(len(field[0])) if field[i][j] in arrow ][0]
    return [ [3,1,1,0], [0,0,0], position]

class player () :
    def __init__(self, field) :
        self.hp = 3
        self.atk = 1
        self.dfc = 1
        self.lev = 0

def rpg1( field: list[list[str]], actions: list[str]) :
    arrow ='^>v<'
    actions = 'F^>v<ACKH'
    compass = [(0,-1),(1,0),(0,1),(-1,0)]
    width, height = len(field[0]), len(field)

    pnj = [[0 for x in range(width)] for y in range(height)]
    map = [[field[y][x] for x in range(width)] for y in range(height)]

    potion,coin,key = 0,0,0
    health,attack,defense,level = 3,1,1,0
    x,y,direction = [ (j, i, arrow.find(field[i][j])) for i in range(height) for j in range(width) if field[i][j] in arrow ][0]
    map[y][x] = ' '

    # preprocess
    for i in range(height) :
        for j in range(width) :
            match field[i][j] :
                case 'E' : pnj[i][j] = 1
                case 'M' : pnj[i][j] = 3
                case 'D' : pnj[i][j] = 10

    # player action
    for action in actions :
        dx,dy = compass[direction]
        nx,ny,sx,sy = x, y, x + dx, y + dy

        match action :
            case '^' : direction = 0
            case '>' : direction = 1
            case 'v' : direction = 2
            case '<' : direction = 3
            case 'F' : nx,ny = sx, sy
            case 'A' :
                if map[sy][sx] == 'E' or map[sy][sx] == 'D' :
                    pnj[sy][sx] -= attack

                    if pnj[sy][sx] <= 0 :
                        level += 1
                else : return None
            case 'C' : # => Coin [object].
                if map[sy][sx] == 'M' and coin :
                    coin -= 1
                    pnj[sy][sx] -= 1
                else : return None
            case 'K' : # => Key [object].
                if map[sy][sx] in '|-'and key :
                    key -= 1
                    map[sy][sx] = ' '
                else : return None
            case 'H' : # => Health [object].
                if potion and health < 3 :
                    potion -= 1
                    health = 3
                else : return None

        # enemies turn
        for dx,dy in compass :
            sx,sy = x + dx, y + dy

            if is_inside(sx,sy, width, height) and is_pnj(map[sy][sx]) :
                if pnj[sy][sx] <= 0 :
                    pnj[sy][sx] = 0
                    map[sy][sx] = ' '

                match map[sy][sx] :
                    case 'E' : health -= max(0, 2 - defense)
                    case 'D' : health -= max(0, 3 - defense)

        if level == 3 : attack, level = attack + 1, 0
        if health <= 0 or not is_inside(nx,ny,width,height) : return None

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

    map[y][x] = arrow[direction]
    bag = ['C'] * coin + ['H'] * potion + ['K'] * key
    return (map, health, attack, defense, bag)

def rpg(field) :
    arrow ='^>v<'
    actions = 'F^>v<ACKH'
    compass = [(0,-1),(1,0),(0,1),(-1,0)]

    width, height = len(field[0]), len(field)
    x,y,direction = [ (j, i, arrow.find(field[i][j])) for i in range(height) for j in range(width) if field[i][j] in arrow ][0]

    pnj = [[0 for x in range(width)] for y in range(height)]
    map = [[field[y][x] for x in range(width)] for y in range(height)]
    heap = [[map, pnj, [x, y, direction, 3, 1, 1, 0, 0, 0, 0]]]
    map[y][x] = ' '

    # preprocess
    for i in range(height) :
        for j in range(width) :
            match field[i][j] :
                case 'E' : pnj[i][j] = 1
                case 'M' : pnj[i][j] = 3
                case 'D' : pnj[i][j] = 10

    while heap :
        map, pnj, state = heap.pop()
        x,y, dir, hp, attack, defence, level, potion, coin, key = state

        if hp <= 0 or not is_inside(x, y, width, height) :
            continue
        if level == 3 :
            attack, level = attack + 1, 0
        if potion and hp < 3 :
            hp, potion = 3, potion - 1

        match map[y][y] :
            case 'K' : key += 1
            case 'C' : coin += 1
            case 'H' : potion += 1
            case 'X' : attack += 1
            case 'S' : defense += 1
            case ' ' : pass
            case  _  : continue

        map[y][x] = arrow[dir]
        display(map)
        map[y][x] = ' '
        # enemys action
        for dx,dy in compass :
            sx,sy = x + dx, y + dy

            if is_inside(sx,sy, width, height) and is_pnj(map[sy][sx]) :
                if pnj[sy][sx] <= 0 :
                    pnj[sy][sx] = 0
                    map[sy][sx] = ' '

                match map[sy][sx] :
                    case 'E' : hp -= max(0, 2 - defence)
                    case 'D' : hp -= max(0, 3 - defence)

        dx,dy = compass[dir]
        sx,sy = x + dx, y + dy

        if is_inside(sx,sy, width, height) :
            if map[sy][sx] == ' ' or is_object(map[sy][sx]) :
                heap.append( (map,pnj, [sx,sy, dir, hp, attack, defence, level, potion, coin, key]) ) # action => 'F'
            else :
                for i in range(4) :
                    dx,dy = compass[i]
                    # if is_inside(x + dx,y + dy, width, height) :
                    #     heap.append( (map,pnj, [x,y, i, hp, attack, defence, level, potion, coin, key]) )
                    pass
                nexm = [ [map[y][x] for x in range(width)] for y in range(height) ]
                nexp = [ [pnj[y][x] for x in range(width)] for y in range(height) ]

                match map[sy][sx] :
                    case 'E' :
                        nexp[sy][sx] -= attack

                        if nexp[sy][sx] <= 0 :
                            heap.append(( nexm,nexp, [x,y,direction, hp, attack, defence, level + 1, potion, coin, key] ))
                        else :
                            heap.append(( nexm,nexp, [x,y,direction, hp, attack, defence, level + 1, potion, coin, key] ))
                    case 'D' :
                        nexp[sy][sx] -= attack

                        if nexp[sy][sx] <= 0 :
                            'win'
                        else :
                            heap.append(( nexm,nexp, [x,y,direction, hp, attack, defence, level + 1, potion, coin, key] ))
                    case 'M' :
                        nexp[sy][sx] -= 1
                        heap.append(( nexm,nexp, [x,y,direction, hp, attack, defence, level, potion, coin - 1, key]))
                    case '-' :
                        nexm[sy][sx] = ' '
                        heap.append( (nexm,nexp, [x,y, dir, hp, attack, defence, level, potion, coin, key - 1]) )
                    case '|' :
                        nexm[sy][sx] = ' '
                        heap.append( (nexm,nexp, [x,y, dir, hp, attack, defence, level, potion, coin, key - 1]) )

map = [
'K   E     |    X#        C      E   S#  D  ',
'    E     #######               ###### E E ',
'##### #####                          ###-##',
'     M                                 EEE ',
'                     ###   ###             ',
'                     #       ##E##E##      ',
'     ^               #          C  K#      ',
'              C      #   H   ##E##E##      ']

m = [
    'SX#  EDE ',
    '  # EEEEE',
    '#-#     X',
    '         ',
    '^       K',
]

rpg(m)


#
# def test_case(fun, map, action) :
#     actual = fun(map, action)
#
#     if actual == None : return
#     m = actual[0]
#     stat = actual[1:]
#
#     print(stat)
