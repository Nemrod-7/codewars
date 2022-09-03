#include <iostream>
#include <vector>


using namespace std;

int main () {

    int n = 9;
    int mid = n / 2;
    const std::string filled = "\xe2\x96\xa0", empty = "\xe2\x96\xa1";
    std::vector<std::vector<int>> cube (n, std::vector<int> (n));
    std::string os;

    for (int sc = 0; (mid + sc) < n; sc += 2) {
        for (int i = -sc; i <= sc; i++) {
            cube[mid + sc][mid + i] = 1, cube[mid - sc][mid + i] = 1;
            cube[mid + i][mid + sc] = 1, cube[mid + i][mid - sc] = 1;
        }
    }

    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            os += cube[y][x] ? "o " : ". ";

        }

        if (y < n - 1)
            os += "\n";
    }

}
