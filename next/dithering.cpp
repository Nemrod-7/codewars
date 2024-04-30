#include <iostream>
#include <iomanip>
#include <queue>

using namespace std;

unsigned bayer(unsigned x, unsigned y, unsigned order) {
  	unsigned res = 0;

  	for (unsigned i = 0; i < order; ++i) {
    		uint xOdd_XOR_yOdd = (x & 1) ^ (y & 1);
    		uint xOdd = x & 1;
    		res = ((res << 1 | xOdd_XOR_yOdd) << 1) | xOdd;
    		x >>= 1, y >>= 1;
  	}

  	return res + 1;
}

unsigned scale (int x) {
    return (x / 2.0) + 1;
}

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
                }
            }
        }
        // cout << "\n";
    }
}
void bit () {

      for(unsigned sq = 3; sq < 4; ++sq) {
          const unsigned size = 1 << sq;
          unsigned width = 1 << sq, height = 1 << sq;
          // printf(" width = %u, height = %u: s2 : %u\n", width, height, sq);

          for (unsigned y = 0; y < width; y++) {
              for (unsigned x = 0; x < height; x++) {
                  unsigned cell = 0, mask = sq - 1, xc = x ^ y, yc = y;

                  for (unsigned bit = 0; bit < (2 * sq); mask--) {
                      cell |= ((yc >> mask) &1) << bit++;
                      cell |= ((xc >> mask) &1) << bit++;
                  }

              }

              // printf("\n");
          }
      }

}

void show (const vector<vector<int>> &mat) {
  for (int y = 0; y < mat.size(); y++) {
      for (int x = 0; x < mat[y].size(); x++) {
          cout << setw(2) << mat[y][x] << ' ';
      }
      cout << endl;
  }
}
void arkive () {
  int width = 4, height = 4;

    vector<vector<int>> mat (height, vector<int> (width));
    vector<vector<int>> kernel = {{0,2},{3,1}};

    int size = 2;
    for (int y = 0; y < height; y += size) {
        for (int x = 0; x < width; x += size) {

            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    int nx = j * (size - 1) + x, ny = i * (size - 1) + y;
                    if (nx < width && ny < height) {
                        mat[ny][nx] += kernel[i][j];
                    }
                }
            }
        }
    }
}

int main () {

    int width = 8, height = 8;
    unsigned order = 2;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            cout << setw(2) << 1.0 / 64.0 * (x + y * order) << " ";
        }
        cout << endl;
    }



    // show(mat);

    cout << "\nexit\n";
}
