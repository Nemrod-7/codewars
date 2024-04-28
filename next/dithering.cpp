#include <iostream>
#include <iomanip>
#include <queue>

using namespace std;

void search () {
  int width = 6, height = 5;
  vector<int> ref = {1,9,3,11,13,5,15,7,4,12,2,10,16,8,14,6}; // 4x4

  vector<vector<int>> mat (height, vector<int> (width));
  vector<vector<bool>> visit (height, vector<bool> (width));

  queue<pair<int,int>> s1 ;
  vector<pair<int,int>> order = {{0,0},{1,1},{1,0},{0,1}};
  s1.push({0,0});
  int index = 1;

  int cycle = 0;
  int sx = 4, sy = 4;

  while (!s1.empty()) {
      auto [cx, cy] = s1.front();
      s1.pop();

      // cout << "[" << cx << "," << cy << "] : ";

      if (!visit[cy][cx]) {
          visit[cy][cx] = true;

          for (int i = 0; i < 4; i++) {
              auto [dx,dy] = order[i];
              int nx = dx * sx + cx, ny = dy * sy + cy;

              if (nx < width && ny < height) {
                  mat[ny][nx] = index++;
                  // cout << "[" << nx << "," << ny << "]";
                  s1.push({nx / 2, ny / 2});
              } else {

              }

          }
      }

      // cout << "\n";
  }
}
int main () {

    int width = 6, height = 5;
    vector<int> ref = {1,9,3,11,13,5,15,7,4,12,2,10,16,8,14,6}; // 4x4

    vector<vector<int>> mat (height, vector<int> (width));
    vector<vector<bool>> visit (height, vector<bool> (width));

    queue<pair<int,int>> s1 ;
    vector<pair<int,int>> order = {{0,0},{1,1},{1,0},{0,1}};
    s1.push({0,0});
    int index = 1;



    
        for (int y = 0; y < height; y++) {
          for (int x = 0; x < width; x++) {
            cout << setw(2) <<  mat[y][x] << ' ';
        }
        cout << endl;
    }
    cout << "\nexit\n";

}
