#include <iostream>
#include <cmath>

#include <iomanip>

using namespace std;
/*
ex : 8
1 23 456 789 10
1 12 123 1234
*/
uint64_t pref (uint64_t n) {
    uint64_t num = n * (n + 1) * 0.5;
    if (n >= 10) num += (n - 9) * (n - 8) * 0.5;
    if (n >= 100) num += (n - 99) * (n - 98) * 0.5;
    return num;
}
uint64_t gen (uint64_t n) { // generate smarandache concanated sequence
    const uint64_t n2 = sqrt (8 * n - 7);
    return n - 0.5 * floor ((n2 + 1) * 0.5) * floor ((n2 - 1) * 0.5);
}
int a007376 (int n) {

    uint64_t dig = 1, val  = 1, lim  = 9;

    while (n > val * lim) {
        n -= val * lim;
        val++;
        lim = lim * 10;
        dig = dig * 10;
    }

    dig += (n - 1) / val;
    string s = to_string (dig);
    return s[(n - 1) % val] - '0';
}

int solve (int num) {

  int index = 0, tmp = num, pre = 0;

  while (true) {
      if (tmp - pre <= 0) break;
      pre = pref (index);

      tmp -= pre;
      index++;
  }

  //cout << tmp << ' ' << pre << " " << a007376 (num - pre) << endl;
  return pre;
}

long bSearch (long n) {
   long res = 0, i = 1, k = 9, j = 9;
   for (; j < n; ++i, k *= 10, j += k){
       res += i * k * (k + 1) / 2;
       res += i * k * (n - j);
   }

   k = n - j / 10;
   res += i * k * (k + 1) / 2;

   return res;
}
int solve2 (long n) {

    long num = n, lb = 0, rb = (long)1e9;
    n--;

    while (lb < rb){
       long mb = (lb + rb + 1) * 0.5;
       if (pref (mb) > n)
          rb = mb - 1;
       else
          lb = mb;
    }

    n -= pref (lb);
    cout << a007376 (num - pref (lb)) << " ";

    long cnt = 9, len = 1;

    while (n >= cnt * len){
       n -= cnt * len;
       cnt *= 10;
       ++len;
    }

    //cout << cnt / 9 + n / len;
    //string x = String.valueOf(cnt / 9 + n / len);

    int y = (int)(n % len);
    return cnt / 9 + n / len;
}

int main () {
  /*
  for (int i = 1; i < 20; i++)
      cout << setw(3) << i;

      cout <<endl;
  for (int i = 1; i < 8; i++)
      for (int j = 1; j < i; j++)
          cout << setw(3) << j;

  cout << "\n\n";
  */
  for (int i = 1; i < 500; i++)

      cout << setw(3) << i << " :: " << setw(2) << solve2 (i) << " :: " << pref (i) << endl;

  //solve (1);
  return 0;
}
