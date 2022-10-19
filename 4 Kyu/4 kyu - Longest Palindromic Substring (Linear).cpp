#include<iostream>
#include <vector>
#include<string>

using namespace std;

string split (string str) {
  string os;
  const int size = str.size();
  for (int i = 0; i < size; i++) {
      os += str[i];
      if (i < size - 1) os += '|';
  }
  return os;
}
string join (string str) {
  string os;
  for (auto &it : str) {
      if (it != '|')
          os += it;
  }
  return os;
}

string longest_palindrome (const string &input) {

    string str = split (input);
    const size_t size = str.size();
    size_t maxv = 0, ax = 0;
    vector<string> base;

    while (maxv < size - ax) {

        int beg = ax - 1, end = ax + 1;

        while (beg >= 0 && end < size) {
            if (str[beg] != str[end]) break;
            beg--, end++;
        }

        string sub = join (str.substr (beg + 1, end - beg - 1));
        if (sub.size() > maxv) base.push_back(sub);
        maxv = max (sub.size(), maxv);

        ax++;
    }

    for (const auto &it : base)
        if (it.size() == maxv) return it;

    return "";
}

int main () {

    string str = "ababbab";
    str = split (str);
    const size_t size = str.size();
    size_t maxv = 0, ax = 0;
    vector<string> base;

    while (maxv < size - ax) {

        int beg = ax - 1, end = ax + 1;

        while (beg >= 0 && end < size) {
            if (str[beg] != str[end]) break;
            beg--, end++;
        }

        string sub = join (str.substr (beg + 1, end - beg - 1));
        cout << sub << "\n";
        if (sub.size() > maxv) base.push_back(sub);
        maxv = max (sub.size(), maxv);

        ax++;
    }


}
