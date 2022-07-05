#include <iostream>
#include <vector>
#include <queue>

using namespace std;

#include <string>
const string space = " ",tab =  "\t", nline = "\n";

struct node{
    int next, c;
    bool operator<(const node &o) const{
        return c > o.c;
    }
};

struct vertex {
    float dist, move;
    pair<int,int> p;

    bool operator< (const vertex &b) const {
        return (move == b.move) ?  dist > b.dist : move > b.move;
    }
};

int dijkstra(std::vector<node> edge[5], int src) {
    std::priority_queue<node> Q;

    node tmp;

        tmp = Q.top();
        Q.pop();
        int u = tmp.next;

        Q.push(tmp);

     return 0;
}

int main () {


    //cout << whitespace_number (0);
}
