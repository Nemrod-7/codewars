#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

class Display {
    public :
        static void board (vector<int> &board) {

            for (int y = 0; y < board.size(); y++) {
                for (int x = 0; x < board.size(); x++) {
                    if (board[x] == y)
                        cout << "[x]";
                    else
                        cout << "[ ]";
                }
                cout << endl;
            }
            cout << endl;
        }
        static void point (const pair<int,int> &p) {
            cout << "[" << p.first << "," << p.second << "]\n";
        }
};

random_device rd;  //Will be used to obtain a seed for the random number engine
mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

bool attack (pair<int,int> a, pair<int,int> b) {

  return (a.first == b.first)
  || (a.second == b.second)
  || (a.first + a.second == b.first + b.second)
  || (a.first - a.second == b.first - b.second);
}

bool csp (vector<int> &track) {

    for (int i = 0; i < track.size(); i++) {
        pair<int,int> a = {i, track[i]};

        for (int j = 0; j < track.size(); j++) {
            pair<int,int> b = {j, track[j]};

            if (a != b && attack (a, b))
                return false;
        }
    }

    return true;
}

int rand_walk (int low, int high) {
    uniform_int_distribution<> dist (low, high);

    return dist(gen);
  }
int search_col (vector<int> &track) { // search column with max conflict
    int cnt;
    pair<int,int> a, b;
    vector<pair<int,int>> hist (track.size()); // first = count, second = column

    for (int i = 0; i < track.size(); i++) {
        a = {i, track[i]};
        cnt = 0;

        for (int j = 0; j < track.size(); j++) {
            b = {j, track[j]};

            if (a != b && attack (a, b))
                cnt++;
        }
        //cout << setw (2) << cnt << " ";
        hist[i] = make_pair (cnt, i);
    }
    /////// random walk => search highest malue //////
    sort (begin(hist), end(hist));
    int maxv = hist.back().first, next = (hist.end() - 1)->first;

    if (maxv != next) return hist.back().second; // if single high value retturn index

    int high = hist.size() - 1, low = high; // make bounds for rnd device generation

    while (true) {
        if (hist[low].first != maxv) break;
        low--;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist (low + 1, high);

    return hist[dist(rd)].second;

}
int scan_col (int x, vector<int> &track) { // check next y with min conflict
    int cnt, index;
    pair<int,int> a, b;

    vector<pair<int,int>> hist (track.size()); // first = count, second = column

    for (int y = 0; y < track.size(); y++) {
        cnt = 0;
        a = {x, y};

        for (int i = 0; i < track.size(); i++) {
            b = {i, track[i]};

            if (attack (a, b)) cnt++;
        }
        hist[y] = make_pair (cnt, y);
        //cout << setw (2) << cnt << "\n";
    }
    /////// random walk => search min malue //////
    sort (begin(hist), end(hist));
    int minv = hist.front().first, nxt = next (begin(hist))->first;
    if (minv != nxt) return hist.front().second; // if single lowest value return index

    int low = 0, high = low; // make bounds for rnd device generation

    while (true) {
        if (hist[high].first != minv) break;
        high++;
    }
    //cout << hist[rand_walk (low, high - 1)].second;
    return hist[rand_walk (low, high - 1)].second;
}

string format (vector<int> &board) {
    string os;

    for (int y = 0; y < board.size(); y++) {
        for (int x = 0; x < board.size(); x++) {
            os += (board[x] == y) ? 'Q' : '.';
        }
        os += '\n';
    }
    return os;
}

string Nqueens (int N, pair<int,int> pos) {

    if (N == 1) return "Q";
    if (N <= 3) return "";

    int x,y, sum;
    int max_iter = 50;

    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<> dist (0, N - 1);

    vector<int> track2 {7,4,3,4,0,1,0,5};
    /*
    vector<int> track2 (N);
    track2[pos.first] = pos.second;

    for (int x = 0; x < N; x++) {
        if (x != pos.first)
            track2[x] = dist(gen);
    }
    */
    while (max_iter-->0) {
        sum = 0;
        if (csp (track2) == true) return format (track2);
        //Display::board (track2);
        x = search_col (track2);
        //cout << "->" << x <<  "\n";
        y = scan_col (x, track2);
        //x = dist(gen);
        track2[x] = y;

    }
    /*

    vector<vector<int>> board (N, vector<int>(N));

    for (int x = 0; x < N; x++) {
        y = track2[x];
        increment (board, {x,y});
    }
    /*

      */

    return "";
}

int main () {

    cout << Nqueens (8, {0,7});
    vector<int> track {1,3,0,2};
    //scan_col (0, track);
    //Display::board (track);
}

bool isfree2 (vector<pair<int,int>> &track, pair<int,int> p) {

  for (auto &it : track) {
    if (attack (it, p))
    return false;
  }

  return true;
}

int countfree (int N, vector<pair<int,int>> &track, pair<int,int> p) {
  int cnt = 0;
  vector<pair<int,int>> tmp = track;
  tmp.push_back (p);

  for (int y = 0; y < N; y++)
  for (int x = 0; x < N; x++)
  if (isfree2 (tmp, {x,y})) cnt++;

  return cnt;
}

bool isinside (vector<vector<int>> &board, pair<int,int> p) {
  return p.first >= 0 && p.second >= 0 && p.first < board.size() && p.second < board.size();
}
bool isfree (vector<vector<int>> &board, pair<int,int> p) {
    pair<int,int> bl, fl;
    vector<int> dir (4);

    if (p.first - p.second > 0) {
        bl.first = p.first - p.second;
    } else {
        bl.second = p.second - p.first;
    }

    fl.first = (p.first + p.second);

    for (int i = 0; i < board.size(); i++) {
        if (board[p.second][i]) dir[0]++; // check line
        if (board[i][p.first]) dir[1]++; // checl col
        if (isinside (board, fl) && board[fl.second][fl.first]) dir[2]++;  // diag 1
        if (isinside (board, bl) && board[bl.second][bl.first]) dir[3]++; // diag 2

        fl.first--, fl.second++;
        bl.second++, bl.first++;
    }

    for (auto &it : dir)
        if (it != 0) return false;

    return true;
}
