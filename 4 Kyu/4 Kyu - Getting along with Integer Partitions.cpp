#include <iostream>
#include <vector>
#include <algorithm>

#include <string>
#include <iomanip>
#include <sstream>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
typedef string test_t;
class Assert {
    public :
        static void That (test_t actual, test_t expression) {
            if (actual != expression) {
                cout << "actual : " << actual;
                //for (auto it : actual)
                //    cout << it;

                cout << " expected : " << expression;
                //for (auto it : expression)
                //    cout << it;
                ///cout << endl;
            }
        }
};
test_t Equals (test_t entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////

class IntPart {
  public:
      static std::string part(long long n);
};

struct stat {
    int range;
    double mean;
    double median;
};

struct stat compute (vector<long long> arr) {

    struct stat actual = {0};
    int next = 0, size = arr.size(), data[size];

    sort (arr.begin(), arr.end());

    for (int i = 0; i < size; ++i)
        if (arr[i] != arr[i+1])
            actual.mean += data[next++] = arr[i];

    actual.median = (next % 2 == 0) ? ((data[next/2] + data[next/2 - 1]) / 2.0) : data[next/2];
    actual.range = data[next - 1] - 1;
    actual.mean /= next;

    return actual;
}

long long mult (vector<long long> arr, int size) {

    int prod = 1;
    for (int i = 0; i < size; ++i)
        prod *= arr[i];

    return prod;
}
string IntPart::part (long long num) {

    ostringstream os;
    stat out;
    vector<long long> cluster(num), prod;
    int index = 0, digit, rem = 0 , stack = 0, next = 0;

    cluster[index] = num + 1;

    while (cluster[0] > 1) {

        digit = cluster[index] - 1;
        rem = stack, next = index;

        while (rem < num)
            if (num - rem >= digit) {
                if (digit > 1) {
                    index = next;
                    stack = rem;
                }
                rem += cluster[next++] = digit;

            }
            else
                digit--;

        prod.push_back(mult (cluster,next));

        if (cluster[index] == 1) index--, stack -= cluster[index];
        ///if (cluster[0] == 1) break;
    }

    stat act = compute (prod);
    os << "Range: " << act.range;
    os << " Average: "<<fixed<<setprecision(2) << act.mean;
    os << " Median: " <<fixed<<setprecision(2) << act.median;

    return os.str();
}

int main () {

    cout << IntPart::part(5) << endl;
}
void testequal(std::string ans, std::string sol) {
    Assert::That(ans, Equals(sol));
}
void Test () {

    testequal(IntPart::part(1), "Range: 0 Average: 1.00 Median: 1.00");
    testequal(IntPart::part(2), "Range: 1 Average: 1.50 Median: 1.50");
    testequal(IntPart::part(3), "Range: 2 Average: 2.00 Median: 2.00");
    testequal(IntPart::part(4), "Range: 3 Average: 2.50 Median: 2.50");
    testequal(IntPart::part(5), "Range: 5 Average: 3.50 Median: 3.50");

}
