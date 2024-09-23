#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long int u64;

typedef struct {
    u64 *sieve, index;
    u64 i, t;
} prime_stream_t;

prime_stream_t *prime_stream_new() {

    const u64 limit = 100;
    const u64 hal = ((limit / 3) >> 6) + 1; // divide limit by 192

    prime_stream_t *prime = malloc(sizeof(prime_stream_t));
    prime->sieve = malloc(hal * sizeof(u64));
    prime->index = 0;
    prime->i = 11;
    prime->t = 0;

    return prime;
}

unsigned prime_stream_next(prime_stream_t *stream) {

    u64 prime = 0;
    const u64 limit = 100;

    if (stream->index < 4) {
        const u64 vs[4] = {2,3,5,7};
        prime = vs[stream->index++];

        return prime;
    } else {
        const u64 wheel[48] = {2,4,2,4,6,2,6,4,2,4,6,6,2,6,4,2,6,4,6,8,4,2,4,2,4,8,6,4,6,2,4,6,2,6,6,4,2,4,6,2,6,4,2,4,2,10,2,10};

        u64 i = stream->i, t = stream->t;

        printf("%llu ", i);
        for (; i <= limit ; i += wheel[stream->t], stream->t = stream->t == 47 ? 0 : stream->t + 1) {
            u64 p = 0xAAAAAAABULL * i >> 33;           // fast division by 3
            u64 mask = stream->sieve[p >> 6] >> (p &63) &1ULL; // x >> 6 => fast division by 64 / x &63 => fast modulus 64


            if (mask == 0) {
                for (u64 j = i * i, v = t; j <= limit; j += i * wheel[v], v = v == 47 ? 0 : v + 1) {
                    u64 p2 = 0xAAAAAAABULL * j >> 33;
                    stream->sieve[p2 >> 6] |= 1ULL << (p2 &63);
                }

                stream->i = i + wheel[t], stream->t = t == 47 ? 0 : t + 1;
                stream->index++;
                return i;
            }
        }
    }

    return 0;
}

void prime_stream_destroy(prime_stream_t *stream) {
    free(stream->sieve);
    free(stream);
}


int main () {

    prime_stream_t *curr = prime_stream_new();


    for (int i = 0; i < 7; i++) {
        u64 next = prime_stream_next(curr);

        //printf("%llu ", next);
    }

    prime_stream_destroy(curr);
}
