#include <iostream>
#include <vector>
#include <list>
#include <random>
#include <algorithm>

#include <chrono>
//////////////////////////////////func def//////////////////////////////////////
void Test () ;

////////////////////////////////////////////////////////////////////////////////
int DN;

using namespace std;

class Skyscraper {
    private :
    int fact (int n) { return (n == 0) || (n == 1) ? 1 : n * fact (n - 1);}
    public :
        int N;
        vector<int> clues;
        vector<vector<int>> grid;
        vector<list<vector<int>>> row, col;

        Skyscraper (const vector<int> src = {}) {
            clues = src;
            N = src.size() / 4, DN = N;
            row.resize(N), col.resize(N);
            grid.resize(N, vector<int>(N));

            vector<int> num (N);
            generate (&num[0], &num[N], [i = 1] () mutable { return i++;});
            int ncomb = fact (N);
            //vector<pair<vector<int>, pair<int,int>>> combinations;
            vector<pair<vector<int>, pair<int,int>>> combinations;
            do {

                //combinations.push_back ({num, check_num (num)});
            } while (next_permutation (&num[0], &num[N]));
            /*
            for (int i = 0; i < N; ++i) {

                int west = ((N * 4) - 1) - i, east = N + i, south = west - N, north = i;
                pair<int,int> lateral = {clues[west], clues[east]}, vertical = {clues[north], clues[south]};

                row[i] = make_combs (combinations, lateral);
                col[i] = make_combs (combinations, vertical);
            }
            */
        }

        pair<int,int> check_num (vector<int> &now) {

            int end = N - 1, first = 0, sec = 0, index = N;
            pair<int,int> comb = {0,0};

            while (index-->0) {
                if (now[index] > sec) {
                    sec = now[index];
                    comb.second++;
                }
                if (now[N - 1 - index] > first) {
                    first = now[end - index];
                    comb.first++;
                }
            }
            return comb;
        }
        list<vector<int>> make_combs (vector<pair<vector<int>, pair<int,int>>> &input, pair<int,int> &p) {
            list<vector<int>> stack;

            for (auto &curr : input) {

                pair<int,int> &comb {curr.second};

                if (p.first != 0 && p.second != 0) {
                    if ((comb.first == p.first) && (comb.second == p.second))
                        stack.push_back (curr.first);
                }
                else if (p.first != 0 && comb.first == p.first)
                        stack.push_back (curr.first);

                else if (p.second != 0 && comb.second == p.second)
                        stack.push_back (curr.first);

                else if (p.second == 0 && p.first == 0 )
                        stack.push_back (curr.first);
            }

            return stack;
        }

        bool csp (pair<int,int> &p, int num = -1) {
            auto [x,y] = p;

            for (int i = 0; i < N; i++) {
                if (grid[y][i] == num || grid[i][x] == num) return false;
            }

            int west = ((N * 4) - 1) - y, east = N + y, south = (((N * 4) - 1) - x) - N, north = x;
            cout << clues[north] << " " << clues[south] << endl;

            return false;
        }

};

class Debug {
    public :
        static void display (Skyscraper &curr) {
            int i, j, up;

            cout << " ";
            for (i = 0; i != curr.N; ++i)
                cout << " " << curr.clues[i] << " ";
            cout << endl;

            for (i = 0; i != curr.N; ++i) {
                up = ((curr.N * 4) - 1) - i;

                cout << curr.clues[up];
                for (j = 0; j != curr.N; ++j) {
                    if (curr.grid[i][j])
                        cout <<  "[" << curr.grid[i][j] << "]";
                    else
                        cout << ("[ ]");
                }
                cout << curr.clues[curr.N + i];
                cout << endl;
            }
            cout << " ";

            for (i = 0; i != curr.N; ++i)
                cout << " " << curr.clues[((curr.N * 4) - 1) - i - curr.N] << " ";
            cout << endl;

        }

        static void read_clust (const vector<int> &input) {
            for (int i = 0; i < DN; i++)
                cout << input[i];
            cout << endl;

        }
        static void read_comb (list<vector<int>> &root) {

            for (vector<int> &node : root) {
                read_clust(node);
            }
        }
};

int count (vector<vector<int>> &grid) {
    int cnt = 0, size = grid.size();

    for (int i = 0; i != size; i++)
        for (int j = 0; j != size; j++)
            if (grid[i][j] != 0)
                cnt++;
    return cnt;
}
bool growing (Skyscraper &city) {

    const int size = city.N, total = size * size;
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

int reduce (list<vector<int>> &input, list<vector<int>> &check, pair<int,int> p) {

    vector<int> buffer(8);
    bool flag = true;
    int *cell = buffer.data(), output = 0, curr;

    for (auto &it : check)
        cell[it[p.second]] = true;

    list<vector<int>>::iterator index = input.begin();

    while (index != input.end()) {
        curr = index->at(p.first);

        if (cell[curr] == false)
            index = input.erase(index);
        else {
            if (output > 0 && output != curr)
                flag = false;

            output = curr;
            index++;
        }
    }

    return  (flag == true) ? output : 0;
}

Skyscraper update (Skyscraper &city) {
    const int size = city.N;
    pair<int,int> p;
    auto &[x, y] = p;

    while (growing (city)) {
        for (y = 0; y != size; ++y)
            for (x = 0; x != size ; ++x) {
                city.grid[y][x] = reduce (city.row[y], city.col[x], p);
                city.grid[x][y] = reduce (city.col[y], city.row[x], p);
            }
    }

    return city;
}
pair<int,int> search_min (Skyscraper &city) {
    int x = 0, y = 0, cntx, cnty, minx = 999, miny = 999;

    for (int i = 0; i < city.N; i++) {
        cntx = city.col[i].size();
        cnty = city.row[i].size();

        if (cntx > 1 && cntx < minx) {
            minx = cntx;
            x = i;
        }

        if (cnty > 1 && cnty < miny) {
            miny = cnty;
            y = i;
        }
    }

    return {x,y};
}
void min_conflict (Skyscraper &city) {

    const int total = city.N * city.N;
    int xx = search_min (city).first;

    Skyscraper tmp;
    list<vector<int>> &lst = city.col[xx];
    list<vector<int>> backup = lst;

    lst.clear();
    while (backup.size() > 0) {
        tmp = city;

        tmp.col[xx].push_back (backup.back());
        backup.pop_back();

        update (tmp);

        //Debug::display (tmp);
        if (count (tmp.grid) == total) break;
    }
    city = tmp;
    /*
    */
}
vector<vector<int>> SolvePuzzle (const vector<int> &clues) {

    Skyscraper city (clues);
    const int total = city.N * city.N;

    // update (city);

    //if (count (city.grid) != total) {
        min_conflict (city);
    //}

    Debug::display (city);

    return city.grid;
}

vector<int> genboard (int size) {
    const int end = size * 4;
    vector<int> clues;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist (0, size);

    for (int i = 0; i < end; i++)
        clues.push_back (dist(gen));

    return clues;
}

int main () {

    auto start = std::chrono::high_resolution_clock::now();

    vector<int> clues = {2,3,3,3,2,1, 1,4,2,2,3,5, 4,3,2,2,1,5, 2,3,2,3,1,2};
    int N = 4;
    clues = genboard (N);

    Skyscraper city (clues);
    pair p {0,0};
    auto &[x,y] = p;


    //SolvePuzzle ({2,2,1,3,2,2,3,1,1,2,2,3,3,2,1,3});
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
