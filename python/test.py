N = 6

def exist (x, bit) : return (x >> bit &1) == 1
def decompose(cell) :
    dig, vs = 0, []

    while cell :
        if (cell &1) : vs.append(dig)
        dig += 1
        cell >>= 1
    return vs

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
    elif a[0] == 0 and a[1] == b[1] :
        return True
    elif a[1] == 0 and a[0] == b[0] :
        return True
    elif a == b :
        return True
    return False

def check_num (now) :
    end, first, sec, index = N - 1, 0, 0, N - 1
    head, tail = 0,0

    while index >= 0 :
        if now[index] == 0 : return (-1,-1)
        if now[index] > sec :
            sec = now[index]
            tail += 1
        if now[N - 1 - index] > first :
            first = now[end - index]
            head += 1
        index -= 1

    return (head,tail)

def comb_subset(mask, clues) :
    start = [0] * N
    heap = [(0,0, start.copy())]

    while heap :
        [index, visit, comb] = heap.pop()

        if index == N :
            if equals(check_num(comb), clues) :
                print(comb)
                for i in range(N) : start[i] |= 1 << comb[i]
        else :
            for dig in decompose(mask[index]) :
                if not exist(visit, dig) :
                    comb[index] = dig
                    heap.append( ( index + 1, (visit | 1 << dig), comb.copy()))
    return start


full = (1 << (N + 1)) - 1;

full = 0
for i in range(1,N+1) :
    full |= 1 << i

row = [126] * N

if row == [full] * N :
    print(row)

# print(full)

