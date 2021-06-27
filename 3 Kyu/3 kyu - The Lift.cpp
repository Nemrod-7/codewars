#include <iostream>
#include <vector>

using namespace std;

enum {down = -1, stop, up};

void display (vector<int> &visit) {

    for (auto &it : visit) {
        cout << it << ' ';
    }
    cout << endl;
}
void display_mat (const vector<vector<int>> &mat) {
  const int top = mat.size();

  for (int i = 0; i < mat.size(); i++) {
    cout << "{";
    for (int j = 0; j < mat[i].size(); j++) {
      cout << mat[i][j] << ' ';
    }
    cout << "} ";
  }
  cout << endl;
}

bool progress (const vector<vector<int>> &scraper, int floor) {

    for (int i = 0; i < scraper.size(); ++i) {
        for (auto &act : scraper[i]) {
            if (act != i) return true;
        }
    }
    if (floor != 0) return true;
    return false;
}
int move (vector<vector<int>> &scraper, int floor, int dir) {
    if (floor == 0) return up;
    if (floor == scraper.size() - 1) return down;
    //if (dir == stop) return up;
    return dir;
}
bool canmove (int user, int floor, int dir) {
    if (dir == up && user > floor) return true;
    if (dir == down && user < floor) return true;
    return false;
}
vector<int> the_lift (const vector<vector<int>> &queues, int capacity) {

    const int top = queues.size();
    int floor = 0, dir = stop;
    bool flag;
    vector<int> visit = {floor}, lift;
    vector<int>::iterator user;
    vector<vector<int>> scraper = queues;

    while (progress(scraper, floor)) {
        flag = false;

        user = lift.begin();

        while (user != lift.end()) {
            if (*user == floor) {
                scraper[floor].push_back (*user);
                lift.erase (user);
                flag = true;
            }
            else user++;
        }

        user = scraper[floor].begin();

        while (user != scraper[floor].end()) {
            if (canmove (*user, floor, dir) && lift.size() < capacity) {
                lift.push_back (*user);
                scraper[floor].erase (user);
                flag = true;
            }
            else user++;
        }
        //display (lift);
        if (flag == true)
            visit.push_back (floor);

        dir = move (scraper, floor, dir);
        //display_mat (scraper);
        floor += dir;
    }

    visit.push_back (floor);
    return visit ;
}

int main () {

    std::vector<std::vector<int>> queues { {}, {}, {5,5,5}, {}, {}, {}, {} };

    queues = { {}, {0}, {}, {}, {2}, {3}, {} };
    vector<int> result = the_lift (queues, 5);

    display (result);
}

void Test () {
  std::vector<std::vector<int>> queues; std::vector<int> result;

    queues = { {}, {}, {5,5,5}, {}, {}, {}, {} };
    result = {0, 2, 5, 0};
    //Assert::That(the_lift(queues, 5), Equals(result));

    queues = { {}, {}, {1,1}, {}, {}, {}, {} };
    result = {0, 2, 1, 0};
    //Assert::That(the_lift(queues, 5), Equals(result));

    queues = { {}, {3}, {4}, {}, {5}, {}, {} };
    result = {0, 1, 2, 3, 4, 5, 0};
    //Assert::That(the_lift(queues, 5), Equals(result));

    queues = { {}, {0}, {}, {}, {2}, {3}, {} };
    result = {0, 5, 4, 3, 2, 1, 0};
    //Assert::That(the_lift(queues, 5), Equals(result));
}
