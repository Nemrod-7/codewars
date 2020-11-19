#include <iostream>
#include <vector>
#include <queue>
#include <chrono>

/////////////////////////////////Assert/////////////////////////////////////////
typedef int test_t;
class Assert {
    public :
        static void That (test_t actual, test_t expression) {
            if (actual != expression)
                std::cout<<"actual : "<<actual<<" expected : "<<expression<<std::endl;
        }
};
test_t Equals (test_t entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////

class DoubleLinear {
    public:
        static int dblLinear(int n) {

        std::vector<int> u(1);
        int next = 1, x, y, z, a = 0, b = 0;

        for (x = 0; x <= n; ++x) {
            y = 2 * u[a] + 1;
            z = 3 * u[b] + 1;

            if (y <= z) {
                u.push_back(y);
                a++;

                if (y == z)
                  b++;

            } else {
                u.push_back(z);
                b++;
            }
        }
        return u[n + 1];
    }

};

int twiceLinear(int n) {

        int x, y, z;
        std::priority_queue <int, std::vector<int>, std::greater<int>> u;

        u.push(1);

        while (n-->0) {
            x = u.top();
            u.pop();

            if (!u.empty() && u.top() == x) u.pop();

            y = 2 * x + 1;
            z = 3 * x + 1;

            u.push (y), u.push (z);
        }

    return u.top();
}
int main () {

  auto start = std::chrono::high_resolution_clock::now();


  Test();

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Process took " << elapsed.count()  << " ms" << std::endl;

    return 0;
}

void testequal(int ans, int sol) {
    Assert::That(ans, Equals(sol));
}
void dotest(int n, int expected) {
    testequal(DoubleLinear::dblLinear(n), expected);
    testequal(twiceLinear(n), expected);
}

void Test () {
        dotest(10, 22);
        dotest(20, 57);
        dotest(30, 91);
        dotest(50, 175);
}
