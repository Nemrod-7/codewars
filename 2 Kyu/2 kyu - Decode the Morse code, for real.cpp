#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <map>

#include <iomanip>

using namespace std;
/*
    "Dot" – is 1 time unit long.
    "Dash" – is 3 time units long.
    "pause" - is 7 time units long.

    Pause between dots and dashes in a character – is 1 time unit long.
    Pause between characters inside a word – is 3 time units long.
    Pause between words – is 7 time units long.
*/
/////////////////////////////////Assert/////////////////////////////////////////
class Assert {
    public :
        static void That (const string &a, const string &b) {
            if (a != b) {
                cout << "actual : " << a << " expected : " << b;
                cout << endl;
            }
        }
};
string Equals (const string &entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////
map<string, string> morse_code =
{{".-", "A"},{"-...", "B"},{"-.-.", "C"},{"-..", "D"},{".", "E"},{"..-.", "F"},
{"--.", "G"},{"....", "H"},{"..", "I"},{".---", "J"},{"-.-", "K"},{".-..", "L"},{"--", "M"},
{"-.", "N"},{"---", "O"},{".--.", "P"},{"--.-", "Q"},{".-.", "R"},{"...", "S"},{"-", "T"},
{"..-", "U"},{"...-", "V"},{".--", "W"},{"-..-", "X"},{"-.--", "Y"},{"--..", "Z"},{"-----", "0"},

{".----", "1"},{"..---", "2"},{"...--", "3"},{"....-", "4"},{".....", "5"},{"-....", "6"},
{"--...", "7"},{"---..", "8"},{"----.", "9"},

{".-.-.-", "."},{"--..--",","},{"..--..","?"},{".----.","'"},{"-.-.--","!"},{"-..-.","/"},
{"-.--.","("},{"-.--.-",")"},{".-...","&"},{"---...",":"},{"-.-.-.",";"},{"-...-","="},
{".-.-.","+"},{"-....-","-"},{"..--.-","_"},{"...-..-","$"},{".--.-.","@"}};

class Point {
    public :
        float x, y;

        Point () { x = 0, y = 0;}
        Point (float a) {x = a, y = 0;};
        Point (float a, float b) { x = a, y = b;}
        float distance (Point p) { return hypot (p.x - x, p.y - y); }
};
class Kmeans {
    private :
        static bool is_equal (Point a, Point b) { return a.x == b.x && a.y == b.y;}
        static Point nearest_point (vector<Point> &grph, Point p) {
            Point nearest;
            float min = numeric_limits<float>::infinity(), dist;

            for (auto &star : grph) {
                dist = p.distance (star);

                if (dist < min) {
                    min = dist;
                    nearest = star;
                }
            }

            return nearest;
        }
    public :
        static vector<Point> clust (vector<Point> galaxy, vector<Point> ctid) {
          /*
          vector<Point> data = {{5,3},{10,15},{15,12},{24,10},{30,45},{85,70},{71,80},{60,78},{55,52},{80,91}};;
          vector<Point> ctid = {{5,3},{10,15}};

          it 1 => c1 = {5, 3}    c2 = {47.78,50.33}
          it 2 => c1 = {13.5,10} c2 = {63.5,69.33}
          it 3 => c1 = {16.8,17} c2 = {70.2,74.2}
          */
            Point near;
            size_t i;
            //bool done = true;
            //while (done) {
                vector<Point> tmp = ctid;
                map<float, float> npoints, sumX, sumY;
                //done = false;
                for (auto &p : galaxy) {
                    near = nearest_point (ctid, p);
                    for (i = 0; i < ctid.size(); ++i)
                        if (is_equal (ctid[i], near)) {
                            npoints[i]++;
                            sumX[i] += p.x;
                            sumY[i] += p.y;
                            //cout << sumX[i] << ' ' << sumY[i] << endl;
                        }
                }
                for (i = 0; i < ctid.size(); ++i) {
                    ctid[i].x = sumX[i] / npoints[i];
                    ctid[i].y = sumY[i] / npoints[i];

                  //  if (!is_equal (ctid[i], tmp[i])) done = true;
                }
            //}
            return ctid;
        }
};

class Debug {
    public :
        static void display_map (map<float,float> points) {
            for (auto &it : points)
              cout << '[' << it.first << ',' << it.second << ']' <<  endl;
            cout << endl;
        }
        static void display_pt (Point p) {
            cout << setw(2) << fixed << setprecision(2) ;
            cout << '[' << p.x << ", " << p.y << ']';
            cout << endl;
        }
        static void display_vect (vector<Point> points) {
            for (auto &it : points) {
                display_pt (it);
            }
            cout << endl;
        }
};

string clean (string src) {
    unsigned long first = src.find_first_of ('1');
    unsigned long last = src.find_last_of('1') + 1;
    return (first != string::npos)? src.substr(first, last - first) : "";
}
int skip (string::iterator &it) {
    int ref = *it, size = 0;
    while (*it == ref) {
      size++, it++;
    }

    return size;
}

vector<Point> mk_seeds (vector<Point> grph, int mode) {
    float size = grph.size() / (float)7;
    if (size == 0) return {};
    float mid = round (grph[0].x * 3 * size);

    vector<Point> V {grph[0]};

    if (mode > 2) V.push_back (mid);
    V.push_back (grph.back());

    return V;
}
vector<float> mk_unit (string src) {

    map<float,float> mapo, mapz;
    vector<Point> ones, zero;

    vector<float> unit (3);
    int nunits, size;
    string::iterator it = src.begin();

    while (it != src.end()) {
        char bit = *it;
        size = skip (it);

        if (bit == '1') mapo[size]++;
        if (bit == '0') mapz[size]++;
    }

    for (auto &it : mapo) ones.push_back ({it.first,it.second});
    for (auto &it : mapz) zero.push_back ({it.first,it.second});

    if (zero.size() == 0) zero = ones;
    
    nunits = min (static_cast<int> (ones.size()), 2);
    ones = Kmeans::clust (ones, mk_seeds (ones, nunits));

    nunits = min (static_cast<int> (zero.size()), 3);
    zero = Kmeans::clust (zero, mk_seeds (zero, nunits));

    float mid = max (ones.back().x, zero[1].x);

    /*
    cout << ":::cluster:::\n";
    cout << "ones :\n";
    Debug::display_vect(ones);
    cout << "zero :\n";
    Debug::display_vect(zero);
    */
    unit[0] = min (ones.front().x, zero.front().x);

    if (nunits == 1) {
        return {unit[0], unit[0] * 3, unit[0] * 7};

    }
    unit[1] = max (ones.back().x, zero[1].x);
    unit[2] = max (ones.back().x, zero.back().x);

    return unit;
}

int getop (int length, vector<float> &units) {
  if (abs (length - units[0]) <= abs (length - units[1])) return 0;
  if (abs (length - units[2]) < abs (length - units[1])) return 2;
  return 1;
}
string decodeMorse (string morseCode) {

    istringstream iss(morseCode);
    string tok, os;

    while (iss >> tok) {
        os += morse_code[tok];
        if (iss.get() == ' ' && iss.get() == ' ')
            os += ' ';
        else
            iss.unget();
    }
    os.erase(os.find_last_not_of(' ')+1);
    
    return os;
}
string decodeBitsAdvanced (const char *bits) {

  string code = clean (bits), morse;
  string::iterator it = code.begin();

  if (code.size() == 0) return morse;

  vector<float> unit = mk_unit (code);
  int bit, opt;

  while (it != code.end()) {
      bit = *it;
      opt = getop (skip (it), unit);

      if (bit == '1') morse += (opt == 0) ? '.' : '-';
      if (bit == '0') {
          if (opt != 0) morse += ' ';
          if (opt == 2) morse += "  ";
      }
  }

  return morse;
}

int main () {
  cout << decodeMorse(decodeBitsAdvanced("1")) << endl; // E
  cout << decodeMorse(decodeBitsAdvanced("101")) << endl; // I
  cout << decodeMorse(decodeBitsAdvanced("1001")) << endl; // EE
  cout << decodeMorse(decodeBitsAdvanced("10001")) << endl; // EE
  cout << decodeMorse(decodeBitsAdvanced("100001")) << endl; // EE
  cout << decodeMorse(decodeBitsAdvanced("10000001")) << endl; // E E
  // Test();

}

void Test () {

  /*
cout << decodeMorse(decodeBitsAdvanced("1")) << endl;
cout << decodeMorse(decodeBitsAdvanced("101")) << endl;
cout << decodeMorse(decodeBitsAdvanced("1001")) << endl;
cout << decodeMorse(decodeBitsAdvanced("10001")) << endl;
cout << decodeMorse(decodeBitsAdvanced("100001")) << endl;
cout << decodeMorse(decodeBitsAdvanced("10000001")) << endl;

// Operator_is_nervous_his_hand_is_shaking_Figure_out_what_his_message_is
const char *trembling = "00000000000000011111111000000011111111111100000000000111111111000001111111110100000000111111111111011000011111111011111111111000000000000000000011111111110000110001111111111111000111000000000001111111111110000111111111100001100111111111110000000000111111111111011100001110000000000000000001111111111010111111110110000000000000001111111111100001111111111110000100001111111111111100000000000111111111000000011000000111000000000000000000000000000011110001111100000111100000000111111111100111111111100111111111111100000000011110011111011111110000000000000000000000111111111110000000011111000000011111000000001111111111110000000001111100011111111000000000111111111110000011000000000111110000000111000000000011111111111111000111001111111111001111110000000000000000000001111000111111111100001111111111111100100000000001111111100111111110111111110000000011101111111000111000000001001111111000000001111111111000000000111100001111111000000000000011111111100111111110111111111100000000000111111110000001100000000000000000000111111101010000010000001111111100000000011111000111111111000000111111111110011111111001111111110000000011000111111110000111011111111111100001111100001111111100000000000011110011101110001000111111110000000001111000011111110010110001111111111000000000000000000111111111110000000100000000000000000011110111110000001000011101110000000000011111111100000011111111111100111111111111000111111111000001111111100000000000001110111111111111000000110011111111111101110001111111111100000000111100000111100000111111111100000111111111111000000011111111000000000001000000111100000001000001111100111111111110000000000000000000010001111111100000011111111100000000000000100001111111111110111001111111111100000111111100001111111111000000000000000000000000011100000111111111111011110000000010000000011111111100011111111111100001110000111111111111100000000000000111110000011111001111111100000000000011100011100000000000011111000001111111111101000000001110000000000000000000000000000111110010000000000111111111000011111111110000000000111111111111101111111111100000000010000000000000011111111100100001100000000000000111100111100000000001100000001111111111110000000011111111111000000000111100000000000000000000111101111111111111000000000001111000011111000011110000000001100111111100111000000000100111000000000000111110000010000011111000000000000001111111111100000000110111111111100000000000000111111111111100000111000000000111111110001111000000111111110111111000000001111000000000010000111111111000011110001111111110111110000111111111111000000000000000000000000111111111110000000111011111111100011111110000000001111111110000011111111100111111110000000001111111111100111111111110000000000110000000000000000001000011111111110000000001111111110000000000000000000000011111111111111000000111111111000001111111110000000000111111110000010000000011111111000011111001111111100000001110000000011110000000001011111111000011111011111111110011011111111111000000000000000000100011111111111101111111100000000000000001100000000000000000011110010111110000000011111111100000000001111100011111111111101100000000111110000011110000111111111111000000001111111111100001110111111111110111000000000011111111101111100011111111110000000000000000000000000010000111111111100000000001111111110111110000000000000000000000110000011110000000000001111111111100110001111111100000011100000000000111110000000011111111110000011111000001111000110000000011100000000000000111100001111111111100000111000000001111111111000000111111111100110000000001111000001111111100011100001111111110000010011111111110000000000000000000111100000011111000001111000000000111111001110000000011111111000100000000000011111111000011001111111100000000000110111000000000000111111111111000100000000111111111110000001111111111011100000000000000000000000000";
decodeMorse(decodeBitsAdvanced(trembling));
*/


  Assert::That(decodeMorse(decodeBitsAdvanced("0000000011011010011100000110000001111110100111110011111100000000000111011111111011111011111000000101100011111100000111110011101100000100000")), "HEY JUDE");

//public void testShortMessages() {
  Assert::That(decodeMorse(decodeBitsAdvanced("")), "");
  Assert::That(decodeMorse(decodeBitsAdvanced("0")), "");
  Assert::That(decodeMorse(decodeBitsAdvanced("000000000000000000000000000000000000000000")), "");
  Assert::That(decodeMorse(decodeBitsAdvanced("1")), "E");
  Assert::That(decodeMorse(decodeBitsAdvanced("")), "");
  Assert::That(decodeMorse(decodeBitsAdvanced("0")), "");
  Assert::That(decodeMorse(decodeBitsAdvanced("000000000000000000000000000000000000000000")), "");
  Assert::That(decodeMorse(decodeBitsAdvanced("1")), "E");

  //public void testMultipleBitsPerDot() {
  Assert::That(decodeMorse(decodeBitsAdvanced("111")), "E");
  Assert::That(decodeMorse(decodeBitsAdvanced("1111111")), "E");
  Assert::That(decodeMorse(decodeBitsAdvanced("110011")), "I");
  Assert::That(decodeMorse(decodeBitsAdvanced("111110000011111")), "I");
  Assert::That(decodeMorse(decodeBitsAdvanced("11111100111111")), "M"); // =>

  //public void testExtraZeros() {
  Assert::That(decodeMorse(decodeBitsAdvanced("01110")), "E");
  Assert::That(decodeMorse(decodeBitsAdvanced("000000011100000")), "E");

  //public void testEmptyMessage() {
  Assert::That(decodeMorse(decodeBitsAdvanced("")), "");
  Assert::That(decodeMorse(decodeBitsAdvanced("0")), "");
  Assert::That(decodeMorse(decodeBitsAdvanced("0000000000")), "");

//public void testLongMessage() {
  Assert::That(decodeMorse(decodeBitsAdvanced("1100110011001100000011000000111111001100111111001111110000000000000011001111110011111100111111000000110011001111110000001111110011001100000011")), "HEY JUDE");
Assert::That(decodeMorse(decodeBitsAdvanced("00000000000111111100000011010001110111000000001110000000000000000001111111011111100001101111100000111100111100011111100000001011100000011111110010001111100110000011111100101111100000000000000111111100001111010110000011000111110010000011111110001111110011111110000010001111110001111111100000001111111101110000000000000010110000111111110111100000111110111110011111110000000011111001011011111000000000000111011111011111011111000000010001001111100000111110111111110000001110011111100011111010000001100001001000000000000000000111111110011111011111100000010001001000011111000000100000000101111101000000000000011111100000011110100001001100000000001110000000000000001101111101111000100000100001111111110000000001111110011111100011101100000111111000011011111000111111000000000000000001111110000100110000011111101111111011111111100000001111110001111100001000000000000000000000000000000000000000000000000000000000000")), "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG");


/*
Assert::That(decodeMorse(decodeBitsAdvanced("11111000001111111000011111100000111111111111111000011111111111111000000111111111111111100001110000011111100000001111000000000000000011111111111111000000111110000011111111111111100000011110000011111111111111100001111111111111110000000000000000000000000000000000011111111111111110000000000000001111000000111110000011110000000111100000000000000111110000000000000000000000000000000000011111111111111100000111111111111111000011111000001111111111111100000000000000001111111000000111111000000011111111111111000000000000000011110000001111100000000000000011111111111111100000111111000111111111111111000011110000000000000000111111111111110000000111100000111111111111110000000000000000000000000000000000011111111111111100000111111000011111000000111110000000000000001111110000111111111111111100000011110000000000000011111111111111100001111111111111110000111111111111110000000000000001111100000001111111111111110000000111111111111111110000000000000000111111111111111000001111100000000000000000000000000000000000011110001111100000011111111111111110000011100000000000000011111111111111110000011111111111110000001111111111111111000000000000001111111111111110000001111100001111110000001111111111111111000000000000000000000000000000000011110000011111111111111100000011111111111111100001111111111111111000000000000001111110001111000011111111111100000000000000001111111111111100000011111111111111100000000000000110000011111111111111100000111111111111111100000111110000000000000001111110000111110000111111000000000000000000000000000000000011111111111111110001111111111111111000001111111111111111000000000000000111100000111110000111100000111111111111111000000000000000111111000000000000000111000000111111111111111000111100000000000000000000000000000000000011111111111111100000000000000011111110000111100000111111000001111110000000000000001111110000000000000000000000000000000000000111111000111111111111111100000111100000011111110000000000000011110000111111111111111000000000000011111111111111000001111111111111111000001111100001111100000000000000011111111111111000001111110000011111111111111111000011111111111111100000000000000000000000000000000000001111111111111111000001111110000011110000000000000111111111111111000001111111111111111000011111111111111110000000000000001111111111111100000011111111111111100000111100000000000000011111100000111111111111110000001110000011111111111111110000011111000011111111111111")), "SOS! THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.");


  char *source;
  source = "1100110011001100000011000000111111001100111111001111110000000000000011001111110011111100111111000000110011001111110000001111110011001100000011";
  //cout << decodeMorse(decodeBitsAdvanced (source)) << endl;

  source = "0000000011011010011100000110000001111110100111110011111100000000000111011111111011111011111000000101100011111100000111110011101100000100000";
  cout << decodeMorse(decodeBitsAdvanced (source)) << endl;

  source =
  "00000000000111111100000011010001110111000000001110000000000000000001111111011111100001101111100000111100111100011111100000001011100000011111110010001111100110000011111100101111100000000000000111111100001111010110000011000111110010000011111110001111110011111110000010001111110001111111100000001111111101110000000000000010110000111111110111100000111110111110011111110000000011111001011011111000000000000111011111011111011111000000010001001111100000111110111111110000001110011111100011111010000001100001001000000000000000000111111110011111011111100000010001001000011111000000100000000101111101000000000000011111100000011110100001001100000000001110000000000000001101111101111000100000100001111111110000000001111110011111100011101100000111111000011011111000111111000000000000000001111110000100110000011111101111111011111111100000001111110001111100001000000000000000000000000000000000000000000000000000000000000";
  cout << decodeMorse(decodeBitsAdvanced (source)) << endl;

  source = "1";
  cout << decodeMorse(decodeBitsAdvanced (source)) << endl;

  source = "10000001";
  cout << decodeMorse(decodeBitsAdvanced (source)) << endl;

  */

}
///////////////////////////////Arkive///////////////////////////////////////////
vector<Point> rnd_centroids (vector<Point> &grph, int size) {
    vector<Point> center;
    srand (time(0));
    for (int i = 0; i < size; ++i) {
        center.push_back(grph[rand () % grph.size()]);
    }

    return center;
}
int unit (const string &src) {
  int i = 0, bit, minval = 999;
  string signal = src;
  string::iterator it = signal.begin();

  while (it != signal.end())
  minval = min (skip (it), minval);

  return minval;
};
Point mid (Point a, Point b) { return {(a.x + b.x) * 0.5, (a.y + b.y) * 0.5};}

bool is_equal (Point a, Point b) { return a.x == b.x && a.y == b.y;}
Point nearest_point (vector<Point> &grph, Point p) {
  Point nearest;
  float min = numeric_limits<float>::infinity(), dist;

  for (auto &star : grph) {
    dist = p.distance (star);

    if (dist < min) {
      min = dist;
      nearest = star;
    }
  }

  return nearest;
}
vector<Point> k_means (vector<Point> galaxy, vector<Point> ctid) {
  /*
  vector<Point> data = {{5,3},{10,15},{15,12},{24,10},{30,45},{85,70},{71,80},{60,78},{55,52},{80,91}};;
  vector<Point> ctid = {{5,3},{10,15}};

  it 1 => c1 = {5, 3}    c2 = {47.78,50.33}
  it 2 => c1 = {13.5,10} c2 = {63.5,69.33}
  it 3 => c1 = {16.8,17} c2 = {70.2,74.2}
  */
  Point near;
  int index = 2, i;
  bool done = true;

  //while (index-->0) {
    vector<Point> tmp = ctid;
    map<float, float> npoints, sumX, sumY;
    //done = false;
    for (auto &p : galaxy) {
      near = nearest_point (ctid, p);
      for (i = 0; i < ctid.size(); ++i)
      if (is_equal (ctid[i], near)) {
        npoints[i]++;
        sumX[i] += p.x;
        sumY[i] += p.y;
        //cout << sumX[i] << ' ' << sumY[i] << endl;
      }
      //Debug::display_pt (near);
    }
    //cout << endl;
    for (i = 0; i < ctid.size(); ++i) {
      ctid[i].x = sumX[i] / npoints[i];
      ctid[i].y = sumY[i] / npoints[i];
      //Debug::display_vect(ctid);
      //            if (!is_equal (ctid[i], tmp[i])) done = true;
    }
    //}
    //Debug::display_vect (ctid);
    return ctid;
  }

vector<Point> mk_grph (string bits, char ref) {
    vector<Point> pt;
    map<float,float> hist;
    string::iterator it = bits.begin();

    while (it != bits.end()) {
        char bit = *it;
        int size = skip (it);

        if (bit == ref || (ref != '1' && ref != '0')) hist[size]++;
    }

    for (auto &it : hist) pt.push_back ({it.first,it.second});

    return pt;
  }
vector<Point> ones (const string &signal) {

  vector<Point> grph = mk_grph (signal, '1');
  int nclust = min (static_cast<int> (grph.size()), 2);

  vector<Point> seeds = mk_seeds (grph, nclust);
  vector<Point> clust = k_means (grph, seeds);
  /*
  cout << "graph\n";
  Debug::display_vect(grph);
  cout << "ones\n";
  Debug::display_vect (clust);
  */
  return clust;
}
vector<Point> zero (const string &signal) {
  vector<Point> grph = mk_grph (signal, '0'), seeds, clust;
  int nclust = min (static_cast<int> (grph.size()), 3);

  seeds = mk_seeds (grph, nclust);
  clust = k_means (grph, seeds);
  /*
  */
  //initseeds (grph.front().x, 3);

  /*
  cout << "seeds :\n";
  Debug::display_vect (seeds);
  cout << "graph :\n";
  Debug::display_vect (grph);
  cout << "clust :\n";
  Debug::display_vect (clust);
  */
  return clust;
}

string decodeBitsAdvanced1 (const string &bits) {
  string code = clean (bits), morse;
  string::iterator it = code.begin();

  const int dot = unit (code), dash = dot * 3;
  int i = 0, bit;

  while (it != code.end()) {
    bit = *it;
    int size = skip (it);

    if (bit == '1') {
      if (size == dot) morse += '.';
      if (size == dash) morse += '-';
    }

    if (bit == '0') {
      if (size == dash) morse += ' ';
      if (size > dash) morse += "  ";
    }
  };

  return decodeMorse (morse);

}
string decodeBitsAdvanced2 (const string &bits) {
  string code = clean (bits), morse;
  vector<Point> graph = mk_grph (code,'1');
  vector<Point> seeds = {graph.front(),graph.back()};
  vector<Point> clust = k_means (graph, seeds);

  float dot = clust[0].x, dash = clust[1].x;
  string::iterator it = code.begin();
  int i = 0, bit;

  while (it != code.end()) {
      bit = *it;
      int size = skip (it);
      if (bit == '1') {
          //cout << abs(size - dot) - abs(size - dash) << ' ';
          if (abs(size - dot) - abs(size - dash) < 0)
              morse += '.';
          else
              morse += '-';
          //if (size <= dot) morse += '.';
          //if (size > dash) morse += '-';
      }

      if (bit == '0') {
          //cout << size << ' ';
          if (abs(size - dot) - abs(size - dash) > 0) morse += ' ';
          //if (size < dash) morse += ' ';
          //if (size > dash) morse += ' ';
      }
  };

  return decodeMorse (morse);
}
string decodeBitsAdvanced3 (const string &bits) {

  string code = clean (bits), morse;
  string::iterator it = code.begin();

  vector<Point> o = ones (code), z = zero (code);
  //vector<Point> unit = mk_unit (code);
  float dot, dash, pause, size;
  int bit;
  /*
  */
  cout << "ones :\n";
  Debug::display_vect (o);
  cout << "zero :\n";
  Debug::display_vect (z);
  while (it != code.end()) {
      bit = *it;
      size = skip (it);
      if (bit == '1') {
          dot = abs (size - o.front().x), dash = abs (size - o.back().x);
          //cout << dot << ' ' << dash << endl;
          if (dot <= dash)
              morse += '.';
          else
              morse += '-';
      }

      if (bit == '0') {
          dot = abs (size - z[0].x), dash = abs (size - z[1].x), pause = abs (size - z[2].x);;

          if (dash < dot) morse += ' ';
          if (pause < dash) morse += ' ';

      }
  }

  cout << morse << endl;
  return decodeMorse (morse);
}

vector<Point> mk_grph1 (string bits, char ref) {
    char bit = ref;
    int size, mxval = 0, mnval = 999, hist[64] = {0};
    vector<Point> points;

    for (int i = 0; i < bits.size(); i++) {
        if (ref != '0' && ref != '1') bit = bits[i];
        size = 0;

        while (bits[i] == bit) {
            size++, i++;
        }

        if (size != 0) {
            hist[size]++;
            mnval = min (mnval, size);
        }

        mxval = max (mxval, size);
    }
    //cout << mnval << endl;
    for (int i = 1; i <= mxval; i++)
        points.push_back (Point(i, hist[i]));

    return points;
}
void k_pp (vector<Point> &galaxy, int k) {
    vector<Point> center;
    srand (time(0));
    auto sqr = [] (float d) { return d * d;};
  /*
// 4 Repeat Steps 2 and 3 until k centers have been chosen.
// 5 Now that the initial centers have been chosen, proceed using standard k-means clustering.
*/
// 1 Choose one center uniformly at random among the data points.
    center.push_back(galaxy[rand () % galaxy.size()]);
    //center[0].id = 0;
    for (auto &p : galaxy) {
      // 2 For each data point x, compute D(x), the distance between x and the nearest center that has already been chosen.
        for (auto &c : center) {
            vector<int> distances;
            distances.push_back (sqr (p.distance (c)));
            cout << fixed << p.distance (c) << endl;

        }
        // 3 Choose one new data point at random as a new center, using a weighted probability distribution where a point x is chosen
        // with probability proportional to D(x)2.
        Point nxt = galaxy[rand () % galaxy.size()];
    }
    //Debug::display_vect(galaxy);
    //Debug::display_pt (center[0]);
}
/*
vector<Point> readcsv() {
    vector<Point> points;
    float x, y;
    string line;
    ifstream file ("Mall_data.csv");
    getline (file, line); // skip first line

    while (getline (file, line)) {

        stringstream lineStream (line);
        string bit;
        int i = 3;
        while (i-->0)
            getline(lineStream, bit, ',');

        x = stof (bit);

        getline(lineStream, bit, ',');
        y = stof (bit);

        points.push_back(Point(x, y));
    }

    return points;
}
void kMeansClustering(vector<Point>* points, int epochs, int k) {

    const int n = points->size();
    vector<Point> centroids;
    srand (time(0));
    int it, clustid;
    for (it = 0; it < k; ++it)
        centroids.push_back (points->at(rand () % n));

    while (epochs-->0) {
        clustid = 0;

        for (auto &c : centroids) {
            for (auto &p : *points) {
                float dist = c.distance(p);

                if (dist < p.minDist) {
                    p.minDist = dist;
                    p.cluster = clustid;
                }
            }
            clustid++;
        }

        vector<int> nPoints (k, 0);
        vector<float> sumX (k,0), sumY (k,0);
        // Iterate over points to append data to centroids
        for (Point &p : *points) {
            clustid = p.cluster;
            nPoints[clustid]++;
            sumX[clustid] += p.x;
            sumY[clustid] += p.y;

            p.minDist = __DBL_MAX__;  // reset distance
        }
        // Compute the new centroids
        for (vector<Point>::iterator c = begin(centroids);  c != end(centroids); ++c) {
            clustid = c - begin(centroids);
            c->x = sumX[clustid] / nPoints[clustid];
            c->y = sumY[clustid] / nPoints[clustid];

            //cout << '[' << setprecision(4)<< c->x << ", " << c->y << ']' <<  endl;
        }
        //cout << endl;
    }
}
*/
