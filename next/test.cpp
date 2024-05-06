#include <iostream>
#include <vector>

using namespace std;

void backtrack(vector<char> line, int index, int open, int closed) {
    int n = line.size() / 2;
    // cout << n << ' ';
    if (index == n && (open - closed) == 0) {
        for (auto &ch : line) {
          cout << ch;
        }
        cout << endl;
    }

    if (open < n) {
        line[index] = '(';
        backtrack(line, index + 1 , open + 1, closed);
    }
    if ((open - closed) > 0  && closed < n) {
        line[index] = ')';
        backtrack(line, index + 1, open, closed + 1);
    }
}

int main () {

    const int n = 3;
    vector<char> line (n * 2);

    backtrack(line, 0, 0, 0);

}
