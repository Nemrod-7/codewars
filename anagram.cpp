#include <iostream>
// #include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

uint64_t factorial (int n) {
  uint64_t mult = 1;
  for (int i = 1; i <= n ; ++i)
      mult *= i;

  return mult;
}
int getindex (const string &word) {
    string name = word;
    sort(name.begin(), name.end());

    int index = 1;

    do {
        if (name == word) {
            // return index;
        }
        cout << name << " :: " << index <<  "\n";
        index++;
    } while (next_permutation (name.begin(), name.end()));

    return -1;
}

int permute (string &str) {

  do {
      // cout << str << '\n';

      int len = str.size();
      int key = len - 1, newkey = len - 1;
      /* The key value is the first value from the end which is smaller than the value to its immediate right        */
      while ((key > 0) && (str[key] <= str[key - 1]) ) {
          key--;
      }
      key--;
      /* If key < 0 the data is in reverse sorted order, which is the last permutation.                          */
      if (key < 0) break;
      /* str[key+1] is greater than str[key] because of how key was found. If no other is greater, str[key+1] is used   */
      newkey = len - 1;
      while( (newkey > key) && (str[newkey] <= str[key])) {
          newkey--;
      }
      swap (str[key], str[newkey]);
      /* variables len and key are used to walk through the tail,
      exchanging pairs from both ends of the tail.  len and
      key are reused to save memory                           */
      len--, key++;
      /* The tail must end in sorted order to produce the next permutation.                                       */
      while (len>key) {
          swap(str[len], str[key]);
          key++;
          len--;
      }

  } while (true);

   return 1;
}

void kth_permutation (int k, string str) {
    int n = str.size(), sel;
    sort(str.begin(), str.end());

    while (str.size()) {
        sel = (k - 1) / factorial (n - 1);
        cout << str[sel];
        k -= (sel * factorial (n-1));
        n --;
        str.erase(sel,1);
    }
}

int main () {

  chrono::steady_clock::time_point start = chrono::steady_clock::now(), end;
  chrono::duration<double> elapsed;

/*
ABAB = 2
AAAB = 1
BAAA = 4
QUESTION = 24572
BOOKKEEPER = 10743
*/
  string word = "2134";

  word = "312";
  // "IMMUNOELECTROPHORETICALLY";
  // STATIONARILY / ANTIROYALIST

  uint64_t last = factorial(word.size());
  string alpha = word, rever = word;

  sort(alpha.begin(), alpha.end());
  reverse(rever.begin(), rever.end());

  string temp = alpha;
  // cout << word << " " << alpha << "\n";
  int i = 0, sel;
  int n = word.size();
  // kth_permutation (24572, "QUESTION");
  int cnt = factorial (n - 1);

  ch = word[i];
  sel = alpha.find(ch);

  cout << (cnt / sel) + 1;

  end = chrono::steady_clock::now(), elapsed = end - start;
  std::cout << "\nDuration : " << fixed << elapsed.count() << " ms\n";
}
