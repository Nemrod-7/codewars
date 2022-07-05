#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using point = pair<int,int>;

class graph {
    private :
        string maze;
    public :
        int width, height;

        graph (string src = {}) : maze(src) {
            width = src.find ('\n') + 1, height = count (begin(src), end(src), '\n') + 1;
        }

        bool is_inside (const point &p) {
            return p.first >= 0 && p.second >= 0 && p.first < width - 1 && p.second < height;
        }
        //bool is_free (const point &p) { return this->operator[](p) != wall; }
        char operator[] (const point &p) { return is_inside(p) ? maze[p.second * width + p.first] : '#'; }
};
vector<string> cut (const string &src) {
    vector<string> part;

    return part;
}

int main () {

}
