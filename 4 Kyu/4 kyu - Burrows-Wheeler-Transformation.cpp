#include <iostream>
#include <string>
#include <vector>

#include <algorithm>
#include <map>

using namespace std;
using psi = std::pair<std::string, int>;

pair<string, int> encode (const std::string &src) {
    vector<string> table;
    pair<string, int> out;

    for (size_t i = 0; i < src.size(); ++i)
        table.push_back(src.substr (i, src.size()) + src.substr (0, i));

    sort (table.begin(), table.end());

    for (string &it : table)
        out.first += it.back();

    out.second = find (table.begin(), table.end(), src) - table.begin();
    return out;
}
string decode (const string &src, int pos) {
    if (src.size() == 0) return "";

    string str2 = src, os;
    int i, ref, next = src.size() - 1;
    map<char, int> cmap1, cmap2;
    vector<int> idx1 (src.size()), idx2(src.size());

    sort (str2.begin(), str2.end());

    for (i = 0; i != src.size(); ++i) {
        idx1[i] = cmap1[src[i]]++;
        idx2[i] = cmap2[str2[i]]++;
    }

    os += str2[pos];
    ref = idx2[pos];

    while (next-->0) {
        i = 0;

        while (src[i] != os.back() || idx1[i] != ref) i++;

        ref = idx2[i];
        os += str2[i];
    }

    return os;
}

int main () {

    return 0;
}
