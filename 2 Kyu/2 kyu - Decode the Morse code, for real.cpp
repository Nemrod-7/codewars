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

    Pause between dots and dashes in a character – is 1 time unit long.
    Pause between characters inside a word – is 3 time units long.
    Pause between words – is 7 time units long.
*/

map<string, string> MORSE_CODE =
{{".-", "A"},{"-...", "B"},{"-.-.", "C"},{"-..", "D"},{".", "E"},{"..-.", "F"},
{"--.", "G"},{"....", "H"},{"..", "I"},{".---", "J"},{"-.-", "K"},{".-..", "L"},{"--", "M"},
{"-.", "N"},{"---", "O"},{".--.", "P"},{"--.-", "Q"},{".-.", "R"},{"...", "S"},{"-", "T"},
{"..-", "U"},{"...-", "V"},{".--", "W"},{"-..-", "X"},{"-.--", "Y"},{"--..", "Z"},{"-----", "0"},
{".----", "1"},{"..---", "2"},{"...--", "3"},{"....-", "4"},{".....", "5"},{"-....", "6"},
{"--...", "7"},{"---..", "8"},{"----.", "9"}};

class Point {
    public :
        double x, y;     // coordinates
        //int id = -1;      // no default id
        //double mindist = numeric_limits<double>::infinity();  // default infinite dist to nearest id
        Point () { x = 0, y = 0;}
        Point (double a, double b) { x = a, y = b;}
        double distance (Point p) { return hypot (p.x - x, p.y - y); }
        //bool isequal (Point p) {return x == p.x && y == p.y;}
};
class Debug {
    public :
        static void display_map (map<double,double> points) {
            for (auto &it : points)
              cout << '[' << it.first << ',' << it.second << ']' <<  endl;
            cout << endl;
        }
        static void display_pt (Point &p) {
            cout << setw(2) << fixed << setprecision(2) ;
            cout << '[' << p.x << ", " << p.y << ']';
            //cout <<" clust : " << p.id;
            cout << endl;
        }
        static void display_vect (vector<Point> &points) {
            for (auto &it : points) {
                display_pt (it);
            }
            cout << endl;
        }
};

string clean (string src) {
    int first = src.find_first_of ('1');
    int last = src.find_last_of('1') + 1;
    return src.substr(first, last - first);
}
int skip (auto &it) {
    int ref = *it, size = 0;
    while (*it == ref) {
      size++, it++;
    }
    return size;
}
bool isequal (Point a, Point b) { return a.x == b.x && a.y == b.y;}
vector<Point> rnd_centroids (vector<Point> &grph, int size) {
    vector<Point> center;
    srand (time(0));
    for (int i = 0; i < size; ++i) {
        center.push_back(grph[rand () % grph.size()]);
        //center[i].id = i;
    }

    return center;
}
vector<Point> mk_grph (string bits, char ref) {
    char bit;
    int size, mxval = 0, hist[64] = {0};
    vector<Point> points;
    //map<double, double> freq;
    for (int i = 0; i < bits.size(); i++) {
        bit = bits[i];
        size = 0;
        //cout << bit << ' ';
        while (bits[i] == ref) {
            size++, i++;
        }
        if (size != 0)
            hist[size]++;

        mxval = max (mxval, size);
    }
    
    for (int i = 1; i <= mxval; i++)
        points.push_back(Point(i, hist[i]));

    return points;
}

Point nearest_point (vector<Point> &map, Point p) {
    Point nearest;
    double min = numeric_limits<double>::infinity(), dist;

    for (auto &star : map) {
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
    int index = 10, i;
    bool done = true;

    while (done) {
        vector<Point> tmp = ctid;
        done = false;
        //Debug::display_vect(ctid);
        //Debug::display_vect(ctid);
        map<double, double> npoints, sumX, sumY;
        for (auto &p : galaxy) {
            near = nearest_point (ctid, p);

            for (i = 0; i < ctid.size(); ++i)
                if (isequal (ctid[i], near)) {
                    npoints[i]++;
                    sumX[i] += p.x;
                    sumY[i] += p.y;
                }

        }

        for (i = 0; i < ctid.size(); ++i) {
            ctid[i].x = sumX[i] / npoints[i];
            ctid[i].y = sumY[i] / npoints[i];
            if (!isequal (ctid[i], tmp[i])) done = true;
        }
    }

    return ctid;
}
void k_pp (vector<Point> &galaxy, int k) {
    vector<Point> center;
    srand (time(0));
    auto sqr = [] (double d) { return d * d;};
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
    /*
    */
    //Debug::display_vect(galaxy);
    //Debug::display_pt (center[0]);
}

string decodeMorse (string morseCode) {


    std::istringstream iss(morseCode);
    std::string tok, os;

    while (iss >> tok) {
        os += MORSE_CODE[tok];
        if (iss.get() == ' ' && iss.get() == ' ')
            os += ' ';
        else
            iss.unget();
    }
    os.erase(os.find_last_not_of(' ')+1);

    return os;
}
string decodeBitsAdvanced2 (const string &bits) {
  string code = clean (bits), morse;
  vector<Point> graph = mk_grph (code,'1');
  vector<Point> seeds = {graph.front(),graph.back()};
  vector<Point> clust = k_means (graph, seeds);

  double dot = clust[0].x, dash = clust[1].x;
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

vector<Point> ones (const string &signal) {
    //string signal = clean (src);
    vector<Point> grph = mk_grph (signal, '1');
    double size = (grph.back().x - grph.front().x) / 3;
    //cout << size << endl;
    vector<Point> means = k_means (grph, {grph.front(), grph.back()});
    Debug::display_vect(grph);
    return means;
}
vector<Point> zero (const string &signal) {
    vector<Point> grph = mk_grph (signal, '0');
    double size = (grph.back().x - grph.front().x) / 7;

    vector<Point> means = k_means (grph, {grph.front(), grph.back()});

    Debug::display_vect(grph);
    return means;
}

int main () {

  const char *source;
  source =
  "00000000000111111100000011010001110111000000001110000000000000000001111111011111100001101111100000111100111100011111100000001011100000011111110010001111100110000011111100101111100000000000000111111100001111010110000011000111110010000011111110001111110011111110000010001111110001111111100000001111111101110000000000000010110000111111110111100000111110111110011111110000000011111001011011111000000000000111011111011111011111000000010001001111100000111110111111110000001110011111100011111010000001100001001000000000000000000111111110011111011111100000010001001000011111000000100000000101111101000000000000011111100000011110100001001100000000001110000000000000001101111101111000100000100001111111110000000001111110011111100011101100000111111000011011111000111111000000000000000001111110000100110000011111101111111011111111100000001111110001111100001000000000000000000000000000000000000000000000000000000000000";
  string signal = clean (source);

  zero (signal);
  //"0000000011011010011100000110000001111110100111110011111100000000000111011111111011111011111000000101100011111100000111110011101100000100000";
  //vector<Point> clust = k_means (graph, seeds);
  //cout << decodeBitsAdvanced2 (source);

  //string medium = "1100110011001100000011000000111111001100111111001111110000000000000011001111110011111100111111000000110011001111110000001111110011001100000011";

  /*

  Debug::display_vect(points);

  */
  //cout << decodeBitsAdvanced (medium);
  //k_means (points, 3);
  //vector<Point> centers = k_means (points, 3);
}
///////////////////////////////Arkive///////////////////////////////////////////
int unit (const string &src) {
  int i = 0, bit, minval = 999;
  string signal = src;
  string::iterator it = signal.begin();

  while (it != signal.end())
  minval = min (skip (it), minval);

  return minval;
};
string decodeBitsAdvanced (const string &bits) {
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

/*
vector<Point> readcsv() {
    vector<Point> points;
    double x, y;
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
                double dist = c.distance(p);

                if (dist < p.minDist) {
                    p.minDist = dist;
                    p.cluster = clustid;
                }
            }
            clustid++;
        }

        vector<int> nPoints (k, 0);
        vector<double> sumX (k,0), sumY (k,0);
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
