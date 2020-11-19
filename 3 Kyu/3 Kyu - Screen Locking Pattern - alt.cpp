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

unsigned int countPatternsFrom(char dot, unsigned short length) {
    if (length < 0 || length > 9) return 0;

    const vector<int> fiv {0,1,5,31,154,684,2516,7104,13792,13792};
    const vector<int> sev {0,1,7,37,188,816,2926,8118,15564,15564};
    const vector<int> eig {0,1,8,48,256,1152,4248,12024,23280,23280};

    if (dot == 'E') return eig[length];
    if ((dot - 'A') % 2 == 0) return fiv[length];
    else return sev[length];

    return 0;
}
int main () {

    //int num = 8 * 7 * 5;

    const vector<int> fiv {0,1,5,31,154,684,2516,7104,13792,13792};
    const vector<int> sev {0,1,7,37,188,816,2926,8118,15564,15564};
    const vector<int> eig {0,1,8,48,256,1152,4248,12024,23280,23280};



    map<char,vector<int>> table {
      {'A',fiv},{'B',sev},{'C',fiv},
      {'D',sev},{'E',eig},{'F',sev},
      {'G',fiv},{'H',sev},{'I',fiv}};

      Test();
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
