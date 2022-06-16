#include <iostream>
#include <vector>

#include "../../templates/Assert.hpp"

using namespace std;

enum {down = -1, stop, up};

bool progress (const vector<vector<int>> &mat, const vector<int> &lift) {

    if (lift.size()) return true;

    for (size_t i = 0; i < mat.size(); ++i) {
        for (size_t j = 0; j < mat[i].size(); j++) {
            if (mat[i][j] != static_cast<int>(i))
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
///////////////////////////////////////////////////////////////////////////////
/*
class elevator {
private :
vector<int>::iterator it;
int direction, top;

bool rules (int user) {
if (user > floor && (direction == up || floor == 0)) return true;
if (user < floor && (direction == down || floor == top)) return true;
return false;
}
void transfer (vector<int> &from, vector<int> &to, vector<int>::iterator &it) {
to.push_back (*it);
from.erase (it);
}
public :
int floor;
size_t capacity;
vector<int> V;

elevator (const vector<vector<int>> &queues, int maxc) {
top = queues.size() - 1;
direction = up, capacity = maxc, floor = 0;
}

void move () {
floor += direction;
if (floor == 0 || floor == top) direction = -direction;
}
bool upgrade (vector<vector<int>> &building) {
bool stopped = false, board;

for (it = V.begin(); it != V.end();) {
if (*it == floor) {
transfer (V, building[floor], it);
stopped = true;
} else {
it++;
}
}

for (it = building[floor].begin(); it != building[floor].end();) {
board = rules (*it);
//cout << floor << ' ' << btm << ' ' << *it << ' ' << board << endl;
if (board) stopped = true;
if (board && V.size() < capacity) { //
transfer (building[floor], V, it);
} else {
it++;
}
}
//cout << floor << ' ' << in << ' ' << out << endl;
return stopped;
}
};
class Display {
public :
static void show (const vector<vector<int>> &src, int capacity) {

cout << "the_lift ({";

for (size_t i = 0; i < src.size(); i++) {
cout << '{';
for (size_t j = 0; j < src[i].size(); j++) {
cout << src[i][j];
if (j != src[i].size() - 1) cout << ',';
}
cout << '}';
if (i != src.size () - 1) cout << ", ";
}
cout << "}," << capacity << ");\n";
}
static void vect (const vector<int> &visit) {
cout << "[";
for (auto &it : visit)
cout << setw(3) << it;
cout << "]";
}
static void skyscraper (const vector<vector<int>> &mat, const elevator &lift) {
int size = 0, index = mat.size();

for (auto &it : mat) //if (it.size() > size) size = it.size();
size = max (size, static_cast<int> (it.size()));
//cout << lift.floor << ' ';
while (index-->0)  {
cout << "[";
for (int j = 0; j < size; j++) {
if (mat[index].size() > static_cast<int> (j)) {
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

bool progress (const vector<vector<int>> &mat, int floor) {

    for (int i = 0; i < mat.size(); ++i) {
        for (auto &act : mat[i]) {
            if (act != i) return true;
        }
    }

    if (floor != 0) return true;
    return false;
}
vector<int> the_lift (const vector<vector<int>> &queues, int capacity) {

    show (queues, capacity);
    elevator lift (capacity, queues.size());

    vector<int> record = {0};
    vector<vector<int>> skyscraper = queues;

    while (progress (skyscraper, lift.floor)) {

        bool visited = lift.upgrade(skyscraper);

        if (visited)
            record.push_back (lift.floor);

        //Display::skyscraper (skyscraper, lift);

        lift.move();

    }
    //Display::vect (visit);
    record.push_back (lift.floor);
    return record;
}

*/
