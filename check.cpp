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

std::vector<std::string> tokenize (const std::string &expr) {
    std::regex re ("\\[[0-9]+\\]");
    std::sregex_token_iterator it (expr.begin(), expr.end(), re);
    return std::vector<std::string> (it, std::sregex_token_iterator());
}

int main () {

    string txt, buffer;
    ifstream iss ("test1");

    while (iss >> buffer)
        txt += buffer;

      auto vec = tokenize(txt);

      cout << vec.size();

    cout << "exit\n";

}
