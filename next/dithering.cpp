#include <iostream>
#include <iomanip>
#include <vector>
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

int scale (int width,int height)  {
    int x = std::min(width, height) - 1;

    while (x &x - 1) {
        x = x &x - 1;
    }
    // std::cout << width / 2.0  << " " << height / 2.0  << "\n";
    return x;
}

int main () {
  const   int BAYER_PATTERN_16X16[16][16] =
    {   //  16x16 Bayer Dithering Matrix.  Color levels: 256
    {     0, 191,  48, 239,  12, 203,  60, 251,   3, 194,  51, 242,  15, 206,  63, 254  },
    {   127,  64, 175, 112, 139,  76, 187, 124, 130,  67, 178, 115, 142,  79, 190, 127  },
    {    32, 223,  16, 207,  44, 235,  28, 219,  35, 226,  19, 210,  47, 238,  31, 222  },
    {   159,  96, 143,  80, 171, 108, 155,  92, 162,  99, 146,  83, 174, 111, 158,  95  },
    {     8, 199,  56, 247,   4, 195,  52, 243,  11, 202,  59, 250,   7, 198,  55, 246  },
    {   135,  72, 183, 120, 131,  68, 179, 116, 138,  75, 186, 123, 134,  71, 182, 119  },
    {    40, 231,  24, 215,  36, 227,  20, 211,  43, 234,  27, 218,  39, 230,  23, 214  },
    {   167, 104, 151,  88, 163, 100, 147,  84, 170, 107, 154,  91, 166, 103, 150,  87  },
    {     2, 193,  50, 241,  14, 205,  62, 253,   1, 192,  49, 240,  13, 204,  61, 252  },
    {   129,  66, 177, 114, 141,  78, 189, 126, 128,  65, 176, 113, 140,  77, 188, 125  },
    {    34, 225,  18, 209,  46, 237,  30, 221,  33, 224,  17, 208,  45, 236,  29, 220  },
    {   161,  98, 145,  82, 173, 110, 157,  94, 160,  97, 144,  81, 172, 109, 156,  93  },
    {    10, 201,  58, 249,   6, 197,  54, 245,   9, 200,  57, 248,   5, 196,  53, 244  },
    {   137,  74, 185, 122, 133,  70, 181, 118, 136,  73, 184, 121, 132,  69, 180, 117  },
    {    42, 233,  26, 217,  38, 229,  22, 213,  41, 232,  25, 216,  37, 228,  21, 212  },
    {   169, 106, 153,  90, 165, 102, 149,  86, 168, 105, 152,  89, 164, 101, 148,  85  }};


    const int width = 6, height = 5;

    const int dir[4][2] = {{0,0},{1,1},{1,0},{0,1}};
    const int dim = scale (6,5);
    int index = 0;
    int visit[64][64] = {{0}};
    std::vector<pair<int,int>> cord;

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            printf("%i ", visit[i][j]);
        }
        printf("\n");
    }




    cout << "\nexit\n";
}
