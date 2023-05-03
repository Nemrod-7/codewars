#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>

using namespace std;

int hamming (const string &a, const string &b) {
    int cnt = 0;
    for (int i = 0; i < a.size(); i++) {
        if (a[i] == b[i]) cnt++;
    }
    return cnt;
}
int cost (const vector<pair<string,int>> &clue, const string &pass) {
    int errors = 0;

    for (auto &[code,res] : clue) {
        int r1 = hamming (code, pass);
        errors += abs(r1 - res);
    }

    return errors;
}
string simulated_annealing (vector<pair<string,int>> clue) {

    int size = clue[0].first.size();
    string next, curr  (size,'0');// generate(clue);

    random_device rd; 
    mt19937 gen(rd()); 
    uniform_real_distribution<> rng (0, 1);
    uniform_int_distribution<> pos (0, size - 1), dig (0, 9);

    const double alpha = 1.0 - 1e-7, t_min = 0.01;
    double T = 0.4;

    while (T > t_min) {
        // mutation
        int x = pos(gen), c0 = cost (clue,curr);
        next = curr;

        if (c0 == 0) {
            cout << "password : " << curr << "\n";
            return curr;
        }

        do {
            next[x] = dig(gen) + '0'; // next[x] = base[x][dig(gen)];
        } while (next[x] == curr[x]);

        int c1 = cost (clue,next);

        if (c1 < c0) {
            curr = next;
        } else if (exp((c0 - c1) / T) > rng(gen)) {
            curr = next;
        }

        T *= alpha;
    }
    // cout << maxv;
    return "";
}
int main () {

    chrono::steady_clock::time_point start = chrono::steady_clock::now (), end;

    // Problem 185
    vector<pair<string,int>> clue  = { {"5616185650518293",2}, {"3847439647293047",1}, {"5855462940810587",3}, {"9742855507068353",3}, {"4296849643607543",3}, {"3174248439465858",1},
        {"4513559094146117",2}, {"7890971548908067",3}, {"8157356344118483",1}, {"2615250744386899",2}, {"8690095851526254",3}, {"6375711915077050",1}, {"6913859173121360",1},
        {"6442889055042768",2}, {"2321386104303845",0}, {"2326509471271448",2}, {"5251583379644322",2}, {"1748270476758276",3}, {"4895722652190306",1}, {"3041631117224635",3},
        {"1841236454324589",3}, {"2659862637316867",2}};

    // clue = { {"70794",0}, {"90342",2}, {"51545",2}, {"39458",2}, {"34109",1}, {"12531",1}};
    simulated_annealing (clue);

    // password : 4640261571849533


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

