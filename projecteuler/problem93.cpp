#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;


void recurse (vector<int> set) {
    vector<char> op = {'+','-','*','/'};

    do {
          cout << set[0] << op[0] << set[1] << op[1] << set[2] << op[2] << set[3] << endl;
    } while (next_permutation(op.begin(), op.end()));
}

int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now (), end;

    vector<int> set = {1,2,3,4};
    vector<char> op = {'+','-','*','/'}, br = {'(',')'};

    do {
        recurse (set);
    } while (next_permutation(set.begin(), set.end()));



    end = chrono::steady_clock::now ();
    std::chrono::duration<double> elapsed = end - alpha;
    cout << "\nDuration " << fixed << elapsed.count() << " ms" << endl;
}
