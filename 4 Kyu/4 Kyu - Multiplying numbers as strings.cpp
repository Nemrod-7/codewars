#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
class Assert {
    public :
        static void That (auto actual, auto expression) {
            if (actual != expression)
                std::cout<<"actual : "<<actual<<" expected : "<<expression<<std::endl;
        }
};
auto Equals (auto entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////

string multiply (string a, string b) {

    int lena = a.size(), lenb = b.size(), ans[1000]{0};

    if ((lena == 1 && a[0] == '0')|| (lenb == 1 && b[0] == '0'))
        return "0";

    int i,j,rem = 0, num, size;
    string output;

    reverse(a.begin(), a.end()), reverse(b.begin(), b.end());

    for (i = 0; i < lena; ++i) {
        for (j = 0; j < lenb; ++j) {
            ans[i + j] +=  (a[i] - '0') * (b[j] - '0');
            ans[i + j + 1] = ans[i + j + 1] + ans[i + j] / 10;
            ans[i + j] %= 10;
        }
    }

    size = i + j;
    while (ans[size] == 0) size--;

    do {
        output.push_back (ans[size] + '0');
    } while (size-->0);
    //reverse(output.begin(), output.end());

    //output.erase(0,  std::min (output.find_first_not_of('0'), output.size()-1));
    return output;
  }

int main () {

    auto start = chrono::high_resolution_clock::now();

    //Test();
    //cout << multiply ("1000","6") << endl;
    multiply("98765", "56894");
    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "Process took " << elapsed.count()  << " ms" << endl;

}

void Test() {
    Assert::That(multiply("2", "3"), Equals("6"));
    Assert::That(multiply("30", "69"), Equals("2070"));
    Assert::That(multiply("11", "85"), Equals("935"));
    Assert::That(multiply("2", "0"), Equals("0"));
    Assert::That(multiply("0", "30"), Equals("0"));
    Assert::That(multiply("0000001", "3"), Equals("3"));
    Assert::That(multiply("1009", "03"), Equals("3027"));
    Assert::That(multiply("98765", "56894"), Equals("5619135910"));
    Assert::That(multiply("1020303004875647366210", "2774537626200857473632627613"), Equals("2830869077153280552556547081187254342445169156730"));
    Assert::That(multiply("58608473622772837728372827", "7586374672263726736374"), Equals("444625839871840560024489175424316205566214109298"));
    Assert::That(multiply("9007199254740991", "9007199254740991"), Equals("81129638414606663681390495662081"));

}
