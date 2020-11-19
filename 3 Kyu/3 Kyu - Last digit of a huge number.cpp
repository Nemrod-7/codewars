#include <iostream>
#include <list>
#include <cmath>

using namespace std;
int last_digit(list<int> array) ;

int main () {
    const unsigned long int arr11[] = { 499942, 898102, 846073 };
    last_digit({499942,898102,846073});

}
int last_digit(list<int> array) {

    unsigned long int num, div ,exp;
    if (array.size() == 0) return num;
    auto actual = array.end();
    //cout << array[index];

    while (actual-- != array.begin())  {
        cout << *actual <<endl;
        /*
        exp = (actual % 100 > 1) ? actual % 100 : actual % 1000;
        div = num < 4 ? num : num % 4 + 4;
        num = pow (exp, div);
        */
      //  actual--;
    }

    return num % 10;
}
