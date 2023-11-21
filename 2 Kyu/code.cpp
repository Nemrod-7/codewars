#include <vector>
#include <map>
#include <list>
#include <cmath>
#include <algorithm>

struct Point {
    double x,y;

    Point() : x(0.0), y(0.0) {}
    Point(double x, double y) : x(x), y(y) {}
};

struct {
    bool operator () (const Point &a, const Point &b) const {
        return a.x != b.x ? a.x < b.x : a.y < b.y;
    }
} verticmp;

bool operator < (const Point &a, const Point &b) { return std::make_pair (a.x,a.y) < std::make_pair (b.x,b.y); }
bool operator == (const Point &a, const Point &b) { return a.x == b.x && a.y == b.y; }

double rnd (const double x) { return round (x * 1e8) / 1e8; }
double ccw (const Point &a, const Point &b, const Point &c) { return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x); }
double distance (const Point &a, const Point &b) { return std::hypot (a.x - b.x, a.y - b.y); }

double area (const std::vector<Point> &geom) { // area of a convex polygon :: all the point must be ordered clockwise
    double area = 0;

    for (size_t i = 0; i < geom.size(); i++) {
        size_t j = (i + 1) % geom.size();
        area += ((geom[i].x * geom[j].y) - (geom[j].x * geom[i].y));
    }

    return fabs (area) * 0.5;
}
Point circumcenter (const Point &a, const Point &b, const Point &c) { // get ttriangle circumcenter
    const double acx = a.x - c.x, bcy = b.y - c.y, bcx = b.x - c.x, acy = a.y - c.y;
    const double D = acx * bcy - bcx * acy;
    Point p;

    p.x = ((acx * (a.x + c.x) + acy * (a.y + c.y)) / 2 * bcy - (bcx * (b.x + c.x) + bcy * (b.y + c.y)) / 2 * acy) / D;
    p.y = ((bcx * (b.x + c.x) + bcy * (b.y + c.y)) / 2 * acx - (acx * (a.x + c.x) + acy * (a.y + c.y)) / 2 * bcx) / D;

    return p;
}
std::vector<Point> convex_hull (std::vector<Point> curr) { // monotone chain convex hull

    const int size = curr.size();
    int i, t, k = 0;
    std::vector<Point> hull (size * 2);
    std::sort (begin(curr), end(curr), verticmp);

    for (i = 0; i < size; i++) {
        while (k >= 2 && ccw (hull[k - 2], hull[k - 1], curr[i]) <= 0)
            k--;

        hull[k++] = curr[i];
    }

    for (i = size - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && ccw (hull[k - 2], hull[k - 1], curr[i]) <= 0)
            k--;

        hull[k++] = curr[i];
    }

    hull.resize (k - 1);
    return hull;
}

std::vector<std::vector<Point>> searchedges (const std::vector<std::vector<Point>> &edges) {
    std::vector<std::vector<Point>> uniq;
    std::map<std::vector<Point>, int> hist;

    for (size_t i = 0; i < edges.size(); i++) {
        std::vector<Point> t1 = edges[i];

        for (int k = 0; k < 3; k++) {
            std::vector<Point> edge {t1[k], t1[((k+1)%3)]};
            if (edge[0].x > edge[1].x) std::swap(edge[0], edge[1]);

            hist[edge]++;
        }
    }

    for (auto &[edge, freq] : hist) {
        if (freq == 1) {
            uniq.push_back (edge);
        }
    }

    return uniq;
}
void clean (std::list<std::vector<Point>> &network, const std::vector<Point> &super) {
    for (auto it = network.begin(); it != network.end();) {
        std::vector<Point> tri = *it;
        bool found = false;

        for (int i = 0; i < 3; i++) {
            if (find(super.begin(), super.end(), tri[i]) != super.end()) {
                found = true;
            }
        }

        if (found == true) {
            it = network.erase(it);
        } else {
            it++;
        }
    }
}

std::list<std::vector<Point>> triangulation (std::vector<Point> seed) { // bowyer-watson algorithm => returns a delauney trianglation

    double ymax = seed[0].y, ymin = seed[0].y;
    double xmax = seed[0].x, xmin = seed[0].x;

    for (size_t i = 1; i < seed.size(); i++) { //
        ymin = std::min(seed[i].y, ymin), ymax = std::max(seed[i].y, ymax);
        xmin = std::min(seed[i].x, xmin), xmax = std::max(seed[i].x, xmax);
    }

    const double dx = (xmax - xmin) * 2.0, dy = (ymax - ymin) * 2.0;
    const std::vector<Point> super = {{xmin - dx, ymin - dy * 3}, {xmin - dx, ymax + dy}, {xmax + dx * 3, ymax + dy}};
    std::list<std::vector<Point>> network = {super};

    for (auto &p : seed) { // add all the points one at a time to the network
        std::vector<std::vector<Point>> triangle;

        for (auto it = network.begin(); it != network.end();) {
            std::vector<Point> tri = *it;
            Point ctr = circumcenter (tri[0], tri[1], tri[2]);
            double rad = distance (ctr, tri[0]);

            if (distance (p, ctr) < rad) {
                it = network.erase(it);
                triangle.push_back(tri);
            } else {
                it++;
            }
        }

        std::vector<std::vector<Point>> polygon = searchedges (triangle);

        for (auto &edge : polygon) { // re-triangulate the polygonal hole
            network.push_back({edge[0], edge[1], p});
        }
    }

    clean(network, super);
    return network;
}
std::map<Point,std::vector<Point>> tovoronoi (const std::list<std::vector<Point>> &delauney) {
    std::map<Point,std::vector<Point>> voronoi;

    for (auto &tri : delauney) {
        Point ctr = circumcenter (tri[0], tri[1], tri[2]);

        voronoi[tri[0]].push_back(ctr);
        voronoi[tri[1]].push_back(ctr);
        voronoi[tri[2]].push_back(ctr);
    }

    return voronoi;
}

bool collinear  (const std::vector<Point> &hull, const Point &p) {
    for (size_t i = 0; i < hull.size(); i++) {
        int j = (i + 1) % hull.size();

        if (rnd (ccw(hull[i], hull[j], p)) == 0) {
            return false;
        }
    }

    return true;
}
std::vector<double> voronoi_areas (const std::vector<Point> &seed) {
    if (seed.size() < 5)
        return std::vector<double> (seed.size(), -1);

    std::vector<double> areas;
    std::vector<Point> hull = convex_hull (seed);
    std::list<std::vector<Point>> network = triangulation(seed);
    std::map<Point,std::vector<Point>> voronoi = tovoronoi(network);

    for (size_t i = 0; i < seed.size(); i++) {
        Point site = seed[i];

        if (collinear (hull, site) == false) {
            areas.push_back(-1.0);
        } else {
            std::vector<Point> cell = convex_hull (voronoi[site]);
            areas.push_back(area(cell));
        }
    }

    return areas;
}

int main () {

}
