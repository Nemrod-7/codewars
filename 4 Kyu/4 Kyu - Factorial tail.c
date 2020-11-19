#define MIN(a,b) (((a) < (b)) ? (a) : (b))

int zeroes (int base, int number) { // to fix //

    int i,p,c,zeroes = number,k;


      for (i = 2; i <= base; i++) {
          if (base % i == 0) {

             p = 0;
             while (base % i == 0) {
                   p++;
                   base /= i;
             }

             c = 0;
             k = number;
             while (k/i > 0) {
                   c += k/i;
                   k /= i;
             }

          zeroes = MIN(zeroes,c/p);
          }
      }

  return zeroes;
}
