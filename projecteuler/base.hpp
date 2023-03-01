#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <vector>
#include <map>
#include <chrono>

using namespace std;

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
            std::cout << "\nDuration "
                      <<fixed<< elapsed.count()
                      << " ms" << std::endl;
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
