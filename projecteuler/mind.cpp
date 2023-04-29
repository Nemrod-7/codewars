#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <chrono>

using namespace std;

void showvec (vector<int> pass) {
  for (int i = 0; i < pass.size(); i++) {
      cout << pass[i];
  }
  cout << "\n";
}


vector<vector<char>> mkhist (vector<pair<string,int>> clue, int size) {
    vector<vector<char>> base (size);
    vector<vector<int>> hist (size, vector<int> (10,1));

    for (auto [code,res] : clue) {
        for (int i = 0; i < size; i++) {
          int dig = code[i] - '0';
          hist[i][dig]++;
        }
    }

    for (auto [code,res] : clue) {
        for (int i = 0; i < size; i++) {
          int dig = code[i] - '0';
          hist[i][dig] *= res;
        }
    }

    for (int i = 0; i < size; i++) {
        vector<pair<int,char>> prob;
        for (int j = 0; j < 10; j++) {
            if (hist[i][j] != 0)
                prob.push_back ({hist[i][j],j + '0'});
        }

        sort(prob.begin(),prob.end());

        for (int j = 0; j < prob.size(); j++) {
            base[i].push_back (prob[j].second);
        }
    }

    return base;
}
vector<vector<pair<int,char>>> mkhist2 (vector<pair<string,int>> clue, int size) {
    vector<vector<pair<int,char>>> base (size);
    vector<vector<int>> hist (size, vector<int> (10,1));

    for (auto [code,res] : clue) {
        for (int i = 0; i < size; i++) {
          int dig = code[i] - '0';
          hist[i][dig]++;
        }
    }

    for (auto [code,res] : clue) {
        for (int i = 0; i < size; i++) {
          int dig = code[i] - '0';
          hist[i][dig] *= res;
        }
    }

    for (int i = 0; i < size; i++) {
        vector<pair<int,char>> prob;
        // cout << i << " => ";
        for (int j = 0; j < 10; j++) {
            if (hist[i][j] != 0)
                prob.push_back ({hist[i][j],j + '0'});

            // cout << hist[i][j] << ' ';
        }
        // cout << "\n";
        sort(prob.begin(),prob.end());

        base[i] = prob;
    }

    return base;
}

int count (const string &a, const string &b) {
  int cnt = 0;
  for (int i = 0; i < a.size(); i++) {
    if (a[i] == b[i]) cnt++;
  }
  return cnt;
}

int eval (const vector<pair<string,int>> &clue, const string &pass) {
    int cnt = 0;
    for (auto &[code,res] : clue) {
        if (count (code,pass) == res) {
            cnt++;
        }
    }
    return cnt;
}
void simulated_annealing (vector<pair<string,int>> clue) {
  int size = clue[0].first.size();

  auto base = mkhist (clue,size);
  int nd = base[0].size() - 1;

  random_device rd;  // Will be used to obtain a seed for the random number engine
  mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  binomial_distribution<> d(nd, 0.9); // geometric_distribution<> d;  // nd - d(gen)
  uniform_real_distribution<> fx (0, 1);
  uniform_int_distribution<> pos (0, size - 1);
  string curr (size,'0');

  for (int i = 0; i < base.size(); i++) { // first gene
      curr[i] = base[i].back();
  }

  const float alpha = 0.99, t_min = 0.5;
  float T = 5000.0;
  cout << fixed;

  while (T > t_min) {
      // mutation
      int x = pos(gen), c0 = eval (clue,curr);
      string next = curr;

      if (c0 == clue.size()) {
          cout << "password : " << curr << "\n";
          break;
      }

      do {
          next[x] = base[x][d(gen)];
      } while (next[x] == curr[x]);

      int c1 = eval (clue,next);

      if (c1 > c0) {
          curr = next;
      } else {
          float dx = 1.0 - exp((c1 - c0) / T);
          // cout << T << " " << dx << " " << fx(gen) << "\n";
          if (dx < fx(gen)) {
              curr = next;
          }
     }

      T *= alpha;
  }


}

vector<string> rnd_walk (vector<pair<string,int>> &clue, vector<vector<pair<int,char>>> &base, string pass) {
    int size = pass.size();
    int maxv = eval (clue,pass);
    string tmp = pass;
    vector<string> vec;

    for (int x = 0; x < size; x++) {
      for (int j = 0; j < base[x].size(); j++) {
        tmp[x] = base[x][j].second;
        maxv = max (maxv,eval (clue,tmp));
      }
    }

    tmp = pass;
    for (int x = 0; x < size; x++) {
      for (int j = 0; j < base[x].size(); j++) {
        tmp[x] = base[x][j].second;
        if (eval (clue,tmp) == maxv) {
          vec.push_back(tmp);
        }
      }
    }
    return vec;
}

void simulated_annealing2 (vector<pair<string,int>> clue) {
  int size = clue[0].first.size();

  auto base = mkhist (clue,size);
  int nd = base[0].size() - 1;

  random_device rd;  // Will be used to obtain a seed for the random number engine
  mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  binomial_distribution<> d(nd, 0.9); // geometric_distribution<> d;  // nd - d(gen)
  uniform_real_distribution<> rng (0, 1);
  uniform_int_distribution<> pos (0, size - 1);
  string curr (size,'0');

  for (int i = 0; i < base.size(); i++) { // first gene
      curr[i] = base[i].back();
  }

  const float alpha = 0.999, t_min = 0.5;
  float T = 5.0;
  cout << fixed;

  while (T > t_min) {
      // mutation
      int x = pos(gen), c0 = eval (clue,curr);
      string next = curr;

      if (c0 == clue.size()) {
          cout << "password : " << curr << "\n";
          break;
      }

      do {
          next[x] = base[x][d(gen)];
      } while (next[x] == curr[x]);

      int c1 = eval (clue,next);

      if (c1 > c0) {
          curr = next;
      } else {
          float dx = exp((c1 - c0) / T);

          if (dx > rng(gen)) {
              curr = next;
          }
     }

      T *= alpha;
  }


}
int main () {

      chrono::steady_clock::time_point start = chrono::steady_clock::now (), end;

      // Problem 185
      vector<pair<string,int>> clue;

      clue = {{"5616185650518293",2}, {"3847439647293047",1}, {"5855462940810587",3}, {"9742855507068353",3}, {"4296849643607543",3}, {"3174248439465858",1}, {"4513559094146117",2}, {"7890971548908067",3},
      {"8157356344118483",1}, {"2615250744386899",2}, {"8690095851526254",3}, {"6375711915077050",1}, {"6913859173121360",1}, {"6442889055042768",2}, {"2321386104303845",0}, {"2326509471271448",2},
      {"5251583379644322",2}, {"1748270476758276",3}, {"4895722652190306",1}, {"3041631117224635",3}, {"1841236454324589",3}, {"2659862637316867",2}};

      clue = { {"70794",0}, {"90342",2}, {"51545",2}, {"39458",2}, {"34109",1}, {"12531",1}};
      cout << fixed;
      const float kmax = 600.0;


      float T = 5.0, alpha = 0.99, t_min = 1.0;

      while (T > t_min) {
          float delta = -1.0 ;
          float dx = exp(delta / T);
          printf ("%4.2f %4.0f %f\n", T, delta, dx);

          T *= alpha;
      }

      // simulated_annealing2 (clue);

      // codebreaker (clue, base, index, pass, 0);
      /*
      vector<float> temp {500,400,300,100,50,20};
      vector<float> dex {-1,-5,-10,-100,-200};

      for (auto T : temp) {
        for (auto delta : dex) {
          // float de = delta / T;
          float dx = exp(delta / T);

          // cout << T << " " << delta << " " << dx << "\n";
        }
      }


      V = {0,1,2,3,4,5,6,7,8,9};
      int hist[10] = {0};

      for (int i = 0; i < 20050; i++) {
        int dig = d(gen) % 10;
        hist[dig]++;
      }

      for (int i = 0; i < 10; i++) {
          cout << i << " => " << hist[i] << "\n";
      }

      */


    end = chrono::steady_clock::now ();
    std::chrono::duration<double> elapsed = end - start;
    cout << "\nDuration " << fixed << elapsed.count() << " ms" << endl;

}

void Test () {

  vector<pair<string,int>> clue = { {"70794",0}, {"90342",2}, {"51545",2}, {"39458",2}, {"34109",1}, {"12531",1}};

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
};

int count (const string &a, const string &b, int x) {
    int cnt = 0;
    for (int i = 0; i < x; i++) {
        if (a[i] == b[i]) cnt++;
    }
    return cnt;
}
bool check (const vector<pair<string,int>> &clue, const string &pass, int x) {
    for (auto &[code,res] : clue) {
      if (count (code,pass,x) > res) return false;
    }
    return true;
}
bool backtrack (const vector<pair<string,int>> &clue, const vector<vector<char>> &base, string pass, int x) {

    if (x == pass.size()) {
        for (auto &[code,res] : clue) {
          if (count (code,pass,x) < res) return false;
        }
        cout << "password : " << pass << "\n";
        return true;
    }
    for (int i = base[x].size() - 1; i >= 0; i--) {
        pass[x] = base[x][i];

        if (check (clue, pass, x) && backtrack (clue, base, pass, x + 1)) {
            return true;
        } else {

          // cout << x << " ";

        }
    }

    return false;
}

bool codebreaker (const vector<pair<string,int>> &clue, const vector<vector<pair<int,char>>> &base, const vector<int> &index, string pass, int i) {

    bool valid = true;
    for (auto &[code,res] : clue) {
      int cnt = count (code,pass);

      if (cnt != res) {
        valid = false;
        break;
      } else {
          // cout << pass << " " << code << " " << res <<  "\n";
      }
    }
    if (valid == true) {
        cout << "password : " << pass << "\n";
        return true;
    } else {
        if (i == index.size()) return false;
        int x = index[i];
        // int bak = pass[x];
        cout<< x << " ";

        for (int j = base[x].size() - 1; j >= 0; j--) {
            pass[x] = base[x][j].second;

            if (codebreaker (clue, base, index, pass, i + 1) == true) {
                return true;
            } else {
                // return false;
                // pass[x] = bak;
            }

        }
        /*
        */
    }

    return false;
}
