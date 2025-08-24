import math 

def sieve1(n:int) -> int:
    if n < 2 : return 0

    i = 3
    sieve = [True] * (math.floor(n / 2) + 1)
    prime = [2]

    while i < n :
        if sieve[math.floor(i / 2)] == True :
            j = i * i
            prime.append(i)

            while j < n :
                sieve[math.floor(j / 2)] = False
                j += 2 * i

        i += 2
    
    return prime

def sieve2(n:int) -> int:
    if n < 2 : return 0

    i = 5
    cnt = 2
    step = 2
    sieve = [True] * ((n // 3) + 1)

    while i < n :
        if sieve[i // 3] == True :
            j = i * i
            t = step
            cnt += 1

            while j < n :
                sieve[j // 3] = False
                j += i * t
                t = 6 - t

        i += step
        step = 6 - step
    
    return cnt

# prim = sieve1(420)
seca = sieve2(420)

# print(prim)
print(seca)
