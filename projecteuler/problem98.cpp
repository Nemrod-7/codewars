#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>
#include <cstdint>
#include <algorithm>

#include <chrono>

using namespace std;

void searchpair (string name, vector<int> &value) {
  int score ,cnt = 0;

  do {
      score = 0;
      for (auto &let : name) {
          score = score * 10 + value[let - 'A'];
      }
      double sq = sqrt(score);

      if (abs (sq - round(sq)) < 1e-8) {
          cout << name << " "  << sq << "\n";
      }
    } while (next_permutation(name.begin(), name.end()));
}
void search(map<string,vector<string>> dict) {

  string buffer = "CARE";
  ifstream iss ("files/p098_words.txt");

  while (getline(iss, buffer,',')) {
      string word = buffer.substr(1, buffer.size()-2);
      cout << word << " :: ";

      for (auto &[key,square] : dict) {
          if (word.size() == key.size()) {
              vector<int> numer = {0,0,0,0,0,0,0,0,0,0}, alpha = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
              bool flag = true;

              for (int j = 0; j < word.size(); j++) {
                  int alp = word[j] - 'A', dig = key[j] - '0';
                  alpha[alp]++, numer[dig]++;

                  if (alpha[alp] != numer[dig]) {
                      flag = false;
                      break;
                  }

              }

              if (flag == true) {
                cout << key << ' ';
              } else {

              }
          }
      }
      cout << endl;
  }
}

int main () {

  chrono::steady_clock::time_point alpha = chrono::steady_clock::now(), end;
  chrono::duration<double> elapsed;

    string buffer = "CARE";
    // map<char, int> hist = {{'C',1},{'A',2},{'R',9},{'E',6}};
    map<string,vector<string>> dict;


   uint64_t st = 3162278,
            nd = 10000000;

   uint64_t nu = st, sq;

   do {
        sq = nu * nu;
        string num = to_string(sq);
        string key = num;

        sort(key.begin(), key.end());
        dict[key].push_back(num);

        if (num.size() > 14) break;

   } while (nu++);

    ifstream iss ("files/p098_words.txt");
    string name;
    unsigned long long maxv = 0;

    while (getline(iss, buffer,',')) {
        string word = buffer.substr(1, buffer.size()-2);

        if (word.size() == 14) {
            //cout << word << endl;
            for (auto &[key,square] : dict) {
                if (word.size() == key.size()) {
                    vector<int> numer = {0,0,0,0,0,0,0,0,0,0}, alpha = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                    bool flag = true;

                    for (int j = 0; j < word.size(); j++) {
                        int alp = word[j] - 'A', dig = key[j] - '0';
                        alpha[alp]++, numer[dig]++;

                        if (alpha[alp] != numer[dig]) {
                            flag = false;
                            break;
                        }
                    }

                    if (flag == true) {
                        for (auto &num : square) {
                            maxv = max(maxv, stoull(num));
                        }
                    } else {

                    }

                }
            }
        }
    }


    cout << maxv << '\n';
    //cout << name;
    end = chrono::steady_clock::now (), elapsed = end - alpha;
    std::cout << "\nDuration " <<fixed<< elapsed.count() << " ms" << std::endl;
}
