#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <chrono>
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

vector<int> generate (int N, pair<int,int> pos) {
    vector<int> track (N);
    uniform_int_distribution<> dist (0, N - 1);

    while (N-->0)
        track[N] = (N == pos.first) ? pos.second : dist(gen);

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

bool attack (const pair<int,int> &a, const pair<int,int> &b) {

  return (a.second == b.second) ||
         (a.first + a.second == b.first + b.second) ||
         (a.first - a.second == b.first - b.second);
}
int rnd_walk (const vector<int> &hist, int val) {
    vector<int> V;

    for (int i = 0; i < hist.size(); i++)
        if (hist[i] == val)
            V.push_back (i);

    uniform_int_distribution<> dist (0, V.size() - 1);

    return V[dist(gen)];
}
vector<int> min_conflict3 (vector<int> track, int pos) {
    const int N = track.size();
    int max_iter  = track.size() * 50;

    int x, y;
    vector<int> hist (N);
    pair<int,int> a, b;

    while (max_iter-->0) {

        int cnt, sum = 0, val = 0;

        for (x = 0; x < N; x++) {

            if (x == pos) continue;
            a = make_pair (x, track[x]);
            //a = {x, track[x]};
            cnt = 0;

            for (int i = 0; i < N; i++) {
                //b = {i, track[i]};
                b = make_pair (i, track[i]);
                if ( a != b && attack (a, b)) cnt++;
            }

            hist[x] = cnt;

            val = max (val, cnt);
            sum += cnt;
        }

        if (sum == 0) return track;

        x = rnd_walk (hist, val);
        //cout << "->" << x <<  "\n";
        val = N;
        for (y = 0; y < N; y++) {
            a = {x, y};
            cnt = 0;

            for (int i = 0; i < N; i++) {
                b = {i, track[i]};

                if (attack (a, b)) cnt++;
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

int cost (vector<int> &board) {
    int cnt = 0, N = board.size();

    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) { // avoid counting twice

            if ((board[i] == board[j]) || (i + board[i] == j + board[j]) || (i - board[i] == j - board[j]))
                cnt++;

        }
    }
    return cnt;
}
double schedule (double t) {
    const double k = 20, lam = 0.005, limit = 30000;
    return t < limit ? (k * exp(-lam * t)) : 0;
}
vector<int> simulated_annealing (vector<int> board, int pos) {
    //double temp = 100, thresh = 0.0001, decay = 0.99;
    int x, y, curr, next ;
    int index = 1;
    double T, p;

    uniform_int_distribution<> dist (0, board.size() - 1);
    uniform_real_distribution<> rand (0, 1);

    while (index < 30000) {
        curr = cost (board);

        x = dist (gen), y = dist (gen);   // let neighbor be a random neighbor of solution
        if (x == pos) continue;
        swap (board[x], y);

        next = cost (board);

        if (next == 0) return board;

        if (next > curr) {  // if the cost of neighbor isn't less than the cost of solution
            //T = 100 / index;             // 0.12 ms
            T = schedule (index);      // 0.24 ms
            //p = exp (curr - next / T);   // compute p = e^(-delta / t)
            if (rand(gen) > p) {
                board[x] = y;
            }
        }
        /*
        */
        index++;
    }

    return {};
}

string solveNQueens (int N, pair<int,int> pos) {

    if (N == 1) return "Q";
    if (N <= 3) return "";

    int index = 1;
    vector<int> track, res;

    //Display::board (track);
    while (index-->0) {
        track = generate (N, pos);

        res = min_conflict3 (track, pos.first);

        if (res.size() != 0) return format(res);
    }

    return "";
}

int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now ();
    chrono::duration<double> elapsed;

    int index = 100, cnt = 0;
    string res;
    res = solveNQueens (400, {2,1});

    //while (index-->0) {
        if (res == "")
            cnt++;
    //}

    cout << cnt;

    chrono::steady_clock::time_point end = chrono::steady_clock::now ();
    elapsed = end - alpha;
    cout << "\nDuration " << fixed << elapsed.count() << " ms" << endl;
}

void Test () {

    /*

    std::string actual = nQueens::solveNQueens(4, {1, 1});
    std::string expected = "";
    Assert::That(actual, Equals(expected));

    actual = nQueens::solveNQueens(4, {2,0});
    expected = "..Q.\nQ...\n...Q\n.Q..\n";
    Assert::That(actual, Equals(expected));

    actual = nQueens::solveNQueens(6, {2,1});
    expected = "....Q.\n..Q...\nQ.....\n.....Q\n...Q..\n.Q....\n";
    Assert::That(actual, Equals(expected));

    */

}
