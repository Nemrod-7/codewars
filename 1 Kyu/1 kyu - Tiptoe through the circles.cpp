
#include <vector>
#include <queue>
#include <map>
#include <cmath>

#include "graph.hpp"
#include "Assert.hpp"
// #include "plot.hpp"

using namespace std;

struct node { Point p; int id; };

struct vertex {
    double heur, dist;
    node nu;
    int ntg;
};
struct comp {
    bool operator() (const vertex &a, const vertex &b ) {
        return a.heur > b.heur;
    }
};

class tangraph {
    public:
        vector<pair<node,node>> vgraph;
        vector<vector<int>> edges;
        vector<vector<Point>> inter;

        tangraph (vector<Circle> circles) {
            const int size = circles.size();

            edges.resize (size);
            inter.resize (size);

            for (int i = 0; i < size; i++) {
                Circle c1 = circles[i];
                for (int j = i + 1; j < size; j++) {
                    Circle c2 = circles[j];

                    for (auto &p1 : hcenter (c1, c2)) {
                        if (!inside_circle (p1, c1)) { // if the center lies outside the circles
                            vector<Point> t1 = tangent (p1, c1), t2 = tangent (p1, c2);

                            if (collision (t1[0], t2[0], circles) == false) {
                                node n1 = {t1[0], i}, n2 = {t2[0], j};
                                vgraph.push_back ({n1,n2});

                                edges[i].push_back(vgraph.size() - 1);
                                edges[j].push_back(vgraph.size() - 1);
                            }

                            if (collision (t1[1], t2[1], circles) == false) {
                                node n1 = {t1[1], i}, n2 = {t2[1], j};
                                vgraph.push_back ({n1,n2});

                                edges[i].push_back(vgraph.size() - 1);
                                edges[j].push_back(vgraph.size() - 1);
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < size; i++) {         // search for circles intersection
                for (int j = i + 1; j < size; j++) {
                    vector<Point> cip = circles_intersection (circles[i], circles[j]);

                    for (auto &p : cip) {
                        inter[i].push_back(p);
                        inter[j].push_back(p);
                    }
                }
            }
        }
};

int identify (const Point &p1, const vector<Circle> &circles) {

    for (size_t i = 0; i < circles.size(); i++) {
        auto &[p2,rad] = circles[i];
        if (abs (distance (p1, p2) - rad) < epsilon) {
            return i;
        }
    }
    return -1;
}
vector<Point> find_tangent (const Point &p1, const vector<Circle> &circles) { // find a valid tangent from a point
    const int size = circles.size();
    vector<Point> vect;

    for (int i = 0; i < size; i++) {
        for (Point &p2 : tangent (p1, circles[i])) {
            if (collision (p1,p2, circles) == false) {
                vect.push_back(p2);
            }
        }
    }

    return vect;
}

void display (Point start, Point exit, vector<Circle> circles) {
    cout << "({" << start.x << "," << start.y << "},{";
    cout << exit.x << "," << exit.y << "},{";

    for (Circle c : circles) {
        cout << "{" << c.ctr.x << "," << c.ctr.y  << "," << c.r << "},";
    }
    cout << "});\n";
}

double shortest_path_length (Point start, Point exit, vector<Circle> circles) {
    // display(start,exit,circles);
    if (collision (start, exit, circles) == false) {
        return distance (start,exit);
    }

    double angle[2], minv[2];

    auto [vgraph,edges,inter] = tangraph (circles);
    priority_queue<vertex,vector<vertex>,comp> q1;

    for (auto &p : find_tangent (exit, circles)) { // connect exit point vgraph to the tan visibility graph
        int id = identify (p, circles);
        node n1 = {p, id}, n2 = {exit, 0};

        vgraph.push_back({n1, n2});
        edges[id].push_back(vgraph.size() - 1);
    }

    for (auto &p : find_tangent (start,circles)) { // connect start point vgraph to the tan visibility graph
        int id = identify (p, circles);
        node n1 = {start, 0}, n2 = {p, id};

        vgraph.push_back({n1, n2});
        edges[id].push_back(vgraph.size() - 1);

        q1.push ({0, distance (start,p), n2, int (vgraph.size() - 1)});
    }

    int cycle = 0;
    vector<Point> explore;
    vector<int> visit(vgraph.size());

    while (!q1.empty()) {
        auto [heur,dist,curr,edge] = q1.top();
        auto &[p1,id] = curr;
        auto &[ctr, rad] = circles[id];

        q1.pop();
        visit[edge] = true;

        cycle++;
        if (p1 == exit) {
            cout << " cycles : " << format (cycle) << " dist : " << dist << "\n" << flush ;
            // showgraph (start,exit,circles,explore);
            return dist;
        }

        minv[0] = 2 * pi, minv[1] = 2 * pi;                      // minv = max radian angle : 2 * pi
        for (auto &p2 : inter[id]) {                             // search for the nearest intersection, if any...
            const bool side = direction (p1, ctr, p2);

            angle[0] = 2 * asin (distance (p1,p2) * 0.5 / rad); // minor theta angle
            angle[1] = 2 * pi - angle[0];                       // major theta angle

            for (int j = 0; j < 2; j++) {
                minv[side^j] = min (minv[side^j], angle[j]);
            }
        }

        const int arcsize = inter.size() == 0 ? 1 : 2; // if the circle doesn't intersect with another, no need to compute the major angle.

        for (int &nxedge : edges[id]) {
            if (visit[nxedge]) continue;
            auto &[n2,n3] = vgraph[nxedge];

            if (n2.id != id) swap (n2, n3);

            const bool side = direction (p1, ctr, n2.p);
            const double alt = dist + distance (n2.p, n3.p);

            angle[0] = 2 * asin (distance (p1, n2.p) * 0.5 / rad); // minor theta angle
            angle[1] = 2 * pi - angle[0];                        // major theta angle

            for (int j = 0; j < arcsize; j++) {
                if (angle[j] > 0 && angle[j] < minv[side^j]) {
                    const double nxdist = rad * angle[j] + alt;
                    const double nxheur = distance (n3.p, exit);

                    q1.push({nxheur + nxdist, nxdist, n3, nxedge});
                }
            }
        }
    }

    return -1.0;
}

int main () {

    Timer now;

    double actual;
    Point start, exit;
    vector<Circle> circles;

    start = {-3, 1};   exit = {4.25, 0};
    circles = { {0.0, 0.0, 2.5}, {1.5, 2.0, 0.5}, {3.5, 1.0, 1.0}, {3.5, -1.7, 1.2} };

    start = {0, 1};  exit = {0, -1};
    circles = { {0.0, 0.0, 0.8}, {-3.8, 0.0, 3.2}, {3.5, 0.0, 3.0}, {7.0, 0.0, 1.0} };
    // actual = shortest_path_length(start,exit,circles);
    // shortest_path_length(start,exit,circles);


    start = {-3.95789,-0.122472}, exit = {3.84467,-0.775224};

    circles = {{-1.02379,4.6093,1.00092},{-3.47377,3.20452,1.40375},{4.67533,-4.25685,1.37145},{-1.11278,2.19384,0.802384},{1.91997,-4.21079,1.57525},{-4.45762,0.698929,0.5903},{4.92942,-4.41925,1.5658},{-0.342824,1.46026,0.570986},{-1.07665,1.27248,0.59809},{-2.23128,-1.66074,0.743706},{1.79625,-1.05022,1.56243},{4.62867,4.00504,0.795384},{-0.80137,2.83617,0.306572},{3.53418,2.98339,0.816837},{-4.9855,-0.510689,0.762409},{-0.814622,-4.87587,1.24673},{1.47127,3.01229,1.205},{1.18216,3.19538,1.06521},{1.45746,-4.33745,0.906502},{1.56613,-0.690713,1.33242},{3.80775,3.06505,0.843671},{-1.92488,1.9737,0.682499},{4.09179,2.27906,1.61176},{4.75822,2.49778,0.928805},{2.56461,2.97993,1.62802},{2.0485,0.0068931,0.698405},{1.15112,-4.25254,1.01176},{-1.94135,2.1933,1.28088},{-3.60405,-4.34815,1.34236},{0.501224,-2.10602,1.13565},{-1.03805,2.74604,1.26},{2.99094,1.39535,0.740853},{-3.74286,2.46016,1.32268},{-0.689434,0.767317,1.37305},{-3.14059,-3.00105,1.50568},{3.26481,-2.72518,0.767433},{-2.65795,2.4178,1.06287},{-1.24139,3.7552,0.411542},{-2.36956,3.84523,1.20138},{3.46635,0.533081,0.85777}};

    // double actual = shortest_path_length2(start,exit,circles);

    start = {1,1}, exit = {9,9};
    circles = {{0,0,0.280193},{0,1,0.512277},{0,2,0.25257},{0,3,0.488738},{0,4,0.16267},{0,5,0.290831},{0,6,0.517327},{0,7,0.395896},{0,8,0.474303},{0,9,0.297503},{0,10,0.339636},{1,0,0.724588},{1,2,0.470597},{1,3,0.619306},{1,4,0.6131},{1,5,0.528716},{1,6,0.203745},{1,7,0.234733},{1,8,0.525894},{1,9,0.30518},{1,10,0.213649},{2,0,0.533744},{2,1,0.43489},{2,2,0.485646},{2,3,0.242196},{2,4,0.316456},{2,5,0.345849},{2,6,0.121074},{2,7,0.523895},{2,8,0.336176},{2,9,0.627789},{2,10,0.39965},{3,0,0.356001},{3,1,0.50239},{3,2,0.639656},{3,3,0.388745},{3,4,0.501267},{3,5,0.309336},{3,6,0.397426},{3,7,0.441516},{3,8,0.526431},{3,9,0.605801},{3,10,0.359763},{4,0,0.358786},{4,1,0.62957},{4,2,0.471734},{4,3,0.260354},{4,4,0.246818},{4,5,0.259451},{4,6,0.517338},{4,7,0.46505},{4,8,0.461615},{4,9,0.379997},{4,10,0.370237},{5,0,0.481941},{5,1,0.780366},{5,2,0.18299},{5,3,0.466359},{5,4,0.502474},{5,5,0.29067},{5,6,0.50215},{5,7,0.710149},{5,8,0.601764},{5,9,0.538156},{5,10,0.531701},{6,0,0.419755},{6,1,0.476202},{6,2,0.523122},{6,3,0.402551},{6,4,0.522785},{6,5,0.346494},{6,6,0.18683},{6,7,0.419227},{6,8,0.418286},{6,9,0.804258},{6,10,0.678069},{7,0,0.510659},{7,1,0.370553},{7,2,0.430294},{7,3,0.340622},{7,4,0.597972},{7,5,0.650402},{7,6,0.662663},{7,7,0.512247},{7,8,0.632385},{7,9,0.489529},{7,10,0.2427},{8,0,0.275816},{8,1,0.40636},{8,2,0.663576},{8,3,0.717072},{8,4,0.253225},{8,5,0.59111},{8,6,0.469538},{8,7,0.524023},{8,8,0.282698},{8,9,0.303605},{8,10,0.413128},{9,0,0.341919},{9,1,0.570117},{9,2,0.456196},{9,3,0.471729},{9,4,0.528228},{9,5,0.455006},{9,6,0.691975},{9,7,0.448557},{9,8,0.427052},{9,10,0.481778},{10,0,0.359008},{10,1,0.683657},{10,2,0.473335},{10,3,0.416889},{10,4,0.326557},{10,5,0.337367},{10,6,0.371881},{10,7,0.702511},{10,8,0.496423},{10,9,0.511858},{10,10,0.579302}};


    //Test();
    // random1();
    // shortest_path_length(start,exit,circles);
    // random1();
    // shortest_path_length(start,exit,circles);


    // Draw::render();



    // display(start,exit,circles);
    now.stop();
    now.get_duration();
}
