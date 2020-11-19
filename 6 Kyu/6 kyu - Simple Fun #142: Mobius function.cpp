#include <iostream>
#include <cmath>


using namespace std;

bool is_even (int n) { return n % 2 == 0 ? true : false;}

int mobius(long long int num){
    if (num == 1) return 1;
    size_t cnt = 0;

    for (long long int index = 2; index <= sqrt(num); index++)
          if (num % index == 0) {
              num /= index;
              cnt++;

              if (num % index == 0) return 0;
          }

    return is_even(cnt) ? -1 : 1;
}

int main () {

    for (int i = 0; i < 40; ++i)
        if (is_prime (i) )
        cout << i << " ";


    return 0;
}

void Test (){
  /*
        Assert::That(mobius(10), Equals(1));
        Assert::That(mobius(9), Equals(0));
        Assert::That(mobius(8), Equals(0));
        Assert::That(mobius(7), Equals(-1));
        Assert::That(mobius(5), Equals(-1));
        */
}
