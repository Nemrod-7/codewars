#include <iostream>
#include <vector>
#include <random>

using namespace std;

vector<vector<double>> mkhist (vector<pair<string,int>> guess, int size) {
    vector<vector<double>> hist (size, vector<double> (10));

    for (auto [code,res] : guess) {
        for (int i = 0; i < size; i++) {
          int dig = code[i] - '0';
          hist[i][dig]++;
        }
    }
    for (auto [code,res] : guess) {
        for (int i = 0; i < size; i++) {
          int dig = code[i] - '0';
          hist[i][dig] *= res;
        }
    }

    return hist;
}
int cost (vector<int> &pass, vector<vector<double>> &hist) {
    int rate = 0;
    for (int i = 0; i < pass.size(); i++) {
        rate += hist[i][pass[i]];
    }
    return rate;
}

void codebreaker (vector<pair<string,int>> guess) {
  int size = 5;
  vector<vector<double>> hist = mkhist (guess,size);

  random_device rd;  //Will be used to obtain a seed for the random number engine
  mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  uniform_int_distribution<> pos (0, size-1), dig (0,9);
  uniform_real_distribution<> rand (0, 1);

  double p;
  vector<int> pass (size);
  int maxv = 0;
  int x, y;

  for (int i = 0; i < 100; i++) {
    // int rate = cost (pass, hist);
    x = pos(gen);

    do {
        y = dig(gen);
    } while (y < pass[x]);
    // cout << pass[x] << " " << y << "\n";
    vector<int> next = pass;
    next[x] = y;
    double rate = cost (pass, hist) /  (double) cost (next, hist);
    cout << rate << " ";
    // if (P (E(next), E(curr)) >= rand(gen)) {}
    if (cost (next, hist) > maxv) {
      maxv = cost (next, hist);
      pass = next;
      for (int i = 0; i < size; i++) {
          cout << pass[i];
      }
      cout << "\n";
    }
  }

/*
  for (int i = 0; i < size; i++) {
      cout << i << " => ";

      for (int j = 0; j < 10; j++) {
          cout << hist[i][j] << " ";
      }

      cout << "\n";
  }
  */
}
int main () {

    /*
    0 => 1 1 1 2 1 2 1 0 1 2
    1 => 0 2 1 1 1 1 1 1 1 2
    2 => 1 1 1 2 2 2 1 0 1 1
    3 => 1 1 1 1 4 2 1 1 1 0
    4 => 1 1 2 1 0 2 1 1 2 1

    0 => 1 2 1 6 1 4 1 0 1 4 : 3
    1 => 0 4 2 1 2 1 1 1 1 4 : 19
    2 => 1 2 1 4 4 6 1 0 1 1 : 5
    3 => 2 1 1 2 9 4 1 1 1 0 : 4
    4 => 1 2 4 1 0 4 1 1 4 2 : 258

    */

    vector<pair<string,int>> guess = {
      {"70794",0},
      {"90342",2},
      {"51545",2},
      {"39458",2},
      {"34109",1},
      {"12531",1}};

      int size = 5;
      vector<vector<int>> hist (size, vector<int> (10, 1));

      for (auto [code,res] : guess) {
          for (int i = 0; i < size; i++) {
            int dig = code[i] - '0';
            hist[i][dig]++;
          }
      }

      for (auto [code,res] : guess) {
          for (int i = 0; i < size; i++) {
            int dig = code[i] - '0';
            hist[i][dig] *= res;
          }
      }

      for (int i = 0; i < size; i++) {
          cout << i << " => ";
          for (int j = 0; j < 10; j++) {
              cout << hist[i][j] << " ";
          }
          cout << "\n";
      }
    // codebreaker (guess);

    cout << "\nend";
}
