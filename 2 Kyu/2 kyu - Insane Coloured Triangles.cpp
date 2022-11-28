#include <iostream>
#include <vector>

#include <random>
#include <chrono>
#include <iomanip>

using namespace std;

/////////////////////////////////Assert/////////////////////////////////////////
typedef char t_type;
class Assert {
    public:
        static void That(t_type input, t_type expected) {
            if (input != expected) {
                cout << "error"<< endl;
            }
        }

};
t_type Equals(t_type entry) {return entry;}
void Test ();
/////////////////////////////////Timer/////////////////////////////////////////
class Timer {
    //using time = chrono::steady_clock;
    private :
        chrono::steady_clock::time_point alpha, end;
        chrono::duration<double> elapsed;
        uint64_t index;

        void update () {
            end = chrono::steady_clock::now ();
            elapsed = end - alpha;
        }
    public :

        Timer() {
            alpha = chrono::steady_clock::now ();
            index = 0;
        }
        void start () { alpha = chrono::steady_clock::now ();}
        void stop () { update();}
        void get_duration () {
            std::cout << "\nDuration "
                      <<fixed<< elapsed.count()
                      << " ms" << std::endl;
        }
        bool running (double total) {
            update();
            index++;
            if (elapsed.count() < total) return true;
                cout << "index :: " << index << endl;
            return false;
        }
};

/*
B B -> 0 + 0 : -0 - 0 = 0
R R -> 1 + 1 : -1 -1 = -2
G G -> 2 + 2 : -2 -2 = -4

R G -> 1 + 2 : 3 -3 = 0 -> B  : -1 - 2 = -3
B G -> 0 + 2 : 3 -2 = 1 -> R  : -0 - 2 = -2
B R -> 0 + 1 : 3 -1 = 2 -> G  : -0 - 1 = -1

// => triangle (N) = sum from n = 0 to N sum from k = 0 to n <- binom (n k)
// => binom (n k)  = sum from i = 0 to k ((n + 1 - i) / i)
// => lucas (n k)  = prod from i = 0 to infinity (binom (n_i k_i) mod p)
*/

const vector<char> colors = {'B','R','G'};

inline int mod3 (int n) {

    if (n < 0) return ((n %= 3) < 0) ? n + 3 : n;
    if (n * 0xAAAAAAAB <= n) return 0;
    return n % 3;
}
int lucas3 (int n, int k) {

    int prod = 1, np, kp;
    // => lucas (n k) = prod from i = 0 to infinity (binom(n_i k_i) mod p)
    do {
        np = mod3 (n), kp = mod3 (k);
        //cout << '(' << np << ' ' << kp << ") => " << binomial (np, kp) << endl;
        if (np < kp) return 0;
        if (np == 2 && kp == 1) prod *= 2; //prod *= binomial (np, kp);

        n /= 3, k /= 3;

    } while (n || k);


    return mod3 (prod);
}
char triangle (const std::string &row) {
    const std::vector<char> colors = {'B','R','G'};
    int N = row.size() - 1, sum = 0, k = row.size();

   while (k-->0)
        sum += lucas3 (N, k) * row[k] % 3;

    return colors[mod3 (pow (-1, N) * sum)];
  }

int binomial (int n, int k) {
    if (k == 0 || k == n) return 1;
    if (k  < 0 || k  > n) return 0;

    int res = 1, limit;
    //double num;
    k = min (k, n - k);
    // => binom (n k) = sum from i = 0 to k ((n + 1 - i) / i)
    for (double i = 1; i <= k; i++)
        //check::overflow (res, num);
        res *= (n + 1 - i) / i;

    return res;
  }

vector<vector<int>> mk_triangle (int size) {
    vector<vector<int>> tri;

    for(int n = 0; n <= size; ++n) {
        tri.push_back(vector<int>());
        for(int k = 0; k <= n; ++k)
            tri[n].push_back (round (binomial (n, k)));
    }

    return tri;
}
void display_tri (vector<vector<int>> &tria) {

    for (auto &n : tria) {
        cout << string ((tria.size() - n.size()) * 2, ' ');
        for (auto &k : n) {
            cout << setw(3) << k << ' ';
        }
        cout << endl;
    }
}
string mk_string (int size) {

    string row;
    mt19937 rng (random_device {}()); // generate entropy pool => does not deplete the system entropy
    uniform_int_distribution<int> distr (0, 2);

    while (size-->0) row += colors[distr(rng)];

    return row;
}
char triangle3 (string &row, pair<int,int> coord) {

    //auto mod3 = [](int k) {return ((k %= 3) < 0) ? k + 3 : k;};
    int N = row.size () - coord.first, sign = pow (-1, N);
    int sum = 0, num, comm;

    for (int k = 0; k <= N; ++k) {
          num = row[k + coord.second] % 3;
          sum += num * lucas3 (N, k);
          //sum += num * lucasth (N, k, 3);
    }

    return colors[mod3 (sign * sum)];
}

int main () {

  Timer clock;

  string row = "RRGBRGBB";

  pair<int,int> p;
  uint64_t N = 20;

  for (int n = 0; n <= N; ++n) {
      cout << string ((N - n) * 2, ' ');
      for (int k = 0; k <= n; ++k) {
          int cell = lucas3 (n, k);
          if (cell == 0) {
              cout << "    ";
          } else {
              cout << setw (3) << '#' << " ";
          }
      }
      cout << endl;
  }

  int index = 0;
  /*
 while (clock.running(2))
      for (int n = 0; n < N; ++n) {
          //cout << string (N - n, ' ');
          for (int k = 0; k <= n; ++k) {
              lucas3 (n, k); //      114 442 610
              //lucasth (n, k, 3); // 73 989 510
              //cout << ' '  << lucasth (n, k, 3);
              index++;
          }
          //cout << endl;
      }

      cout << " idx2 " << index << endl;

  }
  */
  /*
  p.first = row.size();
  while (p.first-->0) {
      for (p.second = 0; p.second < p.first; ++p.second) {
          cout << triangle3 (row, p) << ' ';
      }
      cout << endl;
  }
  Test ();

  for (int i = -5; i < 10; ++i) {
      cout << setw (2) <<  i << ' ' << mod3 (i) << endl;
  }
  */
  clock.stop();
  clock.get_duration();

  return EXIT_SUCCESS;
}

void test_cases(const vector<pair<string, char> > &cases) {
  for (const auto c : cases)
      Assert::That(triangle(c.first), Equals(c.second));
}

void Test () {
        test_cases ({make_pair("B", 'B'),
                    make_pair("GB", 'R'),
                    make_pair("RRR", 'R'),
                    make_pair("RGBG", 'B'),
                    make_pair("RBRGBRB", 'G'),
                    make_pair("RBRGBRBGGRRRBGBBBGG", 'G'),
            	      make_pair("BGRGRBGBRRBBGRBGBBRBRGBRG", 'B'),
                    make_pair("GRBGRRRBGRBGRGBRGBRBRGBRRGRBGRGBB", 'R'),
                    make_pair("RBGRBGBRGBRBRGGRBBGRBGBRBBGRBGGBRBGBBGRBGBRGRBGRBB", 'G'),
                    make_pair("BGBGRBGRRBGRBGGGRBGRGBGRRGGRBGRGRBGBRGBGBGRGBGBGBGRRBRGRRGBGRGBRGRBGRBGRBBGBRGRGRBGRGBRGBBRGGBRBGGRB", 'G')});
}
/////////////////////////////////Arkive////////////////////////////////////////
int lucasth (int n, int k, int p) {

    int prod = 1, np, kp;
    //vector<vector<int>> base = mk_triangle (9);
    // => lucas (n k) = prod from i = 0 to infinity (binom(n_i k_i) mod p)
    do {
        np = n % p, kp = k % p;
        //cout << '(' << np << ' ' << kp << ") => " << binomial (np, kp) << endl;
        if (kp > np) return 0;
        prod *= binomial (np, kp);

        n /= p, k /= p;

    } while (n || k);

    return prod % p;
}

class check {
  public :
      static void overflow (uint64_t a, double b) {
          uint64_t limit = numeric_limits<uint64_t>::max() / b;
          //if (b > 1) limit /= b;
          if (a > limit) throw overflow_error ("integer overflow\n");

      }
};

inline int factorial (int n) {
  //return num ? num * fact (num - 1) : 1;
  int mult = 1;
  for (int i = 1; i <= n ; ++i)
  mult *= i;

  return mult;
}
double binom (int n, int k) {// binom (9, 5); -> 12 118 579 in 2s
    return 1 / ((n + 1) * beta (n - k + 1, k + 1));
}
int binom2 (int n, int k) {  // binom2 (9, 5); -> 2 453 922 in 2s
  if (k == 0 || k == n) return 1;
  if (k  < 0 || k  > n) return 0;
  return binom2 (n - 1, k - 1) + binom2 (n - 1, k);
}
inline int binom3 (int n, int k) {  // binom3 (9, 5); -> 20 973 915
    return factorial (n) / (factorial (k) * factorial (n - k));
}

char triangle1 (const string &row) {

    int size = row.size();
    int V[size], nxt = 0;

    for (auto &it : row)
    V[nxt++] = it % 3;

    do {
      for (int i = 0; i < size - 1; ++i) {
        int c = -V[i] - V[i + 1];
        //cout << c << endl;
        if (V[i] != V[i + 1]) {
            V[i] = 3 - (V[i] + V[i + 1]);
        }
      }
    } while (size--> 1);

    return colors[V[0]];
  }
