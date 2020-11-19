#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>

#include <chrono>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
typedef string test_t;
class Assert {
    public :
        static void That (test_t actual, test_t expression) {
            if (actual != expression) {
              std::cout<<"actual : "<<actual<<" expected : "<<expression;

              std::cout<<std::endl;

            }

        }
};
test_t Equals (test_t entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////
class Decomp {
    public:
        static std::string decompose(const std::string &nrStr, const std::string &drStr);
};

pair<int,int> sub (pair<int,int> &a, pair<int,int> b) {

    pair<int,int> out;
    out.first = a.first * b.second - a.second * b.first;
    out.second = a.second * b.second;
    return out;
}
void display (pair<int,int> actual) {
    cout << actual.first <<"/"<< actual.second << endl;
}

std::string format (std::vector<std::pair<uint64_t,uint64_t>> source) {
    std::ostringstream os;

    os << "[";
    for (auto &it : source) {
        os << it.first;
        if (it.second > 1) os <<"/" << it.second;
        if (it != source.back()) os << ", ";
    }
    os << "]";

    return os.str();
}
std::string Decomp::decompose(const std::string &nrStr, const std::string &drStr) {

    std::vector<std::pair<uint64_t,uint64_t>> decomp;
    std::pair<uint64_t,uint64_t> frac = {stoi (nrStr), stoi (drStr)} ,out, temp;
    std::string os;

    while (frac.first != 0) {

        if (frac.first < frac.second) {
            out.first = 1;
            out.second = (frac.second + frac.first - 1) / frac.first;
        } else {
            out.first = frac.first / frac.second;
            out.second = 1;
        }

        frac.first = out.second * frac.first - frac.second * out.first;
        frac.second = out.second * frac.second;

        os += to_string (out.first);

        if (out.second > 1)
          os += "/" + to_string(out.second);
        os +=  ", ";

    }

    os.resize(os.size() - 2);

    return "[" + os + "]";
}

int main () {
    auto start = std::chrono::steady_clock::now();

    Test();

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Process took " << elapsed.count()  << " ms" << std::endl;
}

void testequal(std::string ans, std::string sol) {
    Assert::That(ans, Equals(sol));
}
void Test() {

    testequal(Decomp::decompose("3", "4"), "[1/2, 1/4]");
    testequal(Decomp::decompose("12", "4"), "[3]");
    testequal(Decomp::decompose("4", "5"), "[1/2, 1/4, 1/20]");
    testequal(Decomp::decompose("66", "100"), "[1/2, 1/7, 1/59, 1/5163, 1/53307975]");
    testequal(Decomp::decompose("125", "100"), "[1, 1/4]");
    testequal(Decomp::decompose("45", "30"), "[1, 1/2]");
    testequal(Decomp::decompose("50", "4187"), "[1/84, 1/27055, 1/1359351420]");

}
