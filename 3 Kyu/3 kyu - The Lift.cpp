#include <iostream>
#include <vector>

#include "assert.hpp"

using namespace std;

enum {down = -1, stop, up};

class Lift {

    private :
        vector<int>::iterator it;
        bool flag;
    public :
        int direction, floor;
        size_t top, capacity;
        vector<int> V;

        Lift (int maxc, int size) {
            direction = floor = stop;
            capacity = maxc;
            top = size - 1;
        }
        int move () {
            if (floor == 0) direction = up;
            if (floor == top) direction = down;
            floor += direction;
            return floor;
        }

        bool eligible (int user) {
            if ((direction == up || direction == stop) && user > floor) return true;
            if (direction == down && user < floor) return true;
            return false;
        }
        bool embark (vector<vector<int>> &skyscraper) {
            flag = false;

            it = skyscraper[floor].begin();

            while (it != skyscraper[floor].end()) {
                flag = eligible (*it);
                //cout << floor << ' ' << *it << ' ' << flag << endl;
                //if (flag == true && direction != stop) flag = true;
                if (flag && V.size() < capacity) {
                    V.push_back (*it);
                    skyscraper[floor].erase (it);
                } else {
                    it++;
                }
            }

            return flag;
        }
        bool debark (vector<vector<int>> &skyscraper) {
            flag = false;
            it = V.begin();

            while (it != V.end()) {
                if (*it == floor) {
                    skyscraper[floor].push_back (*it);
                    V.erase (it);
                    flag = true;
                } else {
                    it++;
                }
            }
            return flag;
        }
};

class Display {
    public :
        static void vect (const vector<int> &visit) {
            cout << "[";
            for (auto &it : visit)
                cout << setw(2) << it;
            cout << "]";
        }
        static void skyscraper (const vector<vector<int>> &mat, const Lift &lift) {
            size_t size = 0, index = mat.size();

            for (auto &it : mat)
                size = max (size, it.size());

            while (index-->0)  {
                cout << "[";
                for (int j = 0; j < size; j++) {
                    if (mat[index].size() > j) {
                      cout << setw(2) << mat[index][j];
                    } else {
                      cout << "  ";
                    }
                }
                cout << "]";
                if (index == lift.floor) {
                    cout << "->";
                    vect (lift.V);
                }
                cout << endl;
            }
            cout << "-";
            for (int j = 0; j < size; j++) cout << "--";
            cout << "-\n\n";
      }

};
bool progress (const vector<vector<int>> &scraper, int floor) {

    for (int i = 0; i < scraper.size(); ++i) {
        for (auto &act : scraper[i]) {
            if (act != i) return true;
        }
    }
    if (floor != 0) return true;
    return false;
}
vector<int> the_lift (const vector<vector<int>> &queues, int capacity) {

    Lift lift (capacity, queues.size());
    int floor = 0;

    vector<int> visited = {floor};
    vector<vector<int>> skyscraper = queues;
    int index = 6;

    while (index-->0) {
        bool movement = (lift.embark (skyscraper) || lift.debark (skyscraper));

        if (movement)
            visited.push_back (lift.floor);

        lift.move();
        //Display::skyscraper (skyscraper, lift);
    }
    //Display::vect (visit);
    visited.push_back (lift.floor);
    return visited;
}

int main () {

    Timer clock;

    vector<vector<int>> queues;
    vector<int> result;
    queues = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{}};
    queues = { { 3 }, {}, { 1, 4 }, { 1, 4, 0, 0 }, {}, { 4 } }; // result = {0,5,4,3,2,1,0}


    // Display::vect (the_lift(queues, 5));
    //Test();

    // [ 0, 6, 5, 4, 3, 2, 1, 0, 5, 4, 3, 2, 1, 0, 4, 3, 2, 1, 0, 3, 2, 1, 0, 1, 0 ]
    clock.stop();
    clock.get_duration();
}

void Test () {
    vector<vector<int>> queues; vector<int> result;

    queues = { {}, {}, {5,5,5}, {}, {}, {}, {} };
    result = {0, 2, 5, 0};
    Assert::That(the_lift(queues, 5), Equals(result));

    queues = { {}, {}, {1,1}, {}, {}, {}, {} };
    result = {0, 2, 1, 0};
    Assert::That(the_lift(queues, 5), Equals(result));

    queues = { {}, {3}, {4}, {}, {5}, {}, {} };
    result = {0, 1, 2, 3, 4, 5, 0};
    Assert::That(the_lift(queues, 5), Equals(result));

    queues = { {}, {0}, {}, {}, {2}, {3}, {} };
    result = {0, 5, 4, 3, 2, 1, 0};
    Assert::That(the_lift(queues, 5), Equals(result));

    queues = { { 2, 6 }, {}, { 0 }, { 1, 2 }, { 2, 2, 6, 3 }, {}, {} };
    result = the_lift (queues, 5);
    Assert::That (result, { 0, 2, 4, 6, 4, 3, 2, 1, 0 });
}
/*
vector<int> the_lift (const vector<vector<int>> &queues, int capacity) {

    const int top = queues.size();
    int floor = 0, dir = stop;

    lift_proto lft2 (capacity);

    vector<int> visit = {floor}, lift;
    vector<int>::iterator user;
    vector<vector<int>> scraper = queues;
    int index = 6;

    while (index-->0) {
    // while (progress (scraper, floor)) {
        bool flag = false, board = false;

        user = lift.begin();

        while (user != lift.end()) {
            if (*user == floor) {
                scraper[floor].push_back (*user);
                lift.erase (user);
                flag = true;
            } else {
                user++;
            }
        }

        user = scraper[floor].begin();

        while (user != scraper[floor].end()) {
            board = canmove (*user, floor, dir);
            cout << floor << ' ' << *user << ' ' << board << endl;
            if (board == true && dir != stop) flag = true;
            if (board && lift.size() < capacity) {
                lift.push_back (*user);
                scraper[floor].erase (user);
            } else {
                user++;
            }

        }
        Display::skyscraper (scraper, lift, floor);
        if (flag == true)
            visit.push_back (floor);

        dir = move (scraper, floor, dir);

        floor += dir;
    }
    //Display::vect (visit);
    visit.push_back (floor);
    return visit ;
}

vector<int> the_lift2 (const vector<vector<int>> &queues, int capacity) {

    //const int top = queues.size();
    int floor = 0, dir = stop;
    bool stopped, board;
    vector<int> visited = {floor}, lift;
    vector<int>::iterator user;
    vector<vector<int>> scraper = queues;
    int index = 8;

    //while (index-->0) {
    while (progress(scraper, floor)) {
        stopped = false;
        user = lift.begin();

        while (user != lift.end()) {
            if (*user == floor) {
                scraper[floor].push_back (*user);
                lift.erase (user);
                stopped = true;
            } else {
                user++;
            }
        }
        user = scraper[floor].begin();

        while (user != scraper[floor].end()) {

            board = eligible (*user, floor, dir);

            if (board == true && lift.size() < capacity) {
                lift.push_back (*user);
                scraper[floor].erase (user);
            } else {
                user++;
            }
        }
        //display (lift);
        Display::skyscraper (scraper, lift, floor);

        if (stopped == true || board == true)
            visited.push_back (floor);

        dir = move (scraper, floor, dir);
        //display_mat (scraper);
        floor += dir;
    }

    visited.push_back (floor);
    //Display::vect (visited);
    return visited ;
  }
*/
