import itertools
N = 4

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


for actual in comb :
    [head,tail] = check_num(actual)
    print(head, actual, tail)

