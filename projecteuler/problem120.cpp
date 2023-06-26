#include <iostream>


int64_t modpow (int64_t base, int64_t exp, int64_t mod) {
    int64_t res = 1;

    while (exp > 0) {
        if ((exp & 1) > 0) res = (res * base) % mod;
        exp >>= 1;
        if (exp > 0)
            base = (base * base) % mod;
    }

    return res;
}
int main () {

    const int64_t mod = 1000000000;

    for (int64_t i = 3; i < 12; i++) {
        int64_t sq = i * i;
        int64_t maxv = 0;
        int64_t nu, pw;

        for (int64_t j = 1; j < 11; j++) {
            int64_t a = modpow (i - 1, j, mod), b = modpow (i + 1, j, mod);
            int64_t rem = (a + b) % sq;

            if (rem > maxv) {
                nu = i, pw = j;
                maxv = rem;
            }
            if (j % 2 == 1) {
                std::cout << a << " " << b << " " << sq << " => " << rem << "\n";
                //std::cout << i - 1 << "^" << j << " + " << i + 1 << "^" << j << " % " << sq << " = " << rem <<  "\n";
            }
            //std::cout << i << " "<< j << " => " << a+b << " % " << sq << " :: " << rem << "\n";
            //cout << i << " => " << rem << "\n";
        }
    //    std::cout << nu << " "<< pw << " => " << maxv <<  "\n";
        std::cout << "\n";
    }

}

