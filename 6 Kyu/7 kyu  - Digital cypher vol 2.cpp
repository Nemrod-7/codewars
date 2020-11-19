#include <iostream>

using namespace std;
class Kata {
public:
  static string Decode(vector<int> code, int n) {
      string dcryted, key = to_string (n);
      size_t next = 0;
      for (auto &c : code) {
          if (next == key.size()) next = 0;
          dcryted += ((c + 'a' - 1) - (key[next++] - '0'));
      }
       return dcryted;
  }
};
