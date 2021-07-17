#include <iostream>
#include <vector>

using namespace std;

int get_length (string src) {
  int size = src.size();
  int sz = min (size / 3 + 1, 7), i = 0;

  while (sz--> 1) {
      int ref = stoi(src.substr (i, sz));
      int nxt1 = stoi (src.substr (i + sz, sz)), nxt2 = stoi (src.substr (i + sz, sz + 1));

      if (ref + 1 == nxt1 || ref + 2 == nxt1) return sz;
      if (ref + 1 == nxt2 || ref + 2 == nxt2) return sz;
  }
  //cout << stoi(src.substr (i, sz)) << ' ' << stoi (src.substr (i + sz, sz)) << endl;
  return -1;
}
int missing (std::string s) {
    int i = 0, j = 0;
    int sz = get_length (s), fst;// = stoi(s.substr (0, sz));
    vector<int> range, miss;

    range.push_back (fst);

    while (i + j + sz < s.size()) {
        i += j, j = sz;
        fst = stoi(s.substr (i, sz));

        while (stoi(s.substr (i + j, sz)) <= fst)
            sz++;
        int next = stoi(s.substr (i + j, sz));

        if (fst + 1 != next) {
            miss.push_back (fst + 1);
        }
        //cout << fst << endl;
        range.push_back(next);
    }

    if (miss.size() == 1) return miss.back();
    return -1;
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
