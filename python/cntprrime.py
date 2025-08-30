import time
import numpy as np
import math

start = time.time()

def sieve2(limit:int) :
    WHEEL = [2,4,2,4,6,2,6,4,2,4,6,6,2,6,4,2,6,4,6,8,4,2,4,2,4,8,6,4,6,2,4,6,2,6,6,4,2,4,6,2,6,4,2,4,2,10,2,10]

    k, t, i, u = 11, 0, 11, 0
    size = ( (limit // 3) >> 6) + 1
    sieve = [0] * size

    cnt = 4
    vs = [2,3,5,7]

    while i <= limit :
        p = i // 3
        bit = sieve[p >> 6] >> (p & 63) & 1

        if bit == 0 :
            j, v = i * i, t

            cnt += 1
            vs.append(i)

            while j <= limit :
                p2 = j // 3
                sieve[p2 >> 6] |= 1 << (p2 & 63)

                j += i * WHEEL[v]
                v = 0 if v == 47 else v + 1

        i += WHEEL[t]
        t = 0 if t == 47 else t + 1
    
    return vs

def sieve3(limit) : 
    L1D_CACHE_SIZE = 32768  # Set your CPU's L1 data cache size in bytes
    wheel = [2,4,2,4,6,2,6,4,2,4,6,6,2,6,4,2,6,4,6,8,4,2,4,2,4,8,6,4,6,2,4,6,2,6,6,4,2,4,6,2,6,4,2,4,2,10,2,10]
    sqr = int(math.sqrt(limit))
    mem = max (sqr, L1D_CACHE_SIZE);

    i, k, t, u = 11, 11, 0, 0
    cnt = 4;
    cache, sieve = [0] * mem, [0] * (sqr + 1)
    primes, mul = [], []
    vs = [2,3,5,7]

    for low in range(0,limit,mem) :   
        high = min(low + mem - 1, limit)
        cache = [1] * mem
        # generate sieving primes using simple SOE
        while i * i <= high :
            if sieve[i] == False :
                j, v = i * i, t
                primes.append(i)
                mul.append(i * i - low)
              
                while j <= sqr :
                    sieve[j] = True
                    j == i * wheel[v]
                    v = 0 if v == 47 else v + 1
            
            i += wheel[t]
            t = 0 if t == 47 else t + 1
        # cache the current segment
        for p in range(0,len(primes)) :
            j = mul[p]

            while j < mem : # for j in range(j, mem, primes[p] * 2) :
                cache[j] = 0
                j += primes[p] * 2
            
            mul[p] = j - mem
        
        while k <= high :
            if cache[k - low] :
                cnt += 1
                vs.append(k)

            k += wheel[u]
            u = 0 if u == 47 else u + 1

    return vs


limit = 225
p1 = sieve2(limit)
p2 = sieve3(limit)

print(p1)
print(p2)

end = time.time()
print( "elapsed : ", end - start)
