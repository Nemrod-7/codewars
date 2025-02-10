
def lucas3 ( n,  k) :

    prod = 1 #  => lucas (n k) = prod from i = 0 to infinity (binom(n_i k_i) mod p)

    while n or k :
        np, kp = n % 3, k % 3
        if np < kp : return 0
        if np == 2 and kp == 1 : prod *= 2 # prod *= binomial (np, kp);

        n /= 3
        k /= 3

    return prod % 3

def triangle (row) :
    N = len(row) - 1
    k, sum = N , 0
    colors = ['B','R','G']

    while (k >= 0) :
        sum += lucas3 (N, k) * ord( row[k]) % 3
        k -= 1

    # print(sum, colors[((-1 ** N) * sum) % 3])
    return colors[((-1 ** N) * sum) % 3]

basic_cases = [
        ['B', 'B'],
        ['GB', 'R'],
        ['RRR', 'R'],
        ['RGBG', 'B'],
        ['RBRGBRB', 'G'],
        ['RBRGBRBGGRRRBGBBBGG', 'G']
        ]

triangle(basic_cases[1][0])
