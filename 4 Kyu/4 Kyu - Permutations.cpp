#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

vector<string> permutations(string s);

int main () {

    //vector<string> actual = permutations("ktgug"); // {"aabb", "abab", "abba", "baab", "baba", "bbaa"}
    vector<string> actual = permutations("bac");
    cout << actual.size() << endl;

    for (int i = 0; i < actual.size(); ++i)
        cout << actual[i] << endl;

    return 0;
}

vector<string> permutations(string s) {

    vector<string> actual;
    sort(s.begin(), s.end());
    //cout << "\narr : " <<  s << " ::size:: " << s.size() << endl;
    do { //heapPermutation (s,s.length(), &actual);
        actual.push_back (s);
    } while (next_permutation(s.begin(), s.end()));

    return actual;
}
/*
void heapPermutation (string a, int size, vector<string> *actual) {

  if (size == 1) {
    bool unique = true;

    for (int i = 0; i < actual->size(); ++i)
    if (actual->at(i).compare(a) == 0)
    unique = false;

    if (unique == true)
    actual->push_back (a);
  }
  for (int i = 0; i < size; i++) {
    heapPermutation(a, size - 1, actual);
    size % 2 == 1 ? swap (a[0], a[size-1]) : swap (a[i], a[size-1]);
  }
}
*/
