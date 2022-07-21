#include <iostream>
#include <vector>

//#include "../../../templates/Assert.hpp"

using namespace std;

enum {down = -1, stop, up};

bool progress (const vector<vector<int>> &mat, const vector<int> &lift) {

    if (lift.size()) return true;

    for (size_t i = 0; i < mat.size(); ++i) {
        for (size_t j = 0; j < mat[i].size(); j++) {
            if (mat[i][j] != static_cast<int> (i))
                return true;
        }
    }

    return false;
}
bool valid (int user, int floor, int direction, int top) {
    if (user > floor && (direction == up || floor == 0)) return true;
    if (user < floor && (direction == down || floor == top)) return true;
    return false;
}
vector<int> the_lift (const vector<vector<int>> &queues, int capacity) {

    int top = queues.size() - 1, direction = up, floor = 0;
    vector<int> record {0}, lift;
    vector<int>::iterator it;
    vector<vector<int>> skyscraper = queues;

    while (progress (skyscraper, lift)) {

        bool visited = false, board;

        for (it = lift.begin(); it != lift.end();) {
            if (*it == floor) {
                lift.erase(it);
                visited = true;
            } else {
                it++;
            }
        }

        for (it = skyscraper[floor].begin(); it != skyscraper[floor].end();) {
            board = valid (*it, floor, direction, top);

            if (board) visited = true;
            if (board && lift.size() < capacity) {
              lift.push_back (*it);
              skyscraper[floor].erase (it);
            } else {
                it++;
            }
        }

        if (visited && record.back() != floor)
            record.push_back (floor);

        floor += direction;
        if (floor == 0 || floor == top) direction = -direction;
    }

    if (record.back() != 0) record.push_back (0);

    return record;
}

int main () {

    Timer clock;

    vector<vector<int>> queues;
    vector<int> result, expected;

    Test();

    clock.stop();
    clock.get_duration();
}

void Test () {
    vector<vector<int>> queues; vector<int> result, expected;

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

    queues = {{},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    result = the_lift (queues, 5);
    Assert::That (result, {0, 6, 5, 4, 3, 2, 1, 0, 5, 4, 3, 2, 1, 0, 4, 3, 2, 1, 0, 3, 2, 1, 0, 1, 0});

    queues = { {}, {}, { 4, 0, 3 }, {}, {} };
    result = the_lift(queues, 4);
    Assert::That (result, {0,2,3,4,2,0});

    queues = { { 1, 4, 4, 1 }, { 3, 2 }, { 3, 4, 0, 3 }, {}, {} };
    result = the_lift(queues, 4);
    Assert::That (result, {0, 1, 2, 3, 4, 2, 0, 2, 3, 4, 0 });

    queues = { { 3 }, {}, { 1, 4 }, { 1, 4, 0, 0 }, {}, { 4 } };
    result = the_lift (queues, 5);
    Assert::That (result,{ 0, 2, 3, 4, 5, 4, 3, 2, 1, 0 });

    result = the_lift ({{}, {}, {4,4,4,4}, {}, {2,2,2,2}, {}, {}},2);
    Assert::That (result, {0, 2, 4, 2, 4, 2, 0});

    result = the_lift ({{}, {11}, {8,6,4}, {}, {2}, {11,9}, {2,9}, {3,4}, {1,6,10,11}, {12,10,4,11,3}, {7,12,8}, {3,2,3,9}, {4,10,2,10,4}},2);
    expected = {0,1,2,5,6,8,9,10,11,12,11,10,9,8,7,6,4,3,2,4,5,6,8,9,11,12,11,10,9,8,7,6,2,1,5,8,9,10,11,12,11,9,8,7,6,4,3,9,11,9,8,7,6,3,2,11,9,8,7,6,4,3,8,7,6,4,2,0};
    Assert::That(result, expected);//queues = {{}, {}, {4,4,4,4}, {}, {2,2,2,2}, {}, {}};
}
