#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using vertex = pair<int,vector<int>>;

void read (int byte) {
    for (int i = 0; i < 10; i++) {
      if ((byte >> i &1) == true) {
          cout << i << " ";
      }
    }
}
void mark (int &byte, int pos) { byte |= 1 << pos; }
void clear (int &byte, int pos) { byte &= ~(1 << pos); }

vector<vertex> mkgraph (vector<int> input) {
    vector<int> digit (10);
    vector<vertex> graph;
    //cout << a << " => ";
    for (int i = 0; i < input.size(); i++) {
        int a = (input[i] / 100) % 10, b = (input[i] / 10) % 10, c = input[i] % 10;
        digit[a] |= 1 << b;
        digit[a] |= 1 << c;
        digit[b] |= 1 << c;
    }

    for (int i = 0; i < 10; i++) {
        vector<int> node;

        for (int j = 0; j < 10; j++) {
            if ((digit[i] >> j &1) == true) {
                node.push_back(j);
            }
        }
        if (node.size())
            graph.push_back({i, node});
    }
    return graph;
}

int main () {

  vector<int> input = {319, 680, 180, 690, 129, 620, 762, 689, 762, 318, 368, 710, 720, 710, 629, 168, 160, 689, 716, 731, 736, 729, 316, 729, 729, 710, 769, 290, 719, 680, 318, 389, 162, 289, 162, 718, 729, 319, 790, 680, 890, 362, 319, 760, 316, 729, 380, 319, 728, 716};
  vector<int> pass;

  for (int i = 0; i < input.size(); i++) {
      int a = (input[i] / 100) % 10, b = (input[i] / 10) % 10, c = input[i] % 10;
      pass.push_back(input[i]);

  }

  sort(pass.begin(), pass.end());

  for (auto it : pass) {
    cout << it << ' ';
  }
}
