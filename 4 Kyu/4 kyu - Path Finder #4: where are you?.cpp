#include <iostream>
#include <vector>

using namespace std;


std::vector<int> i_am_here (std::string path) {
    static std::vector<int> point (2);
    static int dx = -1, dy = 0;
    std::string num;
    int oldx, dig;

    for (auto &it : path) {
        oldx = dx;
        if (isalpha (it)) {
            if (num.size()) {
                point[0] += stoi(num) * dx, point[1] += stoi(num) * dy;
                num.clear();
            }
            if (it == 'r') {
              dx = dy == 0 ? 0 : dy * 1;
              dy = oldx == 0 ? 0 : oldx * -1;
            } else if (it == 'l') {
              dx = dy == 0 ? 0 : dy * -1;
              dy = oldx == 0 ? 0 : oldx * 1;
            } else if (it == 'R' || it == 'L') {
              dx *= -1, dy *= -1;
            }
        } else {
            num += it;
        }
    }

    if (num.size())
        point[0] += stoi(num) * dx, point[1] += stoi(num) * dy;

    return point;
}

int main () {

    vector<int> point = i_am_here ("r5L2l4");


}
