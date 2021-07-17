#include <iostream>

using namespace std;

template<class T> class MagicCompare {
  private :
      T value;
      bool flag;
  public:
      MagicCompare (const T& x = 0) : value(x), flag(true) {}

      bool operator< (const T& rhs) {
          if (value >= rhs) return false;
          return true;
      }
      bool operator> (const T& rhs) {
          if (value <= rhs) return false;
          return true;
      }
      bool operator== (const T& rhs) {
          if (value != rhs) return false;
          return true;
      }
      bool operator!= (const T& rhs) { return (rhs == value) == false;}
      bool operator<= (const T& rhs) { return (value > rhs) == false; }
      bool operator>= (const T& rhs) { return (value < rhs) == false; }

      friend bool operator< (const T& lhs, MagicCompare& rhs) { return rhs > lhs; }
      friend bool operator<= (const T& lhs, MagicCompare& rhs) { return rhs >= lhs; }
      friend bool operator>= (const T& lhs, MagicCompare& rhs) { return rhs <= lhs; }
      friend bool operator> (const T& lhs, MagicCompare& rhs) { return rhs < lhs; }
      friend bool operator== (const T& lhs, MagicCompare& rhs) { return rhs == lhs; }
      friend bool operator!= (const T& lhs, MagicCompare& rhs) { return rhs != lhs; }

      friend ostream &operator<< (ostream &os, const MagicCompare& obj) {
          os << obj.value;
          return os;
      }
};
void check (bool x) {cout << (x ? "true " : "false") ;}
int main () {

    using Integer = MagicCompare<int>;

    Integer a = 3;
    //cout << a;

    for (int i = 0; i < 5; i++) {
        check (i == a);

        cout << "  " << i << "<=" << a << endl;
    }
    //cout << a;
    //bool res_5gtalt7neqa = (5 > a < 7 != a);



}
