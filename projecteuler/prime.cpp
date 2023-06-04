#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class seq {
    private :
    public :
        vector<int64_t> rnd;
        seq () {
            for (int i = 0; i < 56; i++) {
                int64_t num = 100003 - 200003 * i + 300007 * pow (i,3);
                rnd.push_back(num);
            }
        }
        int64_t gen (int k) {
            int64_t num;
            if (k < 56) {
                num = rnd[k];
            } else {
                num = gen (k-24) + gen (k-55);
            }
            return num % 1000000;
        }
};

int main () {

    vector<pair<int64_t,int64_t>> graph;

    int64_t lim = 10000;

    seq now;

    for (int i = 1; i < 10000;i++) {
        int64_t a = now.gen(2 * i - 1), b = now.gen(2 * i);

      if (a == 200007 || b == 200007) {
        cout << i << " => " << a << " " << b << "\n";

      }
    }
    //cout << cnt;
    cout << "\n";

}
