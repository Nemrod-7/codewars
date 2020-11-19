#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
typedef string test_t;
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
const string region = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .";
const vector<int> comb = {1,2,5,4,3,0};

string encrypt(string input) {
    string os;
    const size_t end = input.size();

    for (size_t i = 0; i < end; ++i) {
        if (region.find(input[i]) == string::npos) throw exception ();

        short byte = region.find(input[i]), out = 0;
        // phase 1
        if (i + 1 < end) {
            short nxt = region.find(input[i + 1]);
            bool nx_bit = (nxt >> 5) & 1U, ac_bit = (byte >> 1) & 1U;
            //cout << nx_bit << " " << ac_bit << endl;
            if (nx_bit != ac_bit) {
                byte ^= 1 << 1;
                nxt ^= 1 << 5;
            }
            input[i + 1] = region[nxt];
        }
        // phase 2
        byte ^= 1 << 4;
        byte ^= 1 << 2;

        for (size_t x = 6; x-->0;)
            out ^= ((byte >> comb[x]) & 1U) << x;

        os += region[out];
    }

    return os;
}
string decrypt(string input) {

    const size_t end = input.size();
    string os;

    for (size_t i = 0; i < end; ++i) {
        if (region.find(input[i]) == string::npos) throw exception ();

        short in = region.find(input[i]), byte = 0;

        for (size_t x = 6; x-->0;)
            byte ^= ((in >> x) & 1U) << comb[x];

        byte ^= 1 << 4;
        byte ^= 1 << 2;

        if (i > 0) {
            short prv = region.find(input[i - 1]);
            bool ac_bit = (byte >> 5) & 1U, pv_bit = (prv >> 1) & 1U;

            if (pv_bit != ac_bit) {
                byte ^= 1 << 5;
                prv ^= 1 << 1;
            }
            os += region[prv];
        }
        if (i + 1 == end)
            os += region[byte];

        input[i] = region[byte];
    }

    return os;
}

int main () {

    auto start = std::chrono::high_resolution_clock::now();

    //encrypt ("B9");
    //cout << decrypt ("KyU");
    //vector<short> range(1);
    Test();


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}

void Test() {

          Assert::That(encrypt("A"), Equals("K"));
          Assert::That(encrypt("Abc"), Equals("KyU"));
          Assert::That(encrypt("B9"), Equals("rw"));

          Assert::That(decrypt("K"), Equals("A"));
          Assert::That(decrypt("KyU"), Equals("Abc"));
          Assert::That(decrypt("rw"), Equals("B9"));

          Assert::That(decrypt("jvLdRPdQXV8Rd5x"), Equals("This is a test."));
          Assert::That(decrypt("rfR9qRVMT8TUfeyXGXdrLUcT.dUmVd5xUNo1oRdZQ1dtUXs1QVmRL8RMVt9RHqRtU1Ps1LtPQXVdbpZ9Lfp1"), Equals("Do the kata Kobayashi Maru Test. Endless fun and excitement when finding a solution."));

          Assert::That(encrypt(""), Equals(""));
          Assert::That(encrypt(""), Equals(""));
          /*
          AssertThrows(std::exception, encrypt("A5#"));
          AssertThrows(std::exception, decrypt("A5#"));
          AssertThrows(std::exception, encrypt("A5!"));
          AssertThrows(std::exception, decrypt("A5!"));
      */
}
