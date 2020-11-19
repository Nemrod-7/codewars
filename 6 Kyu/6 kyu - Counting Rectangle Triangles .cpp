#include <cmath>
#include <algorithm>

#define rnd(x) round(x * 1e5) / 1e5
bool vertcmp (const Point &a, const Point &b) { return a.x != b.x ? a.x < b.x : a.y < b.y; }
bool idem (const Point &a, const Point &b) { return a.x == b.x && a.y == b.y; }
double dist (const Point p, const Point q) { return std::hypot (p.x - q.x, p.y - q.y);}
bool is_right (Point p1, Point p2, Point p3) {
    double a = dist (p1, p2), b = dist (p2, p3), c = dist (p1, p3);
    double a2 = rnd (a * a), b2 = rnd (b * b), c2 = rnd (c * c);
    return (a2 + b2 == c2 || b2 + c2 == a2 || a2 + c2 == b2);
}
unsigned countRectTriang (const std::vector<Point> &p) {

    unsigned count = 0;
    std::vector<Point> map = p;
    std::sort (map.begin(), map.end(), vertcmp);
    std::vector<Point>::iterator it = std::unique (map.begin(), map.end(), idem);
    map.erase (it, map.end());

    for (unsigned i = 0; i < map.size(); i++)
        for (unsigned j = i  + 1; j < map.size() ; j++)
            for (unsigned k = j + 1; k < map.size(); k++)
                if (is_right (map[i], map[j], map[k]))
                    count++;

    return count;
}
