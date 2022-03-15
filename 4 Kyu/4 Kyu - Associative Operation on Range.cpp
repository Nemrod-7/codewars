#include <iostream>
#include <iomanip>
#include <vector>

#include <functional>

using namespace std;
using range_t = pair<size_t, size_t>;

template<class T> using op_t = T(const T&, const T&);
/////////////////////////////////ops///////////////////////////////////////////
/*
Matrix mmult(const Matrix& A, const Matrix& B) {
  short n = A.getSize()[0];
  short k = A.getSize()[1];
  short m = B.getSize()[1];
  Matrix C (n,m);
  for(short i = 0; i < n; i++) {
    for(short j = 0; j < m; j++) {
      int cij = 0;
      for(short t = 0; t < k; t++) {
          cij += A.get(i,t) * B.get(t, j);
      }
      C.set(i,j, cij);
    }
  }
  return C;
}
*/
long sum (const long & a, const long& b) { return a + b;}

int max (const int& a, const int& b) {  return a > b ? a : b;}
int min (const int& a, const int& b) { return a < b ? a : b; }
int gcd (const int& a, const int& b) { return b == 0 ? a : gcd(b, a%b); }
long lcm (const long& a, const long& b) {
  if(a == 0) return b;
  if(b == 0) return a;
  return a * b / gcd(a,b);
}
int modSum (const int& a, const int& b) { return (a+b)%1000000; }
///////////////////////////////////////////////////////////////////////////////

template<class T> vector<T> computeRanges (vector<T> arr, op_t<T> op, vector<range_t> ranges) {

    int size = arr.size();
    T opa = arr[0];
    std::vector<T> res;
    std::vector<T> first {opa};
    T base[size][size] = {};

    for (int i = 0; i < size - 1; i++) {
        base[i][i] = arr[i];
        for (int j = i + 1; j < size; j++) {
            base[i][j] = op (base[i][j - 1], arr[j]);
            //cout << base[i][j]  << " ";
        }
        //cout << endl;
    }
    /*
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << setw(2) << base[i][j]  << " ";
        }
        cout << endl;
    }
    */
    for (auto [start, end] : ranges) {

        opa = arr[start];

        for (int i = start + 1; i < end; i++) {
            opa = op (opa, arr[i]);
        }

        res.push_back (opa);
    }

    return res;
}

int main () {

    computeRanges ({1,5,8,5,1,8}, sum, {{0,4},{0,6},{2,4},{1,4}});

    std::vector<long> expected = {19, 28, 13, 18};
    return 0;
}
