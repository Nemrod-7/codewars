
import time
import array

start = time.time()

L1 = 32768
LIMIT = int(1e8)

WHEEL = array.array('I',[2,4,2,4,6,2,6,4,2,4,6,6,2,6,4,2,6,4,6,8,4,2,4,2,4,8,6,4,6,2,4,6,2,6,6,4,2,4,6,2,6,4,2,4,2,10,2,10])
vs = [2,3,5,7]

class eratos : # segmented SOE with advanced wheel
    def __init__ (self) :
        self.low, self.high = 0, 0
        self.k, self.t, self.i, self.u = 11, 0, 11, 0
        self.sieve, self.cache = [0] * L1, [0] * L1
        self.hist = []

    def __next__ (self) :
        prime = 0

        while True :
            while self.k < self.high :
                if self.cache[self.k - self.low] == 0 :
                    prime = self.k
                self.k += WHEEL[self.t]
                self.t = 0 if self.t == 47 else self.t + 1
                if prime != 0 : return prime

            if self.high >= LIMIT : return 'limit exceded'

            if self.k >= self.high :
                self.high = min(self.high + L1, LIMIT);
                self.low = self.high - L1;
                self.cache = [0] * L1

                while self.i * self.i <= self.high :
                    if self.sieve[self.i] == 0 :
                        j, v = self.i * self.i, self.u
                        self.hist.append ([self.i, self.i * self.i - self.low])

                        while j <= L1 :
                            self.sieve[j] = 1
                            j += self.i * WHEEL[v]
                            v = 0 if v == 47  else v + 1

                    self.i += WHEEL[self.u]
                    self.u = 0 if self.u == 47 else self.u + 1

                for i in range(len(self.hist)) :
                    j = self.hist[i][1]

                    while j < L1 :
                        self.cache[j] = 1
                        j += self.hist[i][0] * 2

                    self.hist[i][1] = j - L1

sieve = eratos()

class primes :
    def __init__(self) :
        self.it = 0
    def __next__(self) :
        while self.it >= len(vs) :
            vs.append(next(sieve))

        p = vs[self.it]
        self.it += 1
        return p

stream = primes()

# for i in range(100) :
#     print(next(stream), end=' ')
# print()

for i in range(500000) :
    next(sieve)
    # print(next(stream), end=' ')
print()

end = time.time()
print( "elapsed : ", end - start)
