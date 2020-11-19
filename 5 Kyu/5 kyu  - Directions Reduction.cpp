#include <iostream>
#include <vector>
#include <map>

using namespace std;

map <string,int> direction = {{"NORTH", 1},{"SOUTH", -1},{"EAST", 2},{"WEST",-2}};


class DirReduction {
    public:
        static std::vector<std::string> dirReduc(std::vector<std::string> &arr);
};


int main () {

    vector<string> d1 = {"NORTH", "SOUTH", "SOUTH", "EAST", "WEST", "NORTH", "WEST"};
    vector<string> ans1 = DirReduction::dirReduc(d1);

    for (auto now : ans1)
        cout << now;

    return EXIT_SUCCESS;
}

vector<string> DirReduction::dirReduc (vector<string> &entry) {

    auto it = entry.begin(), end = entry.end();
    vector<string> ov;

    while (it != entry.end()) {
        int now = direction[*it], nxt = ((it + 1) < end) ? direction[*(it + 1)] : now;

        if ((nxt + now) == 0) {
            entry.erase(it, it + 2);
            it = entry.begin();
        }
        else {
            // cout << *it;
            it++;
        }
    }

    return ov;
}
