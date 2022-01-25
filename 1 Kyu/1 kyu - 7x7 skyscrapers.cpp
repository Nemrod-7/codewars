#include <iostream>
#include <vector>
#include <list>
#include <random>
#include <algorithm>

#include "../../templates/Assert.hpp"

int DN;

using namespace std;

struct cluster { int comb[8]; };

class Skyscraper {
    private :
        int N;

    public :
        vector<int> clues;
        vector<vector<int>> grid;
        vector<list<int>> row, col;
        vector<pair<cluster, pair<int,int>>> combs;

        Skyscraper (const vector<int> &src = {}) {
            clues = src;
            N = src.size() / 4, DN = N;
            row.resize(N), col.resize(N);
            grid.resize(N, vector<int>(N));

            cluster num;
            generate (&num.comb[0], &num.comb[N], [i = 1] () mutable { return i++;});

            do {
                combs.push_back({num, check_num(num)});
            } while (next_permutation (&num.comb[0], &num.comb[N]));

            for (int i = 0; i < N; ++i) {

                int west = ((N * 4) - 1) - i, east = N + i, south = west - N, north = i;
                pair<int,int> lateral = {clues[west], clues[east]}, vertical = {clues[north], clues[south]};

                row[i] = make_combs (combs, lateral);
                col[i] = make_combs (combs, vertical);
            }
        }

        pair<int,int> check_num (cluster &now) {

            int end = N - 1, first = 0, sec = 0, index = N;
            pair<int,int> comb = {0,0};

            while (index-->0) {
                if (now.comb[index] > sec) {
                    sec = now.comb[index];
                    comb.second++;
                }
                if (now.comb[N - 1 - index] > first) {
                    first = now.comb[end - index];
                    comb.first++;
                }
            }
            return comb;
        }
        list<int> make_combs (vector<pair<cluster, pair<int,int>>> &input, pair<int,int> p) {
            list<int> stack;

            for (int i = 0; i < input.size(); i++) {

                pair<int,int> &comb {input[i].second};

                if (p.first != 0 && p.second != 0) {
                    if ((comb.first == p.first) && (comb.second == p.second))
                        stack.push_back (i);
                }

                else if (p.first != 0 && comb.first == p.first)
                        stack.push_back (i);

                else if (p.second != 0 && comb.second == p.second)
                        stack.push_back (i);

                else if (p.second == 0 && p.first == 0 )
                        stack.push_back (i);
            }
            return stack;
        }
        int size () { return N; }
};

class Display {
    public :
        static void graph (Skyscraper &curr) {
            int i, j, up;

            cout << " ";
            for (i = 0; i != curr.size(); ++i)
                cout << " " << curr.clues[i] << " ";
            cout << endl;

            for (i = 0; i != curr.size(); ++i) {
                up = ((curr.size() * 4) - 1) - i;

                cout << curr.clues[up];
                for (j = 0; j != curr.size(); ++j) {
                    if (curr.grid[i][j])
                        cout <<  "[" << curr.grid[i][j] << "]";
                    else
                        cout << ("[ ]");
                }
                cout << curr.clues[curr.size() + i];
                cout << endl;
            }
            cout << " ";

            for (i = 0; i != curr.size(); ++i)
                cout << " " << curr.clues[((curr.size() * 4) - 1) - i - curr.size()] << " ";
            cout << endl;
            cout << endl;

        }

        static void read_clust (const cluster &input) {
            for (int i = 0; i < DN; i++)
                cout << input.comb[i];
            cout << endl;

        }
        static void read_comb (list<cluster> &root) {

            for (cluster &node : root) {
                read_clust(node);
            }
        }
};

int count (vector<vector<int>> &grid) {

    int size = grid.size(), cnt = 0;

    for (int i = 0; i != size; i++)
        for (int j = 0; j != size; j++)
            if (grid[i][j] != 0)
                cnt++;

    return cnt;
}
bool growing (Skyscraper &city) {

    const int size = city.size(), total = size * size;
    static int fuse, expansion;

    int cnt = count (city.grid);

    //if (cnt == 0) return true;
    if (cnt == total) {
        fuse = 0;
        return false;
    };
    if (cnt == expansion) fuse++;
    if (fuse >= 3) {
        fuse = 0, expansion = 0;
        return false;
    }

    expansion = cnt;

    return true;
}

int reduce (Skyscraper &city, list<int> &input, list<int> &check, pair<int, int> &p) {

    bool flag = true;
    vector<int> buffer(8);
    int *cell = buffer.data(), output = 0, curr;
    list<int>::iterator index = input.begin();
    auto *base = city.combs.data();
    auto [x, y] = p;

    for (auto &pos : check) {
        cluster &act = base[pos].first;
        cell[act.comb[y]] = true;
    }

    while (index != input.end()) {
        cluster &act = base[*index].first;
        curr = act.comb[x];

        if (cell[curr] == false) {
            index = input.erase(index);
        } else {
            if (output > 0 && output != curr)
                flag = false;

            output = curr;
            index++;
        }
    }

    return  (flag == true) ? output : 0;
}

void update (Skyscraper &city) {
    const int size = city.size();
    pair<int, int> p;
    auto &[x, y] = p;

    while (growing (city)) {
        for (y = 0; y != size; y++) {
            for (x = 0; x != size ; x++) {
              city.grid[y][x] = reduce (city, city.row[y], city.col[x], p);
              city.grid[x][y] = reduce (city, city.col[y], city.row[x], p);
            }
        }
    }
    Display::graph (city);
}
vector<vector<int>> SolvePuzzle (const vector<int> &clues) {

    Skyscraper city (clues);
    const int size = city.size();

    update (city);

    return city.grid;
}

int main () {

    Timer clock;

    vector clues {3,3,2,1,2,2,3,4,3,2,4,1,4,2,2,4,1,4,5,3,2,3,1,4,2,5,2,3};
    // 0.265 ms
    Test();
    //cout << T / total;

    clock.get_duration();
    return 0;
}

void Test() {

  /*
  */
    //4 x 4 skycrapers
    SolvePuzzle({ 2, 2, 1, 3,  2, 2, 3, 1,  1, 2, 2, 3,  3, 2, 1, 3 });
    static vector<vector<int>> clues = {
      { 3, 2, 2, 3, 2, 1,
        1, 2, 3, 3, 2, 2,
        5, 1, 2, 2, 4, 3,
        3, 2, 1, 2, 2, 4 },
      { 0, 0, 0, 2, 2, 0,
        0, 0, 0, 6, 3, 0,
        0, 4, 0, 0, 0, 0,
        4, 4, 0, 3, 0, 0 },
      { 0, 3, 0, 5, 3, 4,
        0, 0, 0, 0, 0, 1,
        0, 3, 0, 3, 2, 3,
        3, 2, 0, 3, 1, 0}
    };
    static vector<vector<vector<int>>> expected ={
        {{ 2, 1, 4, 3, 5, 6},
         { 1, 6, 3, 2, 4, 5},
         { 4, 3, 6, 5, 1, 2},
         { 6, 5, 2, 1, 3, 4},
         { 5, 4, 1, 6, 2, 3},
         { 3, 2, 5, 4, 6, 1 }},
        {{ 5, 6, 1, 4, 3, 2 },
         { 4, 1, 3, 2, 6, 5 },
         { 2, 3, 6, 1, 5, 4 },
         { 6, 5, 4, 3, 2, 1 },
         { 1, 2, 5, 6, 4, 3 },
         { 3, 4, 2, 5, 1, 6 }},
        {{ 5, 2, 6, 1, 4, 3 },
         { 6, 4, 3, 2, 5, 1 },
         { 3, 1, 5, 4, 6, 2 },
         { 2, 6, 1, 5, 3, 4 },
         { 4, 3, 2, 6, 1, 5 },
         { 1, 5, 4, 3, 2, 6 }}
    };
    SolvePuzzle ({2,2,1,3,2,2,3,1,1,2,2,3,3,2,1,3});
    SolvePuzzle ({0,0,1,2,0,2,0,0,0,3,0,0,0,1,0,0});
    SolvePuzzle ({1,2,4,2,2,1,3,2,3,1,2,3,3,2,2,1});
    SolvePuzzle ({2,2,1,3,2,2,3,1,1,2,2,3,3,2,1,3});
    SolvePuzzle ({0,2,0,0,0,3,0,0,0,1,0,0,0,0,1,2});
    SolvePuzzle ({2,1,3,2,3,1,2,3,3,2,2,1,1,2,4,2});
    SolvePuzzle ({2,2,3,1,1,2,2,3,3,2,1,3,2,2,1,3});
    SolvePuzzle ({0,2,0,0,0,3,0,0,0,1,0,0,0,0,1,2});
    SolvePuzzle ({3,1,2,3,3,2,2,1,1,2,4,2,2,1,3,2});
    SolvePuzzle ({1,2,2,3,3,2,1,3,2,2,1,3,2,2,3,1});
    SolvePuzzle ({0,3,0,0,0,1,0,0,0,0,1,2,0,2,0,0});
    SolvePuzzle ({3,1,2,3,3,2,2,1,1,2,4,2,2,1,3,2});
    SolvePuzzle ({3,2,1,3,2,2,1,3,2,2,3,1,1,2,2,3});

    // 6 x 6 skycrapers
    SolvePuzzle (clues[0]);
    SolvePuzzle (clues[1]);
    SolvePuzzle (clues[2]);
    SolvePuzzle({0,3,0,5,3,4,0,0,0,0,0,1,0,3,0,3,2,3,3,2,0,3,1,0});
    SolvePuzzle ({0,0,3,0,3,3, 0,4,4,4,0,0, 0,0,5,0,5,0, 0,2,2,0,2,0});
    SolvePuzzle ({0,4,2,0,3,3, 0,0,0,0,3,2, 4,0,0,3,0,5, 4,0,0,3,0,2});
    SolvePuzzle ({0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6});

    SolvePuzzle ({3,1,2,3,2,5, 3,2,3,3,2,1, 1,3,3,2,5,3, 3,2,1,3,2,2});

     clues = {
      { 7, 0, 0, 0, 2, 2, 3,
        0, 0, 3, 0, 0, 0, 0,
        3, 0, 3, 0, 0, 5, 0,
        0, 0, 0, 0, 5, 0, 4 },
      { 0, 2, 3, 0, 2, 0, 0,
        5, 0, 4, 5, 0, 4, 0,
        0, 4, 2, 0, 0, 0, 6,
        5, 2, 2, 2, 2, 4, 1 }
    };

     expected = {
      { { 1, 5, 6, 7, 4, 3, 2 },
        { 2, 7, 4, 5, 3, 1, 6 },
        { 3, 4, 5, 6, 7, 2, 1 },
        { 4, 6, 3, 1, 2, 7, 5 },
        { 5, 3, 1, 2, 6, 4, 7 },
        { 6, 2, 7, 3, 1, 5, 4 },
        { 7, 1, 2, 4, 5, 6, 3 } },
      { { 7, 6, 2, 1, 5, 4, 3 },
        { 1, 3, 5, 4, 2, 7, 6 },
        { 6, 5, 4, 7, 3, 2, 1 },
        { 5, 1, 7, 6, 4, 3, 2 },
        { 4, 2, 1, 3, 7, 6, 5 },
        { 3, 7, 6, 2, 1, 5, 4 },
        { 2, 4, 3, 5, 6, 1, 7 } }
    };

    for (auto now : clues) SolvePuzzle (now);

      SolvePuzzle({
        2,3,3,4,4,2,1,
        1,6,4,4,2,2,2,
        2,2,3,3,1,3,3,
        2,5,3,3,2,1,5});

      SolvePuzzle({
        2,2,0,2,0,2,0,
        0,3,3,0,3,3,0,
        5,4,6,3,0,0,0,
        0,0,4,4,0,0,4
      });

      SolvePuzzle({0,0,5,0,0,0,6,4,0,0,2,0,2,0,0,5,2,0,0,0,5,0,3,0,5,0,0,3});
      SolvePuzzle({0,0,5,3,0,2,0,0,0,0,4,5,0,0,0,0,0,3,2,5,4,2,2,0,0,0,0,5});
      SolvePuzzle({0,2,3,0,2,0,0,5,0,4,5,0,4,0,0,4,2,0,0,0,6,5,2,2,2,2,4,1});
      SolvePuzzle({0,2,3,0,2,0,0,5,0,4,5,0,4,0,0,4,2,0,0,0,6,0,0,0,0,0,0,0});
      SolvePuzzle({7,0,0,0,2,2,3,0,0,3,0,0,0,0,3,0,3,0,0,5,0,0,0,0,0,5,0,4});
      SolvePuzzle({0,0,0,0,5,0,4,7,0,0,0,2,2,3,0,0,3,0,0,0,0,3,0,3,0,0,5,0});
      SolvePuzzle({0,0,0,0,5,0,4,7,0,0,0,2,2,3,0,0,3,0,0,0,0,3,0,3,0,0,5,0});

      // no solutions
      SolvePuzzle ({2,3,3,3,2,1, 1,4,2,2,3,5, 4,3,2,2,1,5, 2,3,2,3,1,2});
      SolvePuzzle ({2,3,3,1,2,6, 3,4,2,2,2,1, 1,3,5,2,2,3, 4,2,2,3,1,2});
      SolvePuzzle({3,3,2,1,2,2,3,4,3,2,4,1,4,2,2,4,1,4,5,3,2,3,1,4,2,5,2,3});

}
