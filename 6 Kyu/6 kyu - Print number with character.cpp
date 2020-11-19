#include <iostream>
#include <vector>
#include <map>

using namespace std;

bool is_border (int x, int y) {   return y == 0 || y == 9 || x == 0 || x == 39; }
bool is_inside (int x, int y) { return (y > 1 && y < 8 && x > 2 && x < 37 && (x % 7 != 2)); }
/*
**************************************************************************

    **    ****   ****  **  ** ******    **  ******  ****   ****   ****
   ***   **  ** **  ** **  ** **       **   **  ** **  ** **  ** **  **
  * **      **     **  **  ** *****   ****     **   ****  **  ** **  **
    **     **      **   *****     ** **  **   **    ****   ****  **  **
    **    **    **  **     **     ** **  **  **    **  **   **   **  **
  ****** ******  ****      ** *****   ****   **     ****   **     ****

**************************************************************************
*/
std::string print_number(unsigned num, char ch) {

    const vector<vector<int>> numbers {
        {0,1,1,1,1,0},{2,3,4,2,2,5},{0,1,6,2,7,5},{0,1,6,6,1,0},
        {1,1,1,8,9,9},{5,10,11,9,9,11},{6,2,0,1,1,0},{5,1,6,2,7,7},
        {0,1,0,0,1,0},{0,1,1,0,2,7}};

    const vector<vector<int>> patterns = {
        {0,1,1,1,1,0},{1,1,0,0,1,1},{0,0,1,1,0,0},
        {0,1,1,1,0,0},{1,0,1,1,0,0},{1,1,1,1,1,1},
        {0,0,0,1,1,0},{0,1,1,0,0,0},{0,1,1,1,1,1},
        {0,0,0,0,1,1},{1,1,0,0,0,0},{1,1,1,1,1,0}};

    const vector<string> patterns2 = {
        " **** ","**  **","  **  "," ***  ","* **  ","*****",
        "   * "," *   "," ***","    *","*    ","*** "};


    const int nlines = 10, nrows = 40;

    string os, digits = "00000" + to_string (num);
    digits = digits.substr (digits.size () - 5);
    //  std::fill_n(std::back_inserter(res), 5 - res.size(), 0);
    for (int y = 0; y < nlines; y++) {
        int index = 0;
        for (int x = 0; x < nrows; x++) {
            if (is_border (x, y)) {
                os += ch;

            } else if (is_inside (x,y)) {
                int mod = (x - 3) % 7, line = y - 2;
                vector<int> actual = numbers[(digits[index]  - '0')];

                os += (patterns[actual[line]][mod] ? ch : ' ');

                if (mod == 5) index++;
            } else {
                os += ' ';

            }
        }
        if (y < 9) os += '\n';
    }

    return os;
}

int main () {
    unsigned num = 12345;
    char ch = '*';
    const vector<vector<int>> numbers {
        {0,1,1,1,1,0},{2,3,4,2,2,5},{0,1,6,2,7,5},{0,1,6,6,1,0},
        {1,1,1,8,9,9},{5,10,11,9,9,11},{6,2,0,1,1,0},{5,1,6,2,7,7},
        {0,1,0,0,1,0},{0,1,1,0,2,7}};

    const vector<string> patterns2 = {
        " **** ","**  **","  **  "," ***  ","* **  ","******",
        "   ** "," **   "," *****","    **","**    ","****  "};

        string os, digits = "00000" + to_string (num);
        digits = digits.substr (digits.size () - 5);




    //cout << print_number (67890, '*');
}
