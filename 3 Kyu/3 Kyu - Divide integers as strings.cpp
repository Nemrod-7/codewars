#include <iostream>
#include <vector>

using namespace std;

int substract (string &a, string &b, size_t pos) {

    size_t lena = a.size(), lenb = b.size();
    int rem = 0;

    if (lena >= lenb + pos) {
      string tmp = string (lenb, '0');
      int u = 0;

      for (size_t i = 0; i < lenb; ++i) {
          size_t pos2 = lenb - 1 - i;

          u += a[pos + pos2] - b[pos2];
          tmp[pos2] += u + (u < 0 ? 10 : 0);

          u = (u < 0 ? -1 : 0);
      }

      if (u == 0) {
          rem = 1;
      }
      else if (pos > 0) {
          if (a[pos - 1] > '0') {
              a[pos - 1]--;
              rem = 1;
          }
      }

      if (rem != 0)
          a.replace (pos, tmp.size(), tmp);
    }

    return rem;
}
vector<string> divide_strings (string a, string b) {

  size_t lena = a.size(), lenb = b.size ();
  string quot ("0"), den (a);

  for (size_t i = 0 ; lena >= lenb; ++i, --lena) {
      if (i > 0) quot += '0';
      char d = '*';

      while (quot[i] != d) {
          d = quot[i];
          quot[i] += substract (den, b, i);
      }
  }

  quot.erase(0, min (quot.find_first_not_of ('0'), quot.size() - 1));
  den.erase(0, min (den.find_first_not_of ('0'), den.size() - 1));
  //cout << quot << ' ' << den << endl;
  return {quot, den};
}

int main () {
    divide_strings("729", "9");
    //memcpy (a + pos, tmp, lb * sizeof (char));
}
