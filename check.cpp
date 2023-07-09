#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

using namespace std;

void check (int n, vector<vector<int>> ve) {

    vector<int> hist (n);

    for (int i = 0; i < ve.size(); i++) {
        if (ve[i].size() == 0) {
            cout << "empty";
        }
        for (int j = 0; j < ve[i].size(); j++) {
            int pos = ve[i][j];
            hist[pos]++;
        //    cout << pos << ' ';
            if (ve[i][j] == 0) {
            }
            if (ve[i][j] >= n) {
          //      cout << ve[i][j] << ' ';
            }
        }
    }

    for (int i = 0; i < n; i++) {
        // cout << hist[i] << ' ';
    }
}

std::vector<std::string> tokenize (const std::string &expr, const std::string &recon) {
    std::regex re (recon);
    std::sregex_token_iterator it (expr.begin(), expr.end(), re);
    return std::vector<std::string> (it, std::sregex_token_iterator());
}

int main () {

    string txt, buffer;
    ifstream iss ("test1");
    vector<vector<int>> base;
    int n;

    iss >> n;

    while (iss >> buffer)
        txt += buffer;

    vector<string> lines = tokenize (txt, "\\[([0-9]+,?)+\\]");

    for (auto &ve : lines) {
        vector<string> tup = tokenize (ve, "[0-9]+");
        vector<int> line;
        for (auto &cell : tup) {
          line.push_back (stoi(cell));
        }
        base.push_back(line);
    }


    cout << n;

    cout << "exit\n";

}
