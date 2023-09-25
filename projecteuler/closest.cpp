#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <chrono>

using namespace std;

struct point { int64_t x, y; };

class generator {
    private :
        const int64_t mod = 50515093;
        vector<int64_t> seq;
    public :
        generator () { seq.push_back(290797); }

        int64_t operator [] (int n) {

            const int start = seq.size();
            for (int i = start; i <= n; i++) {
                int64_t s0 = seq.back();
                int64_t sn = (s0 * s0) % mod;
                seq.push_back(sn);
            }

            return seq[n];
        }
        size_t size() { return seq.size(); }
};

bool horiz (const point &a, const point &b) {
    if (a.x < b.x) return true;
    return false;
}
bool vertical (const point &a, const point &b) {
    if (a.y < b.y) return true;
    return false;
}

double distance (const point &a, const point &b) { return  std::hypot(a.x - b.x, a.y - b.y); }
vector<point> mindist (const vector<point> &a, const vector<point> &b) {
    if (distance (a[0],a[1]) < distance (b[0],b[1])) {
        return a;
    } else {
        return b;
    }
}
vector<point> bruteforce (vector<point> graph) {
    const int size = graph.size();
    double maxv = numeric_limits<double>::max(), dist;
    vector<point> vec {{0,0},{0,0}};

    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            dist = distance (graph[i],graph[j]);

            if (dist < maxv) {
                maxv = dist;
                vec[0] = graph[i];
                vec[1] = graph[j];
            }
        }
    }

    return vec;
}
vector<point> stripdist (vector<point> graph, vector<point> close) {
    const int size = graph.size();
    double minv = distance(close[0],close[1]), dist;
    vector<point> next = close;
    sort (graph.begin(),graph.end(),vertical);

    for (int i = 0; i < size; i++) {
        int j = i + 1;
        while (j < size && abs(graph[i].y - graph[j].y) < minv) {
            dist = distance (graph[i],graph[j]);

            if (dist < minv) {
                minv = dist;
                next[0] = graph[i];
                next[1] = graph[j];
            }
            j++;
        }
    }

    return next;
}
vector<point> closetp (vector<point> graph) {
    const int size = graph.size();
    const int mid = size / 2;

    if (size < 4) return bruteforce (graph);
    const point center = graph[mid];
    vector<point> left, right, strip;

    for (const point &p : graph) {
        if (p.x < center.x) {
            left.push_back(p);
        } else {
            right.push_back(p);
        }
    }

    vector<point> close = mindist (closetp (left), closetp (right));
    double minv = distance(close[0], close[1]);

    for (const point &p : graph) {
        if (abs(p.x - center.x) < minv) {
            strip.push_back(p);
        }
    }

    return stripdist (strip, close);
}
vector<point> closest_pair (vector<point> graph) { // problem 816 ?

    sort (graph.begin(), graph.end(), horiz);
    return closetp (graph);
}

int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now (), end;

    generator seq;

    int lim = 2000000;
    seq[lim * 2];

    vector<point> graph;

    for (int i = 0; i < lim; i++) {
        int64_t x = seq[2*i], y = seq[2*i+1];
        graph.push_back({x,y});
        //grap2.push_back({x,y});
        //printf("%zu %zu\n", x, y);
    }

    sort (graph.begin(),graph.end(), horiz);


    vector<point> pt = closetp (graph);
    double res = distance (pt[0],pt[1]);

    printf ("%.09f\n", res);
    /*
    */

    end = chrono::steady_clock::now ();
    std::chrono::duration<double> elapsed = end - alpha;
    cout << "\nDuration " << fixed << elapsed.count() << " ms" << endl;
}

void showpoint (const point &p) {
    printf ("[%zu %zu]\n", p.x, p.y);
}
void showvec(const vector<point> &v) {
    for (auto p : v) {
        showpoint (p);
    }
}
