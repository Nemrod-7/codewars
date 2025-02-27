import math

def eratosthenes () :
    limit = 16000000

    i = 5
    step = 2
    prime = [2,3]
    sieve = [True] * (limit >> 1)

    while i < limit :
        if sieve[i >> 1] == True :
            prime.append(i)
            j = i * i
            t = step

            while j < limit :
                sieve[j >> 1] = False
                j += i * t
                t = 6 - t

        i += step
        step = 6 - step

    return prime

class Primes:
    prime = eratosthenes()

    @staticmethod
    def stream():
        yield from Primes.prime
