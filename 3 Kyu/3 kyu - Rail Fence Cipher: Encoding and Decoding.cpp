#include <iostream>
#include <vector>
#include <string>
#include <queue>

using namespace std;

class Assert {
    public:
        static void That (string a, string b) {
          if (a != b)
              cout<<"actual : "<<a<<" expected : "<<b<<endl;
    }
};
string Equals (string in) { return in;}

std::string encode_rail_fence_cipher(const std::string &str, int n) {

    std::string os;
    const int h = (n - 1) * 2;

    for (int i = 0; i < n; ++i) {
        for (size_t j = i, par = 0; j < str.size(); par++) {
            os += str[j];

            if (i == 0 || i == n - 1)
                j += h;
            else
                j += (par % 2 == 0 ) ? (h - (i * 2)) : (i * 2);
        }
    }

    return os;
}
std::string decode_rail_fence_cipher(const std::string &str, int n) {
    std::string os ("",str.size());

    const int h = (n - 1) * 2;
    int next = 0;

    for (int i = 0; i < n; ++i) {
        for (size_t j = i, par = 0; j < str.size(); par++) {
            os[j] = str[next++];
            if (i == 0 || i == n - 1)
                j += h;
            else
                j += (par % 2 == 0 ) ? (h - (i * 2)) : (i * 2);
        }
    }
    return os;
}

void Test () {

    Assert::That(encode_rail_fence_cipher("WEAREDISCOVEREDFLEEATONCE", 3), Equals("WECRLTEERDSOEEFEAOCAIVDEN"));
    Assert::That(encode_rail_fence_cipher("Hello, World!", 3), Equals("Hoo!el,Wrdl l"));
    Assert::That(encode_rail_fence_cipher("Hello, World!", 4), Equals("H !e,Wdloollr"));
    Assert::That(encode_rail_fence_cipher("", 3), Equals(""));

    Assert::That(decode_rail_fence_cipher("H !e,Wdloollr", 4), Equals("Hello, World!"));
    Assert::That(decode_rail_fence_cipher("WECRLTEERDSOEEFEAOCAIVDEN", 3), Equals("WEAREDISCOVEREDFLEEATONCE"));
    Assert::That(decode_rail_fence_cipher("", 3), Equals(""));
}

int main () {
    cout << decode_rail_fence_cipher("H !e,Wdloollr", 4);
    return 0;
}
