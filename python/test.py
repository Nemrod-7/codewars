import itertools
N = 7

comb = list(itertools.permutations([i + 1 for i in range(N)]))

def showmask(cell) :
    for i in range(1,N+1) :
        bit = (cell >> i&1)
        # print(bit, end=' ')
        if bit == 1 :
            print(i, end=' ')
        else :
            print(' ', end=' ')

def equals (a, b) :
    if b[0] == 0 and b[1] == 0 :
        return True
    elif b[0] == 0 and a[1] == b[1] :
        return True
    elif b[1] == 0 and a[0] == b[0] :
        return True
    elif a == b :
        return True
    return False

def check_num (now) :
    end, first, sec, index = N - 1, 0, 0, N - 1
    head, tail = 0,0

    while index >= 0 :
        if now[index] > sec :
            sec = now[index]
            tail += 1
        if now[N - 1 - index] > first :
            first = now[end - index]
            head += 1
        index -= 1

    return (head,tail)


def mkmask2(clues) :
    mask1 = [ [0 for x in range(N)] for y in range(N) ]
    mask2 = [ [0 for x in range(N)] for y in range(N) ]
    mask = [ [0 for x in range(N)] for y in range(N) ]

    for i in range(N) :
        west, east = ((N * 4) - 1) - i, N + i
        south, north = ((N * 4) - 1) - i - N, i

        horiz = (clues[west], clues[east])
        verti = (clues[north], clues[south])

        for actual in comb :
            if equals(check_num(actual), horiz) :
                for j in range(N) : mask1[i][j] |= 1 << actual[j]

            if equals(check_num(actual), verti) :
                for j in range(N) : mask2[j][i] |= 1 << actual[j]

    print()
    for y in range(N) :
        for x in range(N) :
            mask[y][x] = mask1[y][x] & mask2[y][x]
            showmask(mask1[y][x])
            print(end='|')
        print()

for i in range(N) :
    for actual in comb :

        pass
# mkmask2(clues)
