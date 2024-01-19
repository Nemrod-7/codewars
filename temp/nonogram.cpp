#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <bitset>
#include <numeric>

using namespace std;

void getfile (const string &txt) {

    string buffer;
    istringstream iss (txt), clue;
    vector<vector<int>> rows, cols;

    getline (iss, buffer);

    clue.str(buffer);
    while (clue >> buffer) {
        vector<int> line;
        for (char ch : buffer) {
            line.push_back (static_cast<int> (ch - 64));
        }
        rows.push_back(line);
    }

    getline (iss, buffer);
    clue.clear();
    clue.str(buffer);

    while (clue >> buffer) {
        vector<int> line;
        for (char ch : buffer) {
            line.push_back (static_cast<int> (ch - 64));
        }
        cols.push_back(line);
    }
}

template<uint _N, uint _G> class Nonogram {
    enum class ng_val : char {X='#',B='.',V='?'};

    template<uint _NG> struct N {

        N() {}
        N (vector<int> ni, const int l) : X{}, B{}, Tx{}, Tb{}, ng(ni), En{}, gNG(l) {}

        bitset<_NG> X, B, T, Tx, Tb;
        vector<int> ng;
        int En, gNG;

        void fn (const int n, const int i, const int g, const int e, const int l) {

            if (fe(g,l,false) && fe(g+l,e,true)){
                if ((n+1) < ng.size()) {
                    if (fe(g+e+l,1,false))
                        fn(n+1,i-e-1,g+e+l+1,ng[n+1],0);
                } else {
                    if (fe(g+e+l,gNG-(g+e+l),false)) {
                        Tb &= T.flip();
                        Tx &= T.flip();
                        ++En;
                    }
                }
            }

            if (l <= gNG - g - i - 1) {
                fn(n,i,g,e,l+1);
            }
        }

        void fi (const int n, const bool g) {
            X.set(n,g);
            B.set(n, not g);
        }

        ng_val fg (const int n) const {
            return (X.test (n)) ? ng_val::X : (B.test(n)) ? ng_val::B : ng_val::V;
        }

        inline bool fe (const int n,const int i, const bool g){
            for (int e = n; e < n + i; ++e)
                if ((g && fg(e)==ng_val::B) || (!g and fg(e)==ng_val::X))
                    return false;
                else
                    T[e] = g;
            return true;
        }

        int fl (){
            if (En == 1) return 1;

            Tx.set();
            Tb.set();
            En=0;

            fn(0, accumulate (ng.cbegin(), ng.cend(),0) + ng.size() - 1, 0, ng[0], 0);

            return En;
        }
    }; // end of N

    vector<N<_G>> ng;
    vector<N<_N>> gn;
    int En, zN, zG;

    void setCell(uint n, uint i, bool g){
        ng[n].fi(i,g);
        gn[i].fi(n,g);
    }

    public:

    Nonogram (const vector<vector<int>> &n, const vector<vector<int>> &i, const vector<string> &g = {}) : ng{}, gn{}, En{}, zN(n.size()), zG(i.size()) {

        for (int n = 0; n < zG; n++) {
            gn.push_back(N<_N>(i[n],zN));
        }

        for (int i = 0; i < zN; i++) {
            ng.push_back(N<_G>(n[i],zG));

            if (i < g.size()) {
                for (int e = 0; e < zG || e < g[i].size(); e++) {
                    if (g[i][e]=='#') {
                        setCell(i,e,true);
                    }
                }
            }
        }
    }

    bool solve(){
        int i{}, g{};

        for (int l = 0; l < zN; l++) {
            if ((g = ng[l].fl()) == 0) {
                return false;
            } else {
                i+=g;
            }

            for (int i = 0; i < zG; i++) {
                if (ng[l].Tx[i] != ng[l].Tb[i]) {
                    setCell (l,i,ng[l].Tx[i]);
                }
            }
        }

        for (int l = 0; l < zG; l++) {
            if ((g = gn[l].fl()) == 0) {
                return false;
            } else {
                i+=g;
            }
            for (int i = 0; i < zN; i++) {
                if (gn[l].Tx[i] != gn[l].Tb[i]) {
                    setCell (i,l,gn[l].Tx[i]);
                }

            }
        }

        if (i == En) {
            return false;
        } else {
            En = i;
        }

        if (i == zN + zG) {
            return true;
        } else {
            return solve();
        }

    }

    void toStr() const {

        for (int i = 0; i< zN; i++){
            for (int g = 0; g < zG; g++){
                cout << static_cast<char> (ng[i].fg(g));}
            cout << endl;
        }
    }


};

class Nonogram2 {

    vector<vector<int>> grid;
    int width, height;

    void show (vector<vector<int>> &top, vector<vector<int>> &left) {

        size_t padx = 0, pady = 0;
        for (auto csp : top) {
            padx = max (csp.size(), padx);
        }
        for (auto csp : left) {
            pady = max (csp.size(), pady);
        }

        while (padx-->0) {
            for (size_t i = 0; i < pady; i++) {
                cout << setw(2) << " " << "|";
            }
            for (auto &csp : top) {
                if (csp.size() > padx) {
                    cout << setw(2) << csp[padx];
                } else {
                    cout << setw(2) << " ";
                }
                cout << setw(2) << "|";
            }
            cout << setw(2) << endl;
        }

        for (size_t y = 0; y < height; y++) {
            size_t space = pady - left[y].size();
            while (space-->0) cout << setw(3) << "|";

            for (auto cspy : left[y])
                cout << setw(2) << cspy << "|";

            for (size_t x = 0; x < width; x++) {
                if (grid[y][x]) {
                    cout << setw(2) << "#";
                } else {
                    cout << setw(2) << " ";
                }
                cout << setw(2) << "|";
            }
            cout << setw(2) << endl;
        }
    }

    public:
    Nonogram2 (vector<vector<int>> top, vector<vector<int>> left) {

        width = top.size(), height = left.size();
        grid.resize(height, vector<int>(width));


        for (int i = 0; i < left.size(); i++) {
            int k = 0;
            vector<int> clue = left[i];
            vector<int> line(width);

            for (int j = 0; j < clue.size(); j++) {
                int num = clue[j];

                while (num-->0)
                    line[k++]++;

                k++;
            }

            k = width - 1;
            for (int j = clue.size() - 1; j >= 0; j--) {
                int num = clue[j];

                while (num--> 0)
                    line[k--]++;

                k--;
                /*
                */
            }

            for (int j = 0; j < line.size(); j++) {
                cout << "[";

                if (line[j] == 0) {
                    cout << " ";
                } else {
                    cout << line[j];
                }

                cout << "]";
            }

            cout << "\n";
        }

    }
};

int main(){


    int width = 15, height = 20;
    vector<int> clue {1,5,3};

    vector<vector<int>> top = {{3}, {4}, {2, 2, 2}, {2, 4, 2}, {6}, {3}};
    vector<vector<int>> left = {{4}, {6}, {2, 2}, {2, 2}, {2}, {2}, {2}, {2}, {}, {2}, {2}};

    vector<int> line (width);
    Nonogram2 puzzle (top,left);

    int ng = accumulate (clue.begin(), clue.end(), clue.size() - 1);

    int j = 0, k = width - 1;



}
