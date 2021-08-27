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
                        cout << "[#]";
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

  return (a.first == b.first) ||
         (a.second == b.second) ||
         (a.first + a.second == b.first + b.second) ||
         (a.first - a.second == b.first - b.second);
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

vector<int> generate (int N, pair<int,int> pos) {
    vector<int> track (N);
    uniform_int_distribution<> dist (0, N - 1);

    for (int x = 0; x < N; x++) {
        if (x == pos.first)
            track[x] = pos.second;
        else
            track[x] = dist(gen);
    }
    return track;
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

int search_col (vector<int> &track) { // search column with max conflict
    int cnt, sum = 0;
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

        cout << setw (2) << cnt << " ";
        sum += cnt;
        hist[i] = make_pair (cnt, i);
    }

    //if (sum == 0) return -1;
    /////// random walk => search highest malue //////
    sort (begin(hist), end(hist));
    int maxv = hist.back().first, next = (hist.end() - 1)->first;

    if (maxv != next) return hist.back().second; // if single high value retturn index

    int high = hist.size() - 1, low = high; // make bounds for rnd device generation

    while (true) {
        if (hist[low].first != maxv) break;
        low--;
    }

    uniform_int_distribution<> dist (low + 1, high);

    return hist[dist(rd)].second;

}
int scan_col (int x, vector<int> &track) { // check next y with min conflict
    int cnt;
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

    uniform_int_distribution<> dist (low, high - 1);
    //cout << hist[rand_walk (low, high - 1)].second;
    return hist[dist(gen)].second;
}
vector<int> min_conflict (vector<int> track, int pos) {
    int x, y;
    int max_iter  = track.size() * 10;
    vector<int> xx, yy, prev;

    while (max_iter-->0) {

        if (csp (track) == true) {
            //cout << track.size() * 10 - max_iter << " ";
            return track;
        }

        x = search_col (track);

        cout << "->" << x <<  "\n";
        y = scan_col (x, track);
        track[x] = y;
        /*
        if (track == prev) local--;
        if (local == 0) return {};
        prev = track;
        */
    }
    //Display::board (track);

    return {};
}
string Nqueens (int N, pair<int,int> pos) {

    if (N == 1) return "Q";
    if (N <= 3) return "";

    int index = 1;
    vector<int> track = generate (N, pos), res;

    /*
    */
    //while (index-->0) {
    //    track = generate (N, pos);

        res = min_conflict (track, pos.first);

        if (res.size() != 0) return format(res);
    //}

    return "";
}



int main () {

    int index = 50, cnt = 0;
    string res;

    //cout << Nqueens (8, {0,1});
    cout << Nqueens (8, {0,1});
    /*
    while (index-->0) {
        if (Nqueens (100, {0,1}) == "")
            cnt++;
    }

    cout << cnt;
    /*
    */
    //scan_col (0, track);
    //Display::board (track);
}
