#include <iostream>
#include <sstream>
#include <map>

using namespace std;

map<string,string> MORSE_CODE =
{{".-", "A"},{"-...", "B"},{"-.-.", "C"},{"-..", "D"},{".", "E"},{"..-.", "F"},
{"--.", "G"},{"....", "H"},{"..", "I"},{".---", "J"},{"-.-", "K"},{".-..", "L"},{"--", "M"},
{"-.", "N"},{"---", "O"},{".--.", "P"},{"--.-", "Q"},{".-.", "R"},{"...", "S"},{"-", "T"},
{"..-", "U"},{"...-", "V"},{".--", "W"},{"-..-", "X"},{"-.--", "Y"},{"--..", "Z"},{"-----", "0"},
{".----", "1"},{"..---", "2"},{"...--", "3"},{"....-", "4"},{".....", "5"},{"-....", "6"},
{"--...", "7"},{"---..", "8"},{"----.", "9"}};

string clean (string src) {
    int first = src.find_first_of ('1');
    int last = src.find_last_of('1') + 1;
    return src.substr(first, last - first);
}
int skip (auto &it) {
  int ref = *it, size = 0;
  while (*it == ref) {
      size++, it++;
  }
  return size;
}
int unit (const string &src) {
    int i = 0, bit, minval = 999;
    string signal = src;
    string::iterator it = signal.begin();

    while (it != signal.end())
        minval = min (skip (it), minval);

    return minval;
};
string decodeMorse (string morseCode) {

    istringstream iss (morseCode);
    string tok, os;

    while (iss >> tok) {
        os += MORSE_CODE[tok];
        if (iss.get() == ' ' && iss.get() == ' ')
            os += ' ';
        else
            iss.unget();
    }

    os.erase(os.find_last_not_of(' ') + 1);
    return os;
}
string decodeBitsAdvanced (const string &bits) {
      string code = clean (bits), morse;
      string::iterator it = code.begin();

      const int dot = unit (code), dash = dot * 3;
      int i = 0, bit;

      while (it != code.end()) {
          bit = *it;
          int size = skip (it);

          if (bit == '1') {
              if (size == dot) morse += '.';
              if (size == dash) morse += '-';
          }

          if (bit == '0') {
              if (size == dash) morse += ' ';
              if (size > dash) morse += "  ";
          }
      };

      return decodeMorse (morse);
}

int main () {

    string morse = "1100110011001100000011000000111111001100111111001111110000000000000011001111110011111100111111000000110011001111110000001111110011001100000011";


    //while (it != morse.end()) skip (it);
    //cout << decodeBitsAdvanced (morse);
    //cout << decodeBitsAdvanced ("1110000000111"); // E
    //cout << decodeBitsAdvanced ("111"); // E
    // cout << decodeBitsAdvanced ("000000011100000"); // E
    //cout << decodeBitsAdvanced ("000111000101010100010000000");


}
