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

int costpt (vector<int> &board, pair<int,int> a) {
    int cnt = 0;
    pair<int,int> b;

    for (int i = 0; i < board.size(); i++) {
        b = {i, board[i]};
        //Display::point (b);
        if (a != b && attack (a, b)) cnt++;
    }

    return cnt;
}

int rnd_walk (vector<int> hist, int val) {
    vector<int> V;

    for (int i = 0; i < hist.size(); i++)
        if (hist[i] == val)
            V.push_back (i);

    uniform_int_distribution<> dist (0, V.size() - 1);

    return V[dist(gen)];
}
vector<int> min_conflict3 (vector<int> track, int pos) {
    int x, y;
    int max_iter  = track.size() * 50;
    vector<int> hist (track.size());

    while (max_iter-->0) {

        int cnt, sum = 0, val = 0;
        pair<int,int> a, b;

        for (int i = 0; i < track.size(); i++) {
            a = {i, track[i]};

            cnt = costpt (track, a);
            hist[i] = cnt;

            val = max (val, cnt);
            sum += cnt;
        }

        if (sum == 0) return track;

        x = rnd_walk (hist, val);
        //cout << "->" << x <<  "\n";
        val = track.size();
        for (int y = 0; y < track.size(); y++) {
            cnt = 0;
            a = {x, y};

            for (int i = 0; i < track.size(); i++) {
                b = {i, track[i]};

                if (a != b && attack (a, b)) cnt++;
            }

            if (costpt (track, a) != cnt) {
            //    cout << x << " " << y << endl;
                //cout << costpt (track, a) << " " << cnt << endl;
            }

            val = min (val, cnt);
            hist[y] = cnt;
        }
        //cout << costpt (track, a) << " " << cnt << endl;;
        y = rnd_walk (hist, val);

        track[x] = y;

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

        res = min_conflict3 (track, pos.first);

        if (res.size() != 0) return format(res);
    //}

    return "";
}

int main () {

    int index = 100, cnt = 0;
    string res;
    //Nqueens (8, {0,1});

    while (index-->0) {
        if (Nqueens (8, {0,1}) == "")
            cnt++;
    }
    cout << cnt;
    /*
    /*
    */
    //scan_col (0, track);
    //Display::board (track);
}
