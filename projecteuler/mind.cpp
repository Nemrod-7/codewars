#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;

void showvec (vector<int> pass) {
  for (int i = 0; i < pass.size(); i++) {
      cout << pass[i];
  }
  cout << "\n";
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

  random_device rd;  // Will be used to obtain a seed for the random number engine
  mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  geometric_distribution<> d;

  string pass;
  auto base = mkhist2 (clue,size);

  for (int i = 0; i < base.size(); i++) { /// first generation
    auto [prob,dig] = base[i].back();
    pass += dig;
  }

  int rate = eval (clue,pass);

  if (rate == clue.size()) {
    cout << "password : " << pass << "\n";
  } else {
    int minv = numeric_limits<int>::max();

    for (int i = 0; i < base.size(); i++) {
      auto [prob,dig] = base[i].back();
      minv = min (minv,prob);
    }

    vector<int> V;
    for (int i = 0; i < base.size(); i++) {
      auto [prob,dig] = base[i].back();
      if (prob == minv) {
        V.push_back(i);
      }
    }

    uniform_int_distribution<int> dist (0, V.size() - 1);
    int x = V[dist(gen)];
    int bak = pass[x], nxt;
    int nd = base[x].size() - 1;

    do {
      nxt = base[x][nd - d(gen)].second;
    } while (nxt == bak);

    pass[x] = nxt;

    cout << rate << " " << eval (clue,pass);
    // cout << pass << "\n";
  }

}
int main () {

      chrono::steady_clock::time_point alpha = chrono::steady_clock::now (), end;

      // Problem 185
      vector<pair<string,int>> clue;

      clue = {{"5616185650518293",2}, {"3847439647293047",1}, {"5855462940810587",3}, {"9742855507068353",3}, {"4296849643607543",3}, {"3174248439465858",1}, {"4513559094146117",2}, {"7890971548908067",3},
      {"8157356344118483",1}, {"2615250744386899",2}, {"8690095851526254",3}, {"6375711915077050",1}, {"6913859173121360",1}, {"6442889055042768",2}, {"2321386104303845",0}, {"2326509471271448",2},
      {"5251583379644322",2}, {"1748270476758276",3}, {"4895722652190306",1}, {"3041631117224635",3}, {"1841236454324589",3}, {"2659862637316867",2}};

      clue = { {"70794",0}, {"90342",2}, {"51545",2}, {"39458",2}, {"34109",1}, {"12531",1}};

      int size = clue[0].first.size();
      string pass ;
      // auto base = mkhist (clue,size);
      auto base = mkhist2 (clue,size);

      for (int i = 0; i < base.size(); i++) {
          auto [prob,dig] = base[i].back();
          pass += dig;
      }

      for (int x = 0; x < size; x++) {

        int curr = eval (clue,pass);
        int maxv = curr;
        string tmp = pass, next = pass;

        for (int j = base[x].size() - 1; j >= 0; j--) {
          char dig = base[x][j].second;
          tmp[x] = dig;
          int rate = eval (clue,tmp);

          if (rate > maxv) {
            maxv = rate;
            next = tmp;
          }
          cout << curr << " " << rate << " " << tmp <<  "\n";
        }
        pass = next;
      }

      // pass[x]

      // simulated_annealing (clue);


/*
      vector<int> index;
      vector<pair<int,int>> hist;

      for (int i = 0; i < size; i++) {
          auto [prob,dig] = base[i].back();
          hist.push_back({prob,i});
      }

      sort(hist.begin(),hist.end());

      for (int i = 0; i < size; i++) {
          index.push_back(hist[i].second);
      }

      // auto hd = mkhist (clue,size);

      // backtrack (clue, hd, string (size,'0'), 0);

      // codebreaker (clue, base, index, pass, 0);

      /*
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
    std::chrono::duration<double> elapsed = end - alpha;
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
