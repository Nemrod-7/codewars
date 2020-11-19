#include <vector>
#include <cmath>

using namespace std;

inline bool is_even (int n) { return n % 2 == 0 ? true : false; }

int mobius(int num){
    if (num == 1) return 1;
    int cnt = 0;

    for (int index = 2; index <= sqrt(num); index++)
          if (num % index == 0) {
              num /= index;
              cnt++;

              if (num % index == 0) return 0;
          }

    return is_even (cnt) ? -1 : 1;
}
