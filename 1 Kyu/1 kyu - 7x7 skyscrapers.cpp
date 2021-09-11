#include <iostream>
#include <vector>
#include <list>
#include <random>
#include <algorithm>

#include <iomanip>
#include <chrono>
//////////////////////////////////func def//////////////////////////////////////
void Test () ;
////////////////////////////////////////////////////////////////////////////////
int DN;

using namespace std;

struct point {int x, y; };
struct cluster { int comb[8]; };

class Skyscraper {
    private :
        int N;

    public :
        vector<int> clues;
        vector<vector<int>> grid;
        vector<list<cluster>> row, col;

        Skyscraper (const vector<int> &src = {}) {
            clues = src;
            N = src.size() / 4, DN = N;
            row.resize(N), col.resize(N);
            grid.resize(N, vector<int>(N));

            cluster num;
            generate (&num.comb[0], &num.comb[N], [i = 1] () mutable { return i++;});

            vector<pair<cluster, pair<int,int>>> combinations;

            do {
                combinations.push_back({num, check_num(num)});
            } while (next_permutation (&num.comb[0], &num.comb[N]));

            for (int i = 0; i < N; ++i) {

                int west = ((N * 4) - 1) - i, east = N + i, south = west - N, north = i;
                pair<int,int> lateral = {clues[west], clues[east]}, vertical = {clues[north], clues[south]};

                row[i] = make_combs (combinations, lateral);
                col[i] = make_combs (combinations, vertical);
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
        list<cluster> make_combs (vector<pair<cluster, pair<int,int>>> &input, pair<int,int> p) {
            list<cluster> stack;

            for (auto &curr : input) {
                cluster &now = curr.first;
                pair<int,int> &comb {curr.second};

                if (p.first != 0 && p.second != 0) {
                    if ((comb.first == p.first) && (comb.second == p.second))
                        stack.push_back(now);
                }

                else if (p.first != 0 && comb.first == p.first)
                        stack.push_back(now);

                else if (p.second != 0 && comb.second == p.second)
                        stack.push_back(now);

                else if (p.second == 0 && p.first == 0 )
                        stack.push_back(now);
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

bool growing (Skyscraper &city) {

    const int size = city.size(), total = size * size;
    static int fuse, expansion;
    int count = 0;

    for (int i = 0; i != size; i++)
        for (int j = 0; j != size; j++)
            if (city.grid[i][j] != 0)
                count++;

    if (count == 0) return true;
    if (count == total) {
        fuse = 0;
        return false;
    };
    if (count == expansion) fuse++;
    if (fuse >= 3) {
        fuse = 0, expansion = 0;
        return false;
    }

    expansion = count;

    return true;
}

int reduce (list<cluster> &input, list<cluster> &check, pair<int, int> &p) {

    bool flag = true;
    vector<int> buffer(8);
    int *cell = buffer.data(), output = 0, curr;
    list<cluster>::iterator index = input.begin();

    auto [x, y] = p;

    for (auto &it : check)
        cell[it.comb[y]] = true;

    while (index != input.end()) {
        curr = index->comb[x];

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

    while (growing (city))
        for (y = 0; y != size; y++)
            for (x = 0; x != size ; x++) {
              city.grid[y][x] = reduce (city.row[y], city.col[x], p);
              city.grid[x][y] = reduce (city.col[y], city.row[x], p);
            }
}
vector<vector<int>> SolvePuzzle (const vector<int> &clues) {

    Skyscraper city (clues);
    const int size = city.size();
    pair<int, int> p;
    auto &[x, y] = p;


    return city.grid;
}

int search_rnd (vector<list<cluster>> &now, int size) {

    size_t val = 999;
    vector<int> hist;

    random_device rd;
    mt19937 gen(rd());

    for (auto &it : now) {
        if (it.size() > 1)
            val = min (val, it.size());
    }

    for (int i = 0; i < size; i++) {
        if (now[i].size() == val)
            hist.push_back(i);
    }
    uniform_int_distribution<> dis (0, hist.size() - 1);

    return hist[dis(gen)];
}
pair<int, int> min_pos (Skyscraper &city) {
    pair<int, int> p;
    auto &[x, y] = p;
    int minx = 999, miny = 999;

    for (int i = 0; i < city.size(); i++) {

        int valx = city.row[i].size();
        int valy = city.col[i].size();

        if (valx > 1 && valx < minx) {
            minx = valx;
            x = i;
        }
        if (valy > 1 && valy < miny) {
            miny = valy;
            y = i;
        }
    }
    return {x, y};
}
int main(void) {

    auto start = std::chrono::high_resolution_clock::now();

    vector<int> clues {3,3,2,1,2,2,3,4,3,2,4,1,4,2,2,4,1,4,5,3,2,3,1,4,2,5,2,3};

    Skyscraper city (clues);
    const int size = city.size();
    pair<int, int> p;
    auto &[x, y] = p;
    /*
    */

    p = min_pos (city);

    city.grid[y][x] = 9;

    Display::graph (city);
    //Test();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Process took " << elapsed.count()  << " ms" << std::endl;
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
