#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
#include <chrono>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
typedef long long test_t;
class Assert {
    public :
        static void That (test_t actual, test_t expression) {
            if (actual != expression)
                std::cout<<"actual : "<<actual<<" expected : "<<expression<<std::endl;
        }
};
test_t Equals (test_t entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////
typedef vector<vector<long long>> Matrix;
typedef pair<int,int> Point;

long long determinant(vector<vector<long long>> m);
void display_sub (Matrix actual, Point p);
void display (Matrix actual);

int main () {

    auto start = std::chrono::high_resolution_clock::now();

    //Test();
    vector<vector<long long>> matrix {
      {2,5,3},
      {1,-2,-1},
      {1,3,4}
    };
    vector<vector<long long>> tab {
      {1,3},
      {2,5}};
    //cout << matrix.size();

    /*
    */

    //display (M);
    Test();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Process took " << elapsed.count()  << " ms" << std::endl;

}

long long compute (vector<vector<long long >> m) {

    return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}


long long determinant(vector<vector<long long>> actual) {

    if (actual.size() == 1) return actual[0][0];
    if (actual.size() == 2) { return actual[0][0] * actual[1][1] - actual[0][1] * actual[1][0];}

    long long det = 0;
    short sign;

    for (int dig = 0; dig < actual.size(); ++dig) {
        vector<vector<long long>> sub;
        sign = dig % 2 ? -1 : 1;

        for (int i = 0; i < actual.size(); ++i) {
            std::vector<long long> v;
            for (int j = 0; j < actual.size(); ++j) {
                if (i != 0 && j != dig)
                    v.push_back(actual[i][j]);

            }

            if (i != 0 /* && j != p.second */)
                sub.push_back(v);

        }

        det += sign * actual[0][dig] * determinant (sub);
    }
    return det;
}
void display_sub (Matrix actual, Point p) {

    for (int i = 0; i < actual.size(); ++i) {
        for (int j = 0; j < actual.size(); ++j) {
            if ( i != p.first && j != p.second) {
                cout << actual[i][j] << " ";
            } else
                cout << "." << " ";
        }
        cout << endl;
    }
}
void display (Matrix actual) {

    for (int i = 0; i < actual.size(); ++i) {
        for (int j = 0; j < actual.size(); ++j) {
            //if (i != p.first && j != p.second)
                cout << actual[i][j] << " ";
        }
        cout << endl;
    }
    //cout << endl;
}
void Test() {

    Assert::That(determinant(vector<vector<long long>> {vector<long long> {1}}), Equals(1));
    Assert::That(determinant(vector<vector<long long>> {vector<long long> {1, 3}, vector<long long> {2, 5}}), Equals(-1));
    Assert::That(determinant(vector<vector<long long>> {
        vector<long long> {2, 5, 3},
        vector<long long> {1,-2,-1},
        vector<long long> {1, 3, 4}
      }), Equals(-20));

}
