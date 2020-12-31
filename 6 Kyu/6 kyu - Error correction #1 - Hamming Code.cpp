#include <iostream>
#include <bitset>
#include <string>

using namespace std;

std::string encode(std::string src) {
    string os;

    for (auto &it : src) {
        auto byte = bitset<8>(static_cast<int> (it));

        for (auto &bit : byte.to_string())
            os += string(3, bit);

        //os += ' ';
    }
    //os.pop_back();
    //cout << os;
    return os;
}
std::string decode(std::string bits) {
    string text, byte;
    for (int i = 0; i < bits.size(); i += 3) {
        byte += bits[i];

        if (i % 24 == 21) {
            text += bitset<8>(byte).to_ulong();
            byte.clear();
        }
    }

    //cout << text;
    return text;
}

int main () {
  //  string src = "hey", code =  encode (src);

//decode (code);

    int num, next;

    for (int i = 0; i < 12; ++i) {
       num = i, next = 0;
       int byte[8] ={0};
       //out << num << " > " << (num >> 1) << ' ';


       do {
          cout << num << ' ';
          byte[next++] = (num &1U);
       } while (num >>= 1);

       //for (int j = next; j-->0;)
      //    cout << byte[j];
       cout << endl;
       //binary = 0;
    }
    return 0;
}
