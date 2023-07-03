#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <vector>
#include <map>
#include <chrono>

using namespace std;

string format (int64_t x) { // display number in human readable format
    string num = to_string (x), os;
    int size = num.size(), index = 0;

    while (size-->0) {
        os += num[index++];
        if (size % 3 == 0) os += ' ';
    }
    return os;
}
string hformat (double duration) { // display time in human readable format

    if (duration == 0.0) return "now";
    const vector<pair<int,string>> base {{31536000,"year"},{86400,"day"}, {3600,"hour"},{60,"mn"}, {1,"s"}};

    int count = 0;
    vector<int> period;
    ostringstream os;

    for (auto it : base) {
        period.push_back (duration / it.first);

        if (period.back() != 0)
            count++;

        duration -= (period.back() * it.first);
    }

    for (int i = 0; i < 5; ++i) {
        if (period[i] != 0) {
            count--;
            os << period[i] << " " << base[i].second;
            // if (period[i] > 1) os << "s";
            if (count > 1) os << ", ";
            if (count == 1) os << " ";
        }
    }

    if (duration > 0.0) os << " " << fixed << int(duration * 60 * 1e5) / 1e5 << " ms";
    return os.str();
}
string showsize (uint64_t size) { // human-readable size
    const vector<string> sizes = {"TiB", "GiB", "MiB", "KiB", "B" };
    const int64_t KB = 1024;
    const int64_t MB = KB * 1024;
    const int64_t GB = MB * 1024 ;
    const int64_t TB = GB * 1024;
    uint64_t  maxv = TB;
    stringstream os;

    for (int i = 0; i < 5; i++, maxv /= 1024) {
        if (size < maxv) continue;
        os << fixed << setprecision(2) << size / static_cast<float> (maxv) << " " << sizes[i];
        return os.str();
    }

    return "";
}

class Timer {
    //using time = chrono::steady_clock;
    private :
        chrono::steady_clock::time_point alpha, end;
        chrono::duration<double> elapsed;
        uint64_t cycle;

        void update () {
            end = chrono::steady_clock::now ();
            elapsed = end - alpha;
        }
    public :

        Timer() {
            alpha = chrono::steady_clock::now ();
            cycle = 0;
        }
        void start () { alpha = chrono::steady_clock::now ();}
        void stop () { update();}
        void get_duration () {
            std::cout << "\nExecution time : " << hformat (elapsed.count());
        }
        bool running (double total) {
            update();
            cycle++;
            if (elapsed.count() < total) return true;
            cout << "cycles :: " ;

            string num = to_string (cycle);
            int size = num.size(), index = 0;

            while (size-->0) {
                cout << num[index++];
                if (size % 3 == 0) cout << ' ';
            }
            cout << endl;
            return false;
        }
};
class check {
    public :
        static void overflow (int64_t a, int64_t b) {
            int64_t limit = numeric_limits<int64_t>::max() / b;
            if (a > limit) throw overflow_error ("integer overflow\n");
        }
};

template<class T> void showvec (const vector<T> &v) {
    for (auto it : v) {
        cout << it << " ";
    }
    cout << "\n";
}

char getsep (string name) {
    int maxv = 0;
    char sep;
    string buff;
    map<char,int> hist;
    fstream file (name);

    while (getline (file, buff)) {

        for (char ch : buff) {
            if (ispunct (ch)) {
                //cout << ch << ' ';
                hist[ch]++;
            }
        }
    }

    for (auto it : hist) {
        if (it.second > maxv) {
            maxv = it.second;
            sep = it.first;
        }
    }

    file.close();
    return sep;
}
vector<string> tokenize (const string &src, char delim) {
    istringstream iss (src);
    string token;
    vector<string> v;

    while (getline (iss, token, delim))
        v.push_back (token);

    return v;
}
vector<vector<string>> getfile (string name) {

    string buff;
    fstream file (name);
    char delim = getsep (name);
    vector<vector<string>> mat;

    while (getline (file, buff)) {
        mat.push_back(tokenize (buff, delim));
    }

    file.close();
    return mat;
}
vector<vector<int>> txt2int (const vector<vector<string>> &txt) {
  const int size = txt.size();
  vector<vector<int>> adj (size, vector<int> (size));

  for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
          if (txt[i][j] == "-") {
              adj[i][j] = 0;
          } else {
              adj[i][j] = stoi(txt[i][j]);
          }
      }
  }
  return adj;
}
