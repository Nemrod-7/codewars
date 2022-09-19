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

vector<int> generate2 (int N, pair<int,int> pos) {
    vector<int> track (N);

    track[pos.first] = pos.second;

    while (N-->0)
        if (N != pos.first)
            track[N] = (N != pos.second) ? N : pos.first;

    return track;
}
vector<int> generate3 (int N, pair<int,int> pos) {
    vector<int> board (N);

    while (N-->0) board[N] = N;

    shuffle (begin(board), end(board), gen);

    board[pos.first] = pos.second;

    return board;
}

int rnd_walk (const vector<int> &hist, int val) {
    vector<int> V;

    for (int i = 0; i < hist.size(); i++)
        if (hist[i] == val)
            V.push_back (i);

    uniform_int_distribution<int> dist (0, V.size() - 1);

    return V[dist(gen)];
}

class Qdata {
    private :
        int N;
    public :
        vector<int> rows, d1, d2;
        Qdata (int size) {
            N = size;
            rows.resize(N), d1.resize (2 * N - 1), d2.resize (2 * N - 1);
        }

        void reset (vector<int> board) {

            fill (begin(rows), end(rows), 0);
            fill (begin(d1), end(d1), 0);
            fill (begin(d2), end(d2), 0);

            for (int i = 0; i < N; i++) {
                rows[board[i]]++;
                d1[N + i - board[i] - 1]++;
                d2[i + board[i]]++;
            }
        }

        int attack (int x, int y) { return rows[y] + d1[N + x - y - 1] + d2[x + y]; }
};

vector<int> min_conflict4 (const int N, pair<int,int> pos) {

    int x, y;
    vector<int> board(N), hist (N);
    int max_iter  = N * 50;

    Qdata actual (N);
    board[pos.first] = pos.second;

    actual.reset (board);

    while (max_iter-->0) {

        int cnt, sum = 0, val = 0;

        for (x = 0; x < N; x++) {

            if (x == pos.first) {
                cnt = 0;
            } else {
                cnt = actual.attack (x, board[x]) - 3;
            }

            hist[x] = cnt;
            //cout << cnt << " ";
            val = max (val, cnt);
            sum += cnt;
        }
        if (sum == 0) return board;

        x = rnd_walk (hist, val);
        //cout << "->" << x <<  "\n";
        val = N;
        for (y = 0; y < N; y++) {
            cnt = actual.attack (x, y);

            val = min (val, cnt);
            hist[y] = cnt;
            //cout << cnt << endl;
        }
        y = rnd_walk (hist, val);

        board[x] = y;
        actual.reset(board);
    }

    return {};
}

double schedule (double t) {
  const double k = 20, lam = 0.005, limit = 30000;
  return t < limit ? (k * exp (-lam * t)) : 0;
}
int cost2 (vector<int> &board) {
    int cnt = 0, N = board.size(), *ptr = board.data();

    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) { // avoid counting twice
            if ((i + ptr[i] == j + ptr[j]) || (i - ptr[i] == j - ptr[j]))
                cnt++;
        }
    }
    return cnt;
}
vector<int> simulated_annealing2 (int N, pair<int,int> pos) { // N = 400 => 12 ms
    //double temp = 100, thresh = 0.0001, decay = 0.99;
    vector<int> board = generate3 (N, pos);
    int a, b, curr, next ;
    int index = 1;
    double T, p;

    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<> dist (0, board.size() - 1);
    uniform_real_distribution<> rand (0, 1);

    while (index < 30000) {

        curr = cost2 (board);

        do {
            a = dist (gen), b = dist (gen);
        } while (a == pos.first || b == pos.first) ;

        swap (board[a], board[b]);

        next = cost2 (board);

        if (next == 0) return board;

        if (next > curr) {  // if the cost of neighbor isn't less than the cost of solution
            //T = 100 / index;             // 0.12 ms
            T = schedule (index);      // 0.24 ms
            //p = exp (curr - next / T);   // compute p = e^(-delta / t)
            if (rand(gen) > p) {
                swap (board[a], board[b]);
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

    int index = 20;
    vector<int> res;

    while (index-->0) {
        res = min_conflict4 (N, pos);
        Display::board (res);
        //if (res.size() != 0) return format(res);
    }

    return "";
}

int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now ();
    chrono::duration<double> elapsed;

    int index = 100, cnt = 0;
    string res;
    vector<int> board = {7,4,3,4,0,1,0,5};

    res = solveNQueens (8, {1,1});

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
//////////////////////////////// Arkive ///////////////////////////////////////
bool attack2 (const int ax, const int ay, const int bx, const int by) {
    return (ay == by) || (ax + ay == bx + by) || (ax - ay == bx - by);
}
vector<int> generate1 (int N, pair<int,int> pos) {
    vector<int> track (N);
    uniform_int_distribution<> dist (0, N - 1);

    while (N-->0)
        track[N] = (N == pos.first) ? pos.second : dist(gen);

    return track;
  }
bool attack (const pair<int,int> &a, const pair<int,int> &b) {

  return (a.second == b.second) ||
         (a.first + a.second == b.first + b.second) ||
         (a.first - a.second == b.first - b.second);
       }
pair<int,int> search (vector<int> track) {
    int N = track.size();
    int x, y;
    pair<int,int> a, b, c, d;

    vector<int> hist (N);
    vector<vector<int>> mat (N, vector<int> (N));

    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

    for (x = 0; x < N; x++) {
        a = make_pair (x, track[x]);

        int cntx = 0;
        for (y = 0; y < N; y++) {
            c = {x, y}, b = make_pair (y, track[y]);

            if (a != b && attack (a, b)) cntx++;

            int cnty = 0;
            for (int j = 0; j < N; j++) {
                d = make_pair (j, track[j]);
                if (c != d && attack (c, d)) cnty++;
            }
            mat[x][y] = cnty;

        }
        hist[x] = cntx;

    }

    int maxv = -N;

    for (x = 0; x < N; x++) {
        for (y = 0; y < N; y++) {
            //cout << setw(3) << hist[x] - mat[x][y];
            maxv = max (maxv, hist[x] - mat[x][y]);
        }
        //cout << endl;
    }

    vector<pair<int,int>> points;
    for (x = 0; x < N; x++) {
        for (y = 0; y < N; y++) {
            if (hist[x] - mat[x][y] == maxv)
                points.push_back({x,y});
        }
    }
    uniform_int_distribution<> dist (0, points.size() - 1);

    return points[dist(gen)];
}
bool complete (vector<int> &board) {
  const int N = board.size();
  pair<int,int> a, b;

  for (int x = 0; x < N; x++) {
      a = make_pair (x, board[x]);

      for (int y = 0; y < N; y++) {
          b = make_pair (y, board[y]);

          if (a != b && attack (a, b)) return false;
      }

  }
    return true;
}
vector<int> min_conflicts1 (vector<int> track, int pos) {

    int index = track.size() * 50;

    while (index-->0) {
        auto [x, y] = search (track);
        track[x] = y;

        if (complete (track) == true) return track;
    }

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
vector<int> simulated_annealing (int N, pair<int,int> pos) {
    //double temp = 100, thresh = 0.0001, decay = 0.99;
    vector<int> board = generate1 (N, pos);
    int x, y, curr, next ;
    int index = 1;
    double T, p;

    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<> dist (0, board.size() - 1);
    uniform_real_distribution<> rand (0, 1);

    while (index < 30000) {
        curr = cost (board);

        x = dist (gen), y = dist (gen);   // let neighbor be a random neighbor of solution
        if (x == pos.first) continue;
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
