#include <iostream>
#include <vector>

using namespace std;

void generateLatinSquare(int n) {
    int firstend = n + 1;

    for (int i = 1; i <= n; i++) {
        int cell = firstend;

        while (cell <= n) {
            cout << cell << " ";
            cell++;
        }

        for (int secstart = 1; secstart < firstend; secstart++){
            cout << secstart << " ";
        }
        firstend--;

        cout << endl;
    }
    cout << endl;
}

int main () {

    generateLatinSquare(4);

}
