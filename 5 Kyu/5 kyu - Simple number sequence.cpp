#include <iostream>
#include <vector>

using namespace std;

int get_length (std::string src) {
  int size = std::min (static_cast<int>(src.size()) / 3 + 1, 7);

  while (size--> 1) {
      int ref = stoi(src.substr (0, size));
      int nxt1 = stoi (src.substr (size, size)), nxt2 = stoi (src.substr (size, size + 1));

      if (ref + 1 == nxt1 || ref + 2 == nxt1) return size;
      if (ref + 1 == nxt2 || ref + 2 == nxt2) return size;
  }
  return -1;
}
int missing (std::string s) {
    size_t i = 0, j = 0, sz = get_length (s), fst, next;
    std::vector<int> miss;

    while (i + j + sz < s.size()) {
        i += j, j = sz;
        fst = stoi(s.substr (i, sz)), next = stoi(s.substr (i + j, sz));

        while (next <= fst) {
            //sz++;
            next = stoi(s.substr (i + j, ++sz));
        }

        if (fst + 1 != next)
            miss.push_back (fst + 1);
    }

    return (miss.size() == 1) ? miss.back() : -1;
}

int main () {

  cout << missing ("9999859999869999899999909999919999929999939999949999959999969999979999989999991000000100000110000021000003") << endl;

  cout << missing ("99991000110002") << endl;
  cout << missing ("123567") << endl;
  cout << missing ("899091939495") << endl;
  cout << missing ("9899101102") << endl;
  cout << missing ("599600601602") << endl;
  cout << missing ("8990919395") << endl;
  cout << missing ("998999100010011003") << endl;
  cout << missing ("99991000110002") << endl;
  cout << missing ("979899100101102") << endl;
  /*
  */

    return 0;
}
