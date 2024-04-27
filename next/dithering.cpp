#include <iostream>
#include <queue>

using namespace std;

int main () {

    int width = 8, height = 8;
    vector<int> ref = {1,9,3,11,13,5,15,7,4,12,2,10,16,8,14,6}; // 4x4

    vector<vector<int>> mat (width, vector<int> (height));
    vector<vector<bool>> visit (width, vector<bool> (height));

    queue<pair<int,int>> s1 ;
    vector<pair<int,int>> order = {{0,0},{1,1},{0,1},{1,0}};
    s1.push({0,0});
    int index = 1;

    int cycle = 0;

    while (!s1.empty()) {
        auto [sx, sy] = s1.front();
        s1.pop();

        cycle++;
        if (cycle == 3) {
            break;
        }
        cout << sx << " " << sy << "\n";

        if (!visit[sx][sy]) {
            visit[sx][sy] = true;

            for (int i = 0; i < 4; i++) {
              auto [dx,dy] = order[i];
              int nx = dx * width / 2 + sx, ny = dy * height / 2 + sy;

              mat[nx][ny] = index++;
              // cout << nx / 2 << " " << ny / 2 << "\n";
              s1.push({nx / 2, ny / 2});
            }
        }
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            cout <<  mat[x][y] << ' ';
        }
        cout << endl;
    }

}
