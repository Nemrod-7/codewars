#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

int64_t determinant (vector<vector<int64_t>> mat) {

    int64_t det = 0;

    switch (mat.size()) {
        case 0 : det = 0; break;
        case 1 : det = mat[0][0]; break;
        case 2 : det = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]; break;
        default : {

            short sign = 1;

            for (int dig = 0; dig < mat.size(); ++dig) {
                vector<vector<int64_t>> sub;

                for (int i = 0; i < mat.size(); ++i) {
                    vector<int64_t> v;
                    for (int j = 0; j < mat.size(); ++j) {
                        if (i != 0 && j != dig)
                            v.push_back(mat[i][j]);
                    }

                    if (i != 0) sub.push_back(v);
                }

                det += sign * mat[0][dig] * determinant (sub);
                sign *= -1;
            }
        } break;
    }

    return det;
}

int main () {

    //Test();
    vector<vector<int64_t>> matrix {
      {2,5,3},
      {1,-2,-1},
      {1,3,4}
    };
    vector<vector<int64_t>> tab {
      {1,3},
      {2,5}};

}
