#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

std::string clean(std::string &str) {
    std::string os;
    for (auto &ch : str) {
        if (isalnum(ch) || ispunct(ch)) os += ch;
    }
    return os;
}
std::vector<std::string> tokenize (const std::string &src, char delim) {
    std::istringstream iss (src);
    std::string token;
    std::vector<std::string> v;

    while (getline (iss, token, delim))
        v.push_back (clean(token));

    return v;
}

int main () {

    string buffer;
    ifstream ifs ("table");

    getline(ifs,buffer);
    getline(ifs,buffer);
    getline(ifs,buffer);

    while (getline(ifs,buffer)) {
        vector<string> cell = tokenize(buffer, ' ');

        cout << left;
        cout << "[" << setw(3) << cell[0] << "]";
        cout << "[" << setw(13) << cell[1] << "]";
        cout << "[" << setw(2) << cell[2] << "]";
        cout << "[" << setw(2) << cell[3] << "]";

        cout << "\n";

        // cout << buffer << "\n";
    }
}
