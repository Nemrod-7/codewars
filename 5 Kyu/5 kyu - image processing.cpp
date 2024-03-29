#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

typedef unsigned char u8;

vector <vector <float>> kernel_identity = {{1}};
vector <vector <float>> kernel_blur = {
  {1.0/9,1.0/9,1.0/9},
  {1.0/9,1.0/9,1.0/9},
  {1.0/9,1.0/9,1.0/9}
};
vector <vector <float>> kernel_sharpen = {
  { 0,-1, 0},
  {-1, 5,-1},
  { 0,-1, 0}
};
vector <vector <float>> kernel_edgeDetect = {
  {-1,-1,-1},
  {-1, 8,-1},
  {-1,-1,-1}
};
vector <vector <float>> kernel_gaussianBlur = {
    {0.0008886514773351066, 0.0013566613118620517, 0.00191780367098833, 0.0025103228719970316, 0.003042620258895748, 0.0034147479611360866, 0.0035486428168826595, 0.0034147479611360866, 0.003042620258895748, 0.0025103228719970316, 0.00191780367098833, 0.0013566613118620517, 0.0008886514773351066},
    {0.0013566613118620517, 0.0020711493336202577, 0.0029278182848232145, 0.003832388745848613, 0.004645021470408228, 0.005213130869399541, 0.005417541682054079, 0.005213130869399541, 0.004645021470408228, 0.003832388745848613, 0.0029278182848232145, 0.0020711493336202577, 0.0013566613118620517},
    {0.00191780367098833, 0.0029278182848232145, 0.0041388227153865064, 0.005417541682054079, 0.00656629561842648, 0.007369386471966865, 0.007658345701120769, 0.007369386471966865, 0.00656629561842648, 0.005417541682054079, 0.0041388227153865064, 0.0029278182848232145, 0.00191780367098833},
    {0.0025103228719970316, 0.003832388745848613, 0.005417541682054079, 0.007091330046025539, 0.008594999751322403, 0.009646211284823689, 0.010024446540597216, 0.009646211284823689, 0.008594999751322403, 0.007091330046025539, 0.005417541682054079, 0.003832388745848613, 0.0025103228719970316},
    {0.003042620258895748, 0.004645021470408228, 0.00656629561842648, 0.008594999751322403, 0.01041751268743106, 0.011691626684437127, 0.012150064228341442, 0.011691626684437127, 0.01041751268743106, 0.008594999751322403, 0.00656629561842648, 0.004645021470408228, 0.003042620258895748},
    {0.0034147479611360866, 0.005213130869399541, 0.007369386471966865, 0.009646211284823689, 0.011691626684437127, 0.013121571207027808, 0.013636077959481905, 0.013121571207027808, 0.011691626684437127, 0.009646211284823689, 0.007369386471966865, 0.005213130869399541, 0.0034147479611360866},
    {0.0035486428168826595, 0.005417541682054079, 0.007658345701120769, 0.010024446540597216, 0.012150064228341442, 0.013636077959481905, 0.014170758911667438, 0.013636077959481905, 0.012150064228341442, 0.010024446540597216, 0.007658345701120769, 0.005417541682054079, 0.0035486428168826595},
    {0.0034147479611360866, 0.005213130869399541, 0.007369386471966865, 0.009646211284823689, 0.011691626684437127, 0.013121571207027808, 0.013636077959481905, 0.013121571207027808, 0.011691626684437127, 0.009646211284823689, 0.007369386471966865, 0.005213130869399541, 0.0034147479611360866},
    {0.003042620258895748, 0.004645021470408228, 0.00656629561842648, 0.008594999751322403, 0.01041751268743106, 0.011691626684437127, 0.012150064228341442, 0.011691626684437127, 0.01041751268743106, 0.008594999751322403, 0.00656629561842648, 0.004645021470408228, 0.003042620258895748},
    {0.0025103228719970316, 0.003832388745848613, 0.005417541682054079, 0.007091330046025539, 0.008594999751322403, 0.009646211284823689, 0.010024446540597216, 0.009646211284823689, 0.008594999751322403, 0.007091330046025539, 0.005417541682054079, 0.003832388745848613, 0.0025103228719970316},
    {0.00191780367098833, 0.0029278182848232145, 0.0041388227153865064, 0.005417541682054079, 0.00656629561842648, 0.007369386471966865, 0.007658345701120769, 0.007369386471966865, 0.00656629561842648, 0.005417541682054079, 0.0041388227153865064, 0.0029278182848232145, 0.00191780367098833},
    {0.0013566613118620517, 0.0020711493336202577, 0.0029278182848232145, 0.003832388745848613, 0.004645021470408228, 0.005213130869399541, 0.005417541682054079, 0.005213130869399541, 0.004645021470408228, 0.003832388745848613, 0.0029278182848232145, 0.0020711493336202577, 0.0013566613118620517},
    {0.0008886514773351066, 0.0013566613118620517, 0.00191780367098833, 0.0025103228719970316, 0.003042620258895748, 0.0034147479611360866, 0.0035486428168826595, 0.0034147479611360866, 0.003042620258895748, 0.0025103228719970316, 0.00191780367098833, 0.0013566613118620517, 0.0008886514773351066}
};
vector <vector <float>> kernel_laplacianFilter = {
  {    0,     0, 0.075,  0.05,  0.05,  0.05, 0.075,     0,     0},
  {    0,  0.05, 0.075, 0.125, 0.125, 0.125, 0.075,  0.05,     0},
  {0.075, 0.075, 0.125, 0.075,     0, 0.075, 0.125, 0.075, 0.075},
  { 0.05, 0.125, 0.075,  -0.3,-0.575,  -0.3, 0.075, 0.125,  0.05},
  { 0.05, 0.125,     0,-0.575,  -0.9,-0.575,     0, 0.125,  0.05},
  { 0.05, 0.125, 0.075,  -0.3,-0.575,  -0.3, 0.075, 0.125,  0.05},
  {0.075, 0.075, 0.125, 0.075,     0, 0.075, 0.125, 0.075, 0.075},
  {    0,  0.05, 0.075, 0.125, 0.125, 0.125, 0.075,  0.05,     0},
  {    0,     0, 0.075,  0.05,  0.05,  0.05, 0.075,     0,     0}
};

struct Pixel {
    float R;
    float G;
    float B;
};
struct Image {
    int row;
    int col;
    vector<vector<Pixel>> data;
};
vector<u8> processImage (const vector<u8>&, int , int , vector<vector<float>> );

int main () {

  processImage({0,1,2, 3,4,5, 6,7,8, 9,10,11},
             2, 2,
             {{0.2,0,0},{0,0.2,0.2},{0,0.2,0.2}});
             /*
 processImage({0,0,0, 0,0,0, 0,0,0, 255,255,255},
            2, 2,
            {{0.2,0,0},{0,0.2,0.2},{0,0.2,0.2}});
*/

}
Image init (const vector<u8> &img, int height, int width) {

    Image out {.row = height, .col = width};
    int next = 0;
    for (int i = 0; i < out.row; ++i) {
        out.data.push_back(vector<Pixel>(out.col));
        for (int j = 0; j < out.col; ++j) {
            out.data[i][j].R = img[next++];
            out.data[i][j].G = img[next++];
            out.data[i][j].B = img[next++];
        }
    }
    return out;
}
int limit (int pos, int size) {
    if (pos < 0) return 0;
    if (pos >= size) return size - 1;
    return pos;
}
Pixel filter (Image input, int x, int y, vector<vector<float>> kernel, int size) {

    const int center = size * 0.5;
    const int startx = x - center, endx = x + center, starty = y - center, endy = y + center;
    Pixel conv = {0}, cell;
    double R = 0, G = 0, B = 0, k;

    for (int i = startx; i <= endx; ++i) {
        for (int j = starty; j <= endy; ++j) {
            k = kernel[i - startx][j - starty];
            cell = input.data[limit(i, input.row)][limit (j, input.col)];

            R += (k * cell.R);
            G += (k * cell.G);
            B += (k * cell.B);
        }
    }

    conv.R = limit (round (R), 256);
    conv.G = limit (round (G), 256);
    conv.B = limit (round (B), 256);

    return conv;
}
vector <u8> processImage (const vector<u8> &imageData, int height, int width, vector<vector<float>> weights) {

    vector<u8> out;
    int size = weights.size(), next = 0;;
    Image actual = init (imageData,height,width);

    for (int x = 0; x < height; ++x)
          for (int y = 0; y < width; ++y) {
              Pixel altered = filter (actual, x, y, weights, size);

              out.push_back(altered.R);
              out.push_back(altered.G);
              out.push_back(altered.B);
          }

    return out;
}


bool RGBImageDataSimilar (const vector <u8> &actual, const vector <u8> &expected, int h, int w) {
  for (int i = 0, s = 3*w*h; i < s; ++i)
    if (abs (actual[i] - expected[i]) > 3)
      return false;
  return true;
}
void imageTest (const vector <u8> &imageData, int h, int w, vector <vector <float>> weights, const vector <u8> &expected) {
    const int TILE_SIZE = 36;
    const vector <u8> original (imageData);
    auto actual = processImage (original, h, w, weights);

    //putImageData (original, h, w, TILE_SIZE, TILE_SIZE);
    cout << " → \n";
    //putImageData (actual, h, w, TILE_SIZE, TILE_SIZE);
    cout << ", expected: \n";
    //putImageData (expected, h, w, TILE_SIZE, TILE_SIZE);

    //Assert::That (original == imageData);
    //Assert::That (RGBImageDataSimilar (actual, expected, h, w));
}
void Test() {


imageTest ({127,127,127},
           1, 1,
           kernel_identity,
           {127,127,127});

imageTest ({127,255,0},
           1, 1,
           kernel_blur,
           {127,255,0});


imageTest ({0,0,0,0,0,0,0,0,0,255,255,255},
           2, 2,
           {{0.2,0,0},{0,0.2,0.2},{0,0.2,0.2}},
           {51,51,51,102,102,102,102,102,102,204,204,204});



}
