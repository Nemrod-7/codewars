#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <chrono>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
typedef unsigned test_t;
class Assert {
    public :
        static void That (test_t actual, test_t expression) {
            if (actual != expression)
                cout<<"actual : "<<actual<<" expected : "<<expression<<endl;
        }
};
test_t Equals (test_t entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////
/*
vector<pair<int,int>> a{{0,1},{1,0},{1,1},{1,2},{2,1}};

A->B D E F H
C->B D E F H
G->B D E F H
I->B D E F H
{{0,1},{1,0},{1,1},{1,2},{2,1}}

B->A C D E F G I
H->A C D E F G I
{{0,0},{0,2},{1,0},{1,1},{1,2},{2,0},{2,2}}

D->A B C E G H I
F->A B C E G H I
{0,0},{0,1},{0,2},{1,1},{2,0},{2,1},{2,2}

E->A B C D F G H I
{0,0},{0,1},{0,2},{1,0},{1,2},{2,0},{2,1},{2,2}
*/
map<char,pair<int,int>> coord {
    {'A',{0,0}},{'B',{0,1}},{'C',{0,2}},
    {'D',{1,0}},{'E',{1,1}},{'F',{1,2}},
    {'G',{2,0}},{'H',{2,1}},{'I',{2,2}}};

vector<vector<int>> keypad {
    {'A','B','C'},
    {'D','E','F'},
    {'G','H','I'}
};

map<char,vector<pair<int,int>>> moves {
  {'A',{{0,1},{1,0},{1,1},{1,2},{2,1}}},
  {'B',{{0,0},{0,2},{1,0},{1,1},{1,2},{2,0},{2,2}}},
  {'C',{{0,1},{1,0},{1,1},{1,2},{2,1}}},
  {'D',{{0,0},{0,1},{0,2},{1,1},{2,0},{2,1},{2,2}}},
  {'E',{{0,0},{0,1},{0,2},{1,0},{1,2},{2,0},{2,1},{2,2}}},
  {'F',{{0,0},{0,1},{0,2},{1,1},{2,0},{2,1},{2,2}}},
  {'G',{{0,1},{1,0},{1,1},{1,2},{2,1}}},
  {'H',{{0,0},{0,2},{1,0},{1,1},{1,2},{2,0},{2,2}}},
  {'I',{{0,1},{1,0},{1,1},{1,2},{2,1}}}};
  map<char,vector<pair<int,int>>> extra {
    {'A',{{0,2},{2,0},{2,2}}},
    {'B',{{2,1}}},
    {'C',{{0,0},{2,0},{2,2}}},
    {'D',{{1,2}}},
    //{'E',{{}}},
    {'F',{{1,0}}},
    {'G',{{0,0},{0,2},{2,2}}},
    {'H',{{0,1}}},
    {'I',{{0,0},{0,2},{2,0}}}
  };

struct Vertex {
    int w;
    int visited;
    vector<Vertex> edge;
    //Vertex *next;
    //struct edge *Ve;
};

const vector<pair<int,int>> Ve{{-1,-1},{-1,0},{-1,1},{0,-1},{0,0},{0,1},{1,-1},{1,0},{1,1}};
/////////////////////////////////fund def/////////////////////////////////////////
unsigned int countPatternsFrom(char firstDot, unsigned short length) ;
////////////////////////////////////////////////////////////////////////////////

bool is_valid (Vertex *v, int dot) {
    int ax = coord[v->w].first, ay = coord[v->w].second;
    int bx = coord[dot].first, by = coord[dot].second;
    if (ax == bx && ay == by) return false;

    if (((bx == ax) && abs(by - ay) > 1) || ((by == ay) && abs(bx - ax) > 1))
        return false;

    if ((abs(bx - ax) > 1) && (abs(by - ay) > 1))
        return false;

    return true;
}
Vertex newvtx (int i, int j) {
    Vertex next;
    next.w = keypad[i][j];
    next.visited = false;

    return next;
}
vector<Vertex> scan_pad (Vertex *ve) {

    vector<Vertex> nextkey;

    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        //Vertex nextv = newvtx(i, j);

        if (is_valid (ve, keypad[i][j])) {
            nextkey.push_back({keypad[i][j]});
            //cout << &now->edge.back() << " ";
        }
        //cout << coord[keypad[i][j]].first << " ";
      }

      //cout << "\n";
    }


    return nextkey;
}
int main () {

    auto start = std::chrono::high_resolution_clock::now();

    int total = 1;

    Vertex aleph {.w = 'D'};

    vector<Vertex> comb;

    scan_pad (&aleph);


    //display(comb);




    //countPatternsFrom('D', 3);

    cout << endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Process took " << elapsed.count()  << " ms" << std::endl;
}
unsigned int countPatternsFrom(char firstDot, unsigned short length) {

    if (length == 0 || length > 9) return 0;
    if (length == 1) return 1;

    unsigned cnt = 0;

    return cnt;
}

void Test () {

    Assert::That(countPatternsFrom('A', 0), Equals(0));
    Assert::That(countPatternsFrom('A', 10), Equals(0));
    Assert::That(countPatternsFrom('B', 1), Equals(1));
    Assert::That(countPatternsFrom('C', 2), Equals(5));
    Assert::That(countPatternsFrom('D', 3), Equals(37));
    Assert::That(countPatternsFrom('E', 4), Equals(256));
    Assert::That(countPatternsFrom('E', 8), Equals(23280));
}
