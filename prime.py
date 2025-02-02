import time

# Prime Streaming (NC-17)
start = time.time()

def eratosthenes () :

    LIMIT = int(1e7)
    WHEEL = [2,4,2,4,6,2,6,4,2,4,6,6,2,6,4,2,6,4,6,8,4,2,4,2,4,8,6,4,6,2,4,6,2,6,6,4,2,4,6,2,6,4,2,4,2,10,2,10]

    i, t = 11, 0
    prime = [2,3,5,7]
    sieve = [True] * (LIMIT >> 1)

    while i < LIMIT :
        if sieve[i >> 1] == True :
            v, j = t, i * i
            prime.append(i)

            while j < LIMIT :
                sieve[j >> 1] = False
                j += i * WHEEL[v]
                v = v + 1 if v < 47 else 0

        i += WHEEL[t]
        t = t + 1 if t < 47 else 0

    return prime

sieve = eratosthenes()

def primes () : yield from sieve

p = primes()


end = time.time()
print( "elapsed : ", end - start)
