#include <iostream>

#include <string>
#include <map>

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
std::string encoder(const std::string& data) {

    int index = 0, pos = 1;
    string actual, os;
    map<string,int> dict;

    for (auto &it : data) {
        actual += it;

        if (dict[actual]) {
            index = max (index, dict[actual]);
        } else {
            dict[actual] = pos++;
            os += to_string (index) + it ;
            index = 0, actual.clear();
        }
    }

    if (index > 0)
        os += to_string (index);

    return os;
}
std::string decoder(const std::string& data) {
    int pos = 1;
    string output, temp;
    map<int,string> dict;

    for (auto &it : data) {

        if (isdigit(it))
            temp += it;
        else {
            output += dict[pos++] = dict[stoi (temp)] + it;
            temp.clear();
        }
    }

    if (temp.size() != 0)
        output += dict[stoi (temp)];

    return output;
}

int main () {

    auto start = std::chrono::steady_clock::now();

        /*
    */
    Test();

    //decoder("0A0B1A2A4A4B");
    //cout << encoder ("this is a test");




    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Process took " << elapsed.count()  << " ms" << std::endl;
}


void Test() {

  /*
  */
    Assert::That(encoder("ABAABABAABAB"), Equals("0A0B1A2A4A4B"));
    Assert::That(encoder("ABAABABAABABAA"), Equals("0A0B1A2A4A4B3"));
    Assert::That(encoder("ABBCBCABABCAABCAABBCAA"), Equals("0A0B2C3A2A4A6B6"));
    Assert::That(encoder("AAAAAAAAAAAAAAA"), Equals("0A1A2A3A4A"));
    Assert::That(encoder("ABCABCABCABCABCABC"), Equals("0A0B0C1B3A2C4C7A6"));
    Assert::That(encoder("ABCDDEFGABCDEDBBDEAAEDAEDCDABC"), Equals("0A0B0C0D4E0F0G1B3D0E4B2D10A1E4A10D9A2C"));
    Assert::That(decoder("0A0B1A2A4A4B"), Equals("ABAABABAABAB"));
    Assert::That(decoder("0A0B1A2A4A4B3"), Equals("ABAABABAABABAA"));
    Assert::That(decoder("0A0B2C3A2A4A6B6"), Equals("ABBCBCABABCAABCAABBCAA"));
    Assert::That(decoder("0A1A2A3A4A"), Equals("AAAAAAAAAAAAAAA"));
    Assert::That(decoder("0A0B0C1B3A2C4C7A6"), Equals("ABCABCABCABCABCABC"));
    Assert::That(decoder("0A0B0C0D4E0F0G1B3D0E4B2D10A1E4A10D9A2C"), Equals("ABCDDEFGABCDEDBBDEAAEDAEDCDABC"));
    /*
    */
}
