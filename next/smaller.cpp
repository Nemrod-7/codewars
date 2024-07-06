#include <iostream>
#include <fstream>
#include <vector>

#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

using u64 =  unsigned long int;

bool check (u64 b, u64 x) {
    u64 z = x;

    while (z % b == 1) {
        z = (z - 1) / b;
        cout << z << " " << flush;
    }

    return z == 0;
}

u64 get_min_base (u64 x) {

  for (u64 n = ceil(log2(x)); n > 2; n--) {
      u64 b = floor(pow (x, 1 / n));
      cout << b - 1 << " " << x << "\n" << flush;
      if (check (b, x)) return b;
      // if (check (b - 1, x)) return b - 1;
  }

  return x - 1;
}

u64 get_min_base1 (u64 num) {

    for (u64 b = 2 ; b * b <= num; b++) {
        if (num % b != 1) continue;
        u64 pow = b + 1;

        while (pow <= num) {

            if (pow == num) return b;
            if (numeric_limits<u64>::max() / b < pow) break;

            pow = pow * b + 1;
        }
    }

    return num - 1;
}


void dotest(u64 number, u64 expected) {

    u64 res = get_min_base(number);
    if (res != expected) {
        cout << "error : " << number << "expected : " << expected << " got : " << res <<  "\n" ;
    }
}
void test() {

  vector<u64> sample = {673564044601506,558058364837027,431592636922903,115461762697991,138885830530222,733762420777309,417340353197959,112076136946983,616654913066283,357253495660880,819287049830251,140341054286692,76241243375083,224696224198318,124225533493649,721569864869151,745898190608135,822428316264997,199618905476200,300391382801185,161292826752289,96330133956611,191160824523934,837719774358691,234680349236169,791688469705891,354717061024797,378362442381519,53103481945660,293832940372192,175979798744173,295555467117624,480944742542459,125621710992148,247899905002313,856715146424028,25832349117448,545599763142656,137713683536157,890265552224223,384944933857907,169576028409195,128841859939042,352759353320557,889438687893958,121838788696063,562194989253808,758020725281721,84974426023363,878892759644742};


  for (auto [n, expected] : vector<pair<u64,u64>> {{3, 2}, {7, 2}, {21, 4}, {57, 7}, {1111, 10}}) {
      dotest(n, expected);
  }

  // for (auto [n, expected] : vector<pair<u64,u64>> {{1000002, 1000001}, {1000000002, 1000000001}, {1000000000000, 999999999999}}) {
  //     dotest(n, expected);
  // }
  //
  // for (auto [n, expected] : vector<pair<u64,u64>> {{1001001, 1000}, {1001001001, 1000}, {1001001001001, 1000}, {2500050001, 50000},
  // {125002500050001, 50000}}) {
  //     dotest(n, expected);
  // }
  //
  // for (auto [n, expected] : vector<pair<u64,u64>> {{149760653732457804, 2723}, {99013090109455081, 680},{1037362579310443,318},{99247603088339501,17749},{10503607679802991111,2190010}}) {
  //     dotest(n, expected);
  // }
  //
  // for(int i = 0; i < sample.size(); i++) {
  //   get_min_base(sample[i]);
  // }
}

int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now(), end;
    chrono::duration<double> elapsed;

    // Here's a simple bit of math: a positive integer n's representation
    // in base b is all 1s if and only if n*(b-1)+1 is a power of b
    // A079696  Numbers one more than composite numbers.
    // A002808  The composite numbers: numbers n of the form x*y for x > 1 and y > 1.

    test();




    end = chrono::steady_clock::now (), elapsed = end - alpha;
    std::cout << "\nDuration " << fixed << elapsed.count() << " ms" << std::endl;
}
