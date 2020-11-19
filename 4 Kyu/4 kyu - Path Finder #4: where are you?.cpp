#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
typedef vector<int> test_t;
class Assert {
  public :
  static void That (test_t actual, test_t expression) {
    //if (actual != expression)
    //std::cout<<"actual : "<< actual <<" expected : "<< expression <<std::endl;
  }
};
test_t Equals (test_t entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////
std::vector<int> i_am_here (std::string path);

const vector<pair<int,int>> compass{{1,0},{0,-1},{-1,0},{0,1}};
struct direction {
    int direc;
    int accu;
};

void display (vector<vector<int>> grid) {
    for (auto x : grid) {
        for (auto y : x)
            if (y == 0)
                cout << "  ";
            else
                cout << y << ' ';

        cout << endl;
    }

}

direction instruct (char c) {
//direction actual;
    switch (c) {
        case 'l' : return {-1,1};
        case 'L' : return {-1,2};
        case 'r' : return {1,1};
        case 'R' : return {1,2};
    }

    return {};
}
void mark (vector<vector<int>> &grid, pair<int,int> p) { grid[p.first][p.second] = 1; }

int main () {
    auto start = std::chrono::high_resolution_clock::now();

    int idx = 3;
    const vector<vector<int>> v {
      {1,2,3},
      {4,5,6},
      {7,8,9}};

    pair<int,int> p {1,1};
/*
    for (int i = 0; i < 4; ++i) {
        p.first += compass2[i].first;
        p.second += compass2[i].second;
        cout << "<" << v[p.first][p.second] << ">";
    }
    */
    string path = "r5L2l4";


    i_am_here (path);


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " <<std::fixed<< elapsed.count()  << " ms" << std::endl;
}

std::vector<int> i_am_here (std::string path) {
    int moves = 0, idx = 1, mul = 1, size = 15;

    vector<vector<int>> grid (size, vector<int> (size));
    pair<int,int> p {size / 2,size / 2}, start = p, end;
    std::string::iterator it = path.begin();

    do {
        char c = *it;
        if (isdigit(c))
            moves = moves * 10 + (c - '0');
        if (isalpha(c)) {
            //cout << moves << ' ' << it << endl;
            moves *= mul;
            for (int i = 0; i < moves; ++i) {
            //while (moves-->0) {
                p.first += compass[idx].first;
                p.second += compass[idx].second;
                //cout << '<' << p.first <<','<<p.second<<'>' << endl;
                mark (grid, p);
            }
            switch (c) {
                case 'l' : idx--; mul = 1; break;
                case 'L' : idx--; mul = 2; break;
                case 'r' : idx++; mul = 1; break;
                case 'R' : idx++; mul = 2; break;
            }
            moves = 0;
        }
        //cout << '<' << compass[idx].first << ',' << compass[idx].second << '>' << endl;
        if  (it == path.end()) {
            //cout << '<' << p.first << ',' << p.second << '>' << endl;
            moves *= mul;
            for (int i = 0; i < moves; ++i) {
                p.first += compass[idx].first;
                p.second += compass[idx].second;
                mark (grid, p);
            }
            /*
            */
        }
    } while (it++ != path.end());

    display (grid);

    cout << '<' << p.first - start.first << ','
         << p.second - start.second << '>' << endl;
    /*
    /*
    */
    //cout << moves;
  return {0, 0};
}
void Test () {
    Assert::That(i_am_here(""      ), Equals(std::vector<int>{0, 0}));
    Assert::That(i_am_here("RLrl"  ), Equals(std::vector<int>{0, 0}));
    Assert::That(i_am_here("r5L2l4"), Equals(std::vector<int>{4, 3}));

    /* => from java
    public static Collection<Object[]> prepTests() {
      return Arrays.asList( new Object[] {"",       new Point(  0, 0)},
                            new Object[] {"RLrl",   new Point(  0, 0)},
                            new Object[] {"r5L2l4", new Point(  4, 3)},
                            new Object[] {"r5L2l4", new Point(  0, 0)},
                            new Object[] {"10r5r0", new Point(-10, 5)},
                            new Object[] {"10r5r0", new Point(  0, 0)});
  }
      */
}
