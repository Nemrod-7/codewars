#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <chrono>
#include <set>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
typedef uint32_t test_t;
class Assert {
    public :
        static void That (test_t actual, test_t expression) {
            if (actual != expression)
                cout<<"actual : "<<actual<<" expected : "<<expression<<endl;
        }
};
test_t Equals (test_t entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////

uint32_t n_linear(const std::set<uint32_t>& m, size_t n) {

    int size = m.size(), i = 0;
    vector<uint32_t> N(m.begin(), m.end()), it(size), u{1};
    vector<queue<uint32_t>> Q(size);

    for (auto num : m)
        Q[i++].push(num + 1);

    while(n-->0) {
        uint32_t dist = numeric_limits<uint32_t>::max();
        for (i = 0; i < size; i++) {
            if (Q[i].front() < dist)
                dist = Q[i].front();
        }

        u.push_back(dist);
        for (i = 0; i < size; i++)
            if (Q[i].front() == dist) {
                Q[i].pop();
                it[i]++;
                Q[i].push(u[it[i]] * N[i] + 1);
            }

    }

    return u.back();
}

int main () {

  auto start = chrono::high_resolution_clock::now();

  set<int> n;
  //set<int>::iterator it = n.begin(), sec = ++it;

  Test();
  //n_linear({5,7,8},10);
  //n_linear({10,13},741);
  n_linear3({10,13},741);

  auto end = chrono::high_resolution_clock::now();
  chrono::duration<double> elapsed = end - start;
  cout << "Process took " << elapsed.count()  << " ms" << endl;

    return 0;
}

void Test () {

    Assert::That(n_linear3({2, 3}, 10), Equals(22));
    Assert::That(n_linear3({3, 2}, 10), Equals(22));
    Assert::That(n_linear3({5, 7, 8}, 10), Equals(64));
    Assert::That(n_linear3({5, 7, 8}, 11), Equals(65));

    Assert::That(n_linear3({10,13},741), Equals (3172655773));
    Assert::That(n_linear3({2, 3, 10000002}, 2000000), Equals (133028307));
    /*
    n : 2017107
    2 3 5 845135
    Expected: equal to 7723480
    Actual: 7556635
    */
}
