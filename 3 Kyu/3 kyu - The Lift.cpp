#include <iostream>
#include <vector>

#include "assert.hpp"

using namespace std;

enum {down = -1, stop, up};

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
    if ((dir == up || dir == stop) && user > floor) return true;
    if (dir == down && user < floor) return true;
    return false;
}
vector<int> the_lift (const vector<vector<int>> &queues, int capacity) {

    const int top = queues.size();
    int floor = 0, dir = stop;

    vector<int> visit = {floor}, lift;
    vector<int>::iterator user;
    vector<vector<int>> scraper = queues;

    while (progress (scraper, floor)) {
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

            if (board == true && dir != stop) flag = true;
            if (board && lift.size() < capacity) {
                lift.push_back (*user);
                scraper[floor].erase (user);
            } else {
                user++;
            }

        }
        //Display::skyscraper (scraper, lift, floor);
        if (flag == true)
            visit.push_back (floor);

        dir = move (scraper, floor, dir);

        floor += dir;
    }
    //Display::vect (visit);
    visit.push_back (floor);
    return visit ;
}


int main () {

    Timer clock;

    vector<vector<int>> queues;
    vector<int> result;
    queues = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{}};
    queues = { { 2, 6 }, {}, { 0 }, { 1, 2 }, { 2, 2, 6, 3 }, {}, {} };

    Test();

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
