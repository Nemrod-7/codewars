enum kBool {false,true};
typedef unsigned long long int uint64_t;
int fortune
(uint64_t f0, double p, int c0, int n, double inf) {

      int i;
      const double rate = (double)p/100;
      const double nflt = (double)inf/100;
      uint64_t dpst = f0;
      int wthdr = c0;

      for (i = 1; i <= n; ++i) {
          dpst += dpst * rate - wthdr;
          wthdr += wthdr * nflt;
             if (dpst <= 0) return 0;
      }
return  ((dpst > 0) ? true : false);
}
