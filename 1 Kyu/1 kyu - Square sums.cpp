#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

struct vertex {
  int id, visit;
  vector<vertex*> edges;
};

class Timer {
    //using time = chrono::steady_clock;
    private :
        chrono::steady_clock::time_point alpha, end;
        chrono::duration<double> elapsed;
        uint64_t cycle;

        void update () {
            end = chrono::steady_clock::now ();
            elapsed = end - alpha;
        }

    public :

        Timer() {
            alpha = chrono::steady_clock::now ();
            cycle = 0;
        }
        void start () { alpha = chrono::steady_clock::now ();}
        void stop () { update();}
        void get_duration () {
            std::cout << "\nDuration "
                      <<fixed<< elapsed.count()
                      << " ms" << std::endl;
        }
        bool running (double total) {
            update();
            cycle++;
            if (elapsed.count() < total) return true;
            cout << "cycles :: " ;

            string num = to_string (cycle);
            int size = num.size(), index = 0;

            while (size-->0) {
                cout << num[index++];
                if (size % 3 == 0) cout << ' ';
            }
            cout << endl;
            return false;
        }
};
class Display {
    public :

        static void graph (vertex **src) {
            for (int i = 1; src[i] ; i++) {

            }
            cout << endl;
        }
        static void vect (vector<int> src) {

            for (auto &it : src)
                cout << it << " ";

            cout << endl;
        }
};

class graph {
      private :
          vector<int> sqr;
          vector<vertex> vmap;

      public :
          int N;

          graph (int n) {
              N = n;

              for (int i = 2; i * i < N * 2; i++) {
                  sqr.push_back (i * i);
              }

              for (int i = 0; i < N + 1 ; i++) {
                  vmap.push_back ({i, false, vector<vertex *>()});
              }

              for (int i = 1; i < vmap.size(); i++) {
                  update (&vmap[i]);
              }
          }
          void update (vertex *v) {

              v->edges.clear();

              for (auto &sq : sqr) {
                  int val = sq - v->id;

                  if (val > 0 && val <= N && val != v->id && !vmap[val].visit) {
                        v->edges.push_back (&vmap[val]);
                  }
              }
          }

          vertex *operator [] (int index) { return &vmap[index]; }
};

int getstart (graph &curr) {
  int minv = 99, start;

  for (int i = 1; i < curr.N + 1; i++) {
      if (curr[i]->edges.size() <= minv) {
        minv = curr[i]->edges.size();
        start = i;
      }
  }
  return start;
}
bool dfs (int index, graph &curr, vector<int> &seq) {

    if (index == seq.size() - 1) return true;

    vertex *u = curr[seq[index]];

    for (auto edge : u->edges) {
        curr.update (edge);
    }

    sort (u->edges.begin(), u->edges.end(), [] (const vertex *a, const vertex *b) {
        int aa = a->edges.size(), bb = b->edges.size();
        return (aa != bb) ? aa < bb : a->id < b->id;
    });

    for (auto edge : u->edges) {
        seq[index + 1] = edge->id;
        edge->visit = true;

        if (dfs (index + 1, curr, seq)) return true;

        seq[index + 1] = 0;
        edge->visit = false;
    }

    return false;
}
vector<int> square_sums_row (int N) {

    graph curr (N);
    vector<int> seq (N);

    bool found = false;
    seq[0] = getstart (curr);
    curr[seq[0]]->visit = true;


    found = dfs (0, curr, seq);
    /*
    for (int i = 1; i < N + 1; i++) {
      cout << curr[i]->visit << " -> ";
      for (auto it : curr[i]->edges) {
        cout << it->visit << " ";
      }
      cout << endl;
    }
    */
    //curr.close();

    return found ? seq : vector<int>{};
}

int main () {

    vector<int> test = {15, 50, 128, 256, 512, 980};
    vector<int> vec;

    for (auto N : test) {
        Timer clock;
        auto res = square_sums_row(N);

        Display::vect (res);

        clock.stop();
        clock.get_duration();
    }

}
