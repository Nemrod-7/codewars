#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

void recurse (int width, int height) {

}

int main () {

  /*
    2x2 : 1 3 4 2
    1 3
    4 2

    4×4 : 1 9 3 11 13 5 15 7 4 12 2 10 16 8 14 6
     1  9  3 11
    13  5 15  7
     4 12  2 10
    16  8 14  6

    8×8 :
    vector<vector<int>> grid = {
    { 1, 33,  9, 41,  3, 35, 11, 43},
    {49, 17, 57, 25, 51, 19, 59, 27},
    {13, 45,  5, 37, 15, 47,  7, 39},
    {61, 29, 53, 21, 63, 31, 55, 23},
    { 4, 36, 12, 44,  2, 34, 10, 42},
    {52, 20, 60, 28, 50, 18, 58, 26},
    {16, 48,  8, 40, 14, 46,  6, 38},
    {64, 32, 56, 24, 62, 30, 54, 22} };
    ref = {1,33,9,41,3,35,11,43, 49,17,57,25,51,19,59,27, 13,45,5,37,15,47,7,39, 61,29,53,21,63,31,55,23, 4,36,12,44,2,34,10,42, 52,20,60,28,50,18,58,26, 16,48,8,40,14,46,6,38, 64,32,56,24,62,30,54,22};

    3x3 :
    1 6 3
    8 5 9
    4 7 2

    // 1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
    // 8,-6, 8, 2,-8,10,-8,-3, 8,-10, 8, 6,-8, 6,-8
    // +     +     -     -     +      +     -     -
    */

    int size = 4, s2 = size * size;
    vector<int> ref = {1,9,3,11,13,5,15,7,4,12,2,10,16,8,14,6}; // 4x4

    const vector<vector<int>> seq = {{0,2},{3,1}};
    vector<vector<int>> mat (size, vector<int> (size));

    for (int x = 0; x < 2; x++) {
        for (int y = 0; y < 2; y++) {
            int nx = x * size / 2, ny = y * size / 2;
            mat[nx][ny] = seq[x][y] + 1;
         }
    }

    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            cout << mat[x][y] << ' ';
        }
        cout << endl;
    }


}

void arkive() {

    // int sign = 1;
    // for (int i = 0; i < size; i++) {
    //     for (int j = 0; j < size; j++) {
    //         // M(i, j) = bit_reverse(bit_interleave(bitwise_xor(i, j), i)) / n ^ 2;
    //         // M(i, j) = bit_reverse(bit_interleave(i^j, i)) / s2;
    //         cout << xor(i,j) << ' ';
    //     }
    //     cout << endl;
    // }
    // for (int i = 1; i < s2; i += size) {
    //     for (int j = 0; j < size; j += 2) {
    //       seq[i + j] = ((size * size) / 2) * sign;
    //       // cout << i + j << " ";
    //     }
    //     sign *= -1;
    // }
    //
    //
    // for (int i = 1; i < ref.size(); i++) {
    //     // cout << ref[i-0] - ref[i-1] << " " ;
    //     cout << seq[i] << " ";
    // }

}
