
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

struct Point { double x, y; };

class vec {
    public:
        // double dot (const pair<double,double> &u, const pair<double,double> &v) { return u.x * v.x + u.y * v.y; }
        // double norm (double v) { return sqrt (dot (v,v)); }
        // double distance (const pair<double,double> &a, const pair<double,double> &b) { norm}
};

int rectanglearea2 (const vector<vector<Point>> &graph) {

    double area = 0.0;
    map<double,vector<int>> hist;

    for (int i = 0; i < graph.size(); i++) {
        double x1 = graph[i][0].x, x2 = graph[i][1].x;
        hist[x1].push_back(i);
        hist[x2].push_back(i);
    }

    set<double> line;
    vector<pair<double, vector<int>>> sweep ({hist.begin(), hist.end()});

    for (int i = 0; i < sweep.size(); i++) {
        auto [x, rec] = sweep[i];

        if (!line.empty()) {
            double x1 = sweep[i-1].first;

            int cnt = 0;
            map<double,int> yaxis; // count of bounds
            vector<pair<double,double>> interval;

            for (auto &index : line) {
                double y0 = graph[index][0].y, y1 = graph[index][1].y;
                yaxis[y0]++, yaxis[y1]--;
            }

            for (auto &[start,end] : yaxis) {
                cnt += end;
                if (cnt == end) interval.push_back({start, -1});
                if (cnt == 0)   interval.back().second = start;
            }

            for (auto &[y0, y1] : interval) {
                area += abs(x1 - x) * abs(y1 - y0);
            }
        }

        for (int j = 0; j < rec.size(); j++) {
            int index = rec[j];
            vector<Point> rect = graph[index];

            if (rect[0].x == x) line.insert(index);
            if (rect[1].x == x) line.erase(index);
        }
    }

    return area;
}
int main(void) {

    vector<vector<int>> graph = {{3,3,8,5},{6,3,8,9},{11,6,14,12}};



    cout << "\nend\n";
    return 0;
}
