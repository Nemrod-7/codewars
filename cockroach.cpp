#include <iostream>

#include <array>
#include <string>
#include <vector>

using namespace std;

bool is_inside (int x, int y, int width, int height) { return x >= 0 && y >= 0 && x < width && y < height; }

std::array<int, 10> cockroaches(const std::vector<std::string> &room) {
  array<int, 10> freq;
  vector<pair<int,int>> hole;
  vector<string> curr = room, next = curr;

    for (int i = 0; i < curr.size(); i++) {
        for (int j = 0; j < curr[i].size(); j++) {
            char ch = curr[i][j];
            pair<int,int> nx = {j,i};

            if (ch == 'U') {
                // next[i][j] = ' ';
                // next[i-1][j] = 'U';
                nx = {j,i-1};
            } else if (ch == 'D') {
                // next[i][j] = ' ';
                // next[i+1][j] = 'D';
                nx = {j,i+1};
            } else if (ch == 'R') {
                // next[i][j] = ' ';
                // next[i][j+1] = 'R';
                nx = {j+1,i};

            } else if (ch == 'L') {
                // next[i][j] = ' ';
                // next[i][j-1] = 'L';
                nx = {j-1,i};
            }

            next[i][j] = ' ';
            next[nx.second][nx.first] = ch;
        }
    }
    curr = next;

    for (int i = 0; i < room.size(); i++) {
        for (int j = 0; j < room[i].size(); j++) {
            cout << next[i][j];
        }
        cout << "\n";
    }

    return freq;
}

int main () {

      vector<string> room = {
    "+----------------0---------------+",
    "|                                |",
    "|                                |",
    "|          U        D            |",
    "|     L                          |",
    "|              R                 |",
    "|           L                    |",
    "|  U                             1",
    "3        U    D                  |",
    "|         L              R       |",
    "|                                |",
    "+----------------2---------------+"
};
// , {1,2,2,5,0,0,0,0,0,0});

    // There are 0 or more cockroaches in the room
    // Cockroaches firstly run in a straight line in the direction they are facing
    // When they hit a wall they always turn LEFT and then they follow the wall until they can find a hole to crawl into!
    // Cockroaches do not bump into each other
    // There are no cockroaches outside the room

    cockroaches(room);

}
