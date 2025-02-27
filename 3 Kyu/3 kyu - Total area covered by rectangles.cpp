#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <map>
#include <set>

using namespace std;
using rectangle = vector<int>;

class Assert {

    public:
        static void equals_eq (int res, int exp, std::string str) {

            static int cycle;
            cycle++;
            if (res != exp) {
                std::cout << "test " << cycle << " : " << str << " => ";
                std::cout << "got : " << res << " ";
                std::cout << "exp : " << exp << "\n";
            }
        }
};
void Test();

int calculate (vector<vector<int>> graph) {

    int area = 0;
    set<int> line;
    map<int,vector<int>> hist;

    sort (graph.begin(), graph.end());
    for(int i = 0; i < graph.size(); i++) {
        rectangle r = graph[i];
        int x1 = r[0], x2 = r[2];
        hist[x1].push_back(i);
        hist[x2].push_back(i);
    }

    vector<pair<int, vector<int>>> sweep = {hist.begin(), hist.end()};

    for (int i = 0; i < sweep.size(); i++) {
        auto [pos, rec] = sweep[i];
        //cout << pos << " => ";
        if (!line.empty()) {
            int x2 = pos, x1 = sweep[i - 1].first;

            int cnt = 0;
            map<int,int> bounds;
            vector<vector<int>> interv;

            for (auto index : line) {
                bounds[graph[index][1]]++, bounds[graph[index][3]]--;
            }

            for (auto &[start, end] : bounds){
                cnt += end;
                if (cnt == end) interv.push_back({start, -1});
                if (cnt == 0)   interv.back()[1] = start;
            }

            for (auto curr : interv) {
                int y1 = curr[0], y2 = curr[1];
                area += abs(x2 - x1) * abs(y2 - y1);
            }
        }

        for (auto index : rec) {
            rectangle r1 = graph[index];
            if (r1[0] == pos) line.insert(index);
            if (r1[2] == pos) line.erase(index);
        }
        //cout << endl;
    }

    return area;
}

int main () {
        vector<vector<int>> rect = {{3,3,8,5}, {6,3,8,9},{11,6,14,12}};

    //Assert::equals_eq(calculate({ { 1, 4, 5, 6 },  { 2, 5, 6, 7 },{ 3, 6, 7, 8 },{ 4, 7, 8, 9 },{ 2, 3, 6, 5 },{ 3, 2, 7, 4 },{ 4, 1, 8, 3 }}), 38, "wings");
    // Assert::equals_eq(calculate({ { 7, 1, 11, 7 },  { 8, 0, 12, 3 },  { 8, 4, 13, 5 },  { 9, 5, 14, 8 },{ 10, 2, 15, 6 }}), 53, "intersection 2");

    //sweep(graph);
    /*
       Assert::equals_eq(calculate({{1,2,3,7},{2,1,7,3},{6,2,8,7},{2,6,7,8},{4,4,5,5}}), 37, "circle");
       Assert::equals_eq(calculate({{3,3,6,5},{4,4,6,6},{4,3,7,5},{4,2,8,5},{4,3,8,6},{9,0,11,4},{9,1,10,6},{9,0,12,2},{10,1,13,5},{12,4,15,6},{14,1,16,5},{12,1,17,2}}), 52, "very hard!");
       */
    // Test();
}

void Test () {
    vector<vector<int>> graph {{1, 4, 2, 7}, {1, 4, 2, 6}, {1, 4, 4, 5}, {2, 5, 6, 7}, {4, 3, 7, 6}}; // multiple intersections =>  1 point belongs to 3 rectanles at least.
                                                                                                      //
    Assert::equals_eq(calculate({ { 1, 7, 3, 10 },  { 1, 8, 3, 9 }}), 6, "1 under 2");
    Assert::equals_eq(calculate({ { 6, 7, 9, 10 },  { 7, 8, 8, 9 }}), 9, "nested");
    Assert::equals_eq(calculate({ { 1, 1, 4, 3 },  { 2, 2, 3, 4 }}), 7, "intersection up");
    Assert::equals_eq(calculate({ { 5, 0, 7, 3 },  { 6, 1, 8, 2 }}), 7, "intersetion right");
    Assert::equals_eq(calculate({ { 9, 0, 11, 2 },  { 10, 1, 12, 3 }}), 7, "intersection up right");
    Assert::equals_eq(calculate({ { 13, 5, 15, 6 },  { 14, 4, 16, 7 }}), 7, "intersection of the entire right side");
    Assert::equals_eq(calculate({ { 17, 1, 19, 3 },  { 18, 0, 20, 2 }}), 7, "intersection down right");
    Assert::equals_eq(calculate({ { 13, 1, 16, 3 },  { 14, 0, 15, 2 }}), 7, "intersection down");

    Assert::equals_eq(calculate({ { 1, 7, 4, 10 },  { 2, 7, 4, 9 },  { 3, 7, 4, 9 }}), 9, "nested 2");
    Assert::equals_eq(calculate({ { 1, 3, 4, 6 },  { 2, 1, 5, 4 },  { 3, 2, 6, 5 }}), 20, "intersection 3 rect");

    Assert::equals_eq(calculate({{1,1,2,2},{2,1,3,2},{3,1,4,2},{1,2,2,3},{2,2,3,3},{3,2,4,3},{1,3,2,4},{2,3,3,4},{3,3,4,4}}), 9, "3*3");
    Assert::equals_eq(calculate({{ 1, 1, 6, 6 },{ 1, 3, 4, 6 },{ 2, 3, 4, 6 },{ 2, 4, 5, 6 },{ 3, 5, 4, 6 }}), 25, "intersection");
    Assert::equals_eq(calculate({{1,1,6,6},{2,1,6,6},{3,1,6,6},{4,1,6,6},{5,2,6,5}}), 25, "shift right");
    Assert::equals_eq(calculate({{1,1,7,6},{2,2,8,7},{3,3,9,8},{4,4,10,9},{5,5,11,10}}), 70, "shift right down");
    Assert::equals_eq(calculate({ { 9, 5, 12, 6 },  { 10, 4, 11, 7 }}), 5, "intersection cross");
    Assert::equals_eq(calculate({ { 7, 1, 11, 7 },  { 8, 0, 12, 3 },  { 8, 4, 13, 5 },  { 9, 5, 14, 8 },{ 10, 2, 15, 6 }}), 53, "intersection 2");
    Assert::equals_eq(calculate({{1,2,6,6},{1,3,5,5},{1,1,7,7}}), 36, "pyramid");
    Assert::equals_eq(calculate({{1,1,2,2},{1,1,2,2},{1,1,2,2},{1,1,2,2},{1,1,2,2},{1,1,2,2}}), 1, "one");
    Assert::equals_eq(calculate({ { 1, 4, 5, 6 },  { 2, 5, 6, 7 },{ 3, 6, 7, 8 },{ 4, 7, 8, 9 },{ 2, 3, 6, 5 },{ 3, 2, 7, 4 },{ 4, 1, 8, 3 }}), 38, "wings");
    Assert::equals_eq(calculate({{1,2,3,7},{2,1,7,3},{6,2,8,7},{2,6,7,8},{4,4,5,5}}), 37, "circle");
    Assert::equals_eq(calculate({{3,3,6,5},{4,4,6,6},{4,3,7,5},{4,2,8,5},{4,3,8,6},{9,0,11,4},{9,1,10,6},{9,0,12,2},{10,1,13,5},{12,4,15,6},{14,1,16,5},{12,1,17,2}}), 52, "very hard!");
    Assert::equals_eq(calculate({{2, 2, 17, 2}, {2, 2, 17, 4}, {2, 2, 17, 6}, {2, 2, 17, 8}, {2, 2, 17, 10}, {2, 2, 17, 12}, {2, 2, 17, 14}, {2, 2, 17, 16}, {2, 2, 17, 18}, {2, 2, 17, 20}, {2, 2, 17, 22}, {2, 2, 17, 24}, {2, 2, 17, 26}, {2, 2, 17, 28}}), 390, "waterfall");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int area (rectangle rec) { return abs (rec[2] - rec[0]) * abs (rec[3] - rec[1]); }
bool is_in_rect (int px, int py, rectangle r) { return (px >= r[0] && px <= r[3] && py >= r[1] && py <= r[4]); }
bool overlap (rectangle r1, rectangle r2) {
    return r1[0] <= r2[2] && r2[0] <= r1[2] && r1[1] <= r2[3] && r2[1] <= r1[3];
}
int overarea(rectangle r1, rectangle r2) {
    int width = min(r1[2], r2[2])- max(r1[0], r2[0]);
    int height = min(r1[3], r2[3]) - max(r1[1], r2[1]);
    if (width >= 0 && height >= 0) return width * height;
    return 0;
}
vector<vector<int>> merge(vector<vector<int>> interv) {

    vector<vector<int>> merge;
    sort(interv.begin(), interv.end());

    for (auto curr: interv) {
        if (merge.empty() || merge.back()[1] < curr[0]) {
            merge.push_back(curr);
        } else {
            merge.back()[1] = max (merge.back()[1], curr[1]);
        }
    }
    return merge;
}
