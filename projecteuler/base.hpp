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
    istringstream file (name);

    while (getline (file, buff)) {
        for (char ch : buff) {
            if (ispunct (ch)) {
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

    return sep;
}
vector<int> tokenize (const string &src, char delim) {
    istringstream iss (src);
    string token;
    vector<int> v;

    while (getline (iss, token, delim))
        v.push_back (stoi (token));

    return v;
}
vector<vector<int>> getfile (string name) {
    int num;
    string buff;
    fstream file (name);
    vector<vector<int>> mat;

    while (getline (file, buff)) {
        mat.push_back(tokenize (buff, ' '));
    }

    file.close();
    return mat;
}
