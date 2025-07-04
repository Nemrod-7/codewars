import itertools
N = 6

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

comb = list(itertools.permutations([i + 1 for i in range(N)]))
clues = (3,2,2,3,2,1, 1,2,3,3,2,2, 5,1,2,2,4,3, 3,2,1,2,2,4)

mask = [ [set() for x in range(N+2)] for y in range(N+2) ]

for i in range(N) :
    west, east = ((N * 4) - 1) - i, N + i
    south, north = ((N * 4) - 1) - i - N, i

    horiz = (clues[west], clues[east])
    verti = (clues[north], clues[south])

    for actual in comb :
        if equals(horiz, check_num(actual)) :
            for j in range(N) :
                mask[i][j].add(actual[j])
            # print(clues[west], actual, clues[east])

for y in range(N) :
    for x in range(N) :
        cmb = mask[y][x]

        for i in range(1, N + 1) :
            if i in cmb :
                print(i, end=' ')
            else :
                print(' ', end=' ')
        print(end='|')
    print()
#     grid[0][i + 1] = clues[north]
#     grid[N+1][i + 1] = clues[south]
#     grid[i+1][0] = clues[west]
#     grid[i+1][N + 1] = clues[east]
#
# for line in grid :
#     for dig in line :
#         if dig == 0 :
#             print(' ', end=' ')
#         else :
#             print(dig, end=' ')
#     print()
