
////////////////////////////Arkive////////////////////////////////
double dotprod (Point a, Point b) { return a.x * b.x + a.y * b.y; }
double angle (const Point &p1, const Point &p2, const Circle &c1) { // central angle of a circle
  return 2.0 * asin (distance (p1,p2) * 0.5 / c1.r);
}

double area (const vector<Point> &curr) { // polygon area
    double sum = 0;

    for (size_t i = 0; i < curr.size(); ++i) {
        sum += (curr[i].x * curr[i].y + 1) - (curr[i].x + 1 * curr[i].y);
    }

    return abs (sum) / 2;
}
double ccw (const Point &o, const Point &a, const Point &b) { // counter clockwise
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}
vector<Point> convex_hull (vector<Point> curr) {

    const int size = curr.size();
    int i, t, k = 0;
    vector<Point> hull (size * 2);
    //sort (begin(curr), end(curr), vertcmp);

    for (i = 0; i < size; i++) {
        while (k >= 2 && ccw (hull[k - 2], hull[k - 1], curr[i]) <= 0) {
            k--;
        }

        hull[k++] = curr[i];
    }

    for (i = size - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && ccw (hull[k - 2], hull[k - 1], curr[i]) <= 0) {
            k--;
        }
        hull[k++] = curr[i];
    }

  //return vector<point>(h.begin(), h.begin() + k - (k > 1));
    hull.resize (k - 1);
    return hull;
}
vector<Point> neighrestpoint (vector<Circle> space, vector<int> circle, vector<vector<Point>> edge, Point p1) {
    int id = identify (p1, space);
    vector<Point> nxp;
    double minv = numeric_limits<double>::infinity();

    for (int i = 0; i < circle.size(); i++) {
        vector<Point> nxe = edge[circle[i]];
        int ida = identify (nxe[0], space);

        if (ida != id) swap (nxe[0], nxe[1]);

        Point tmp = nxe[0];
        double dist = distance (p1,tmp);
        //cout << ida << " " << idb << "\n";
        if (dist < minv) {
            minv = dist;
            nxp = nxe;
        }
    }
    return nxp;
}

void mkcircle (Point p1, Point p2) {

  double m = slope (p1,p2);
  const double rad = 0.5;
  double cosine = 1.0 / sqrt (1.0 + sq (m)), sine = m / sqrt (1.0 + sq (m));

  for (double deg = 0; deg < 90; deg++) {
      double alpha = radian(deg);
  //    Point p2 = {p1.x + rad * cos (alpha), p1.y + rad * sin (alpha)};
  //    node.push_back(p2);
  }

}
pair<double,double> barycentre (const Point &p1, const Point &p2, const Point &p3) {
  double x = (p1.x + p2.x + p3.x) / 3;
  double y = (p1.y + p2.y + p3.y) / 3;
  return {x, y};
}
double area (const Point &a, const Point &b, const Point &c) { // triangle area
    double x1 = a.x, x2 = b.x, x3 = c.x;
    double y1 = a.y, y2 = b.y, y3 = c.y;
    return rnd (abs (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) * 0.5);
    //area2 = abs((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y))
}


void arclenth (Point a, Point b, Point c, double r) { // arc length of circle segment a-b
    double ab = distance (a,b) / 2, hyp = distance (a,c); // ab -> opposite side, hyp = hypthenuse
    double theta = degree (asin (ab / hyp));
    double arclen = 2 * M_PI * r * (theta / 360);
}
void astar (Point start, Point exit, vector<Circle> graph) {
    struct vertex {
        double cost, dist, heur;
    		vector<Point> hist;
    };
    struct comp {
        bool operator()(const vertex &a, const vertex &b ) {
            return  a.cost == b.cost ? a.heur > b.heur : a.cost > b.cost;
            //return  a.heur == b.heur ? a.cost > b.cost : a.heur > b.heur;
        }
    };

    const double rad = 0.1;
    const vector<Point> direct { {rad,0.0},{0.0,rad},{-rad,0.0},{0.0,-rad},  {rad,rad},{-rad,rad},{rad,-rad},{-rad,-rad} };

		priority_queue<vertex,vector<vertex>,comp> q1;
		map<Point,bool> visit;
		//Graph system (start, exit, graph);
		//Draw::graph(start, exit, graph);
		vertex source = {0,0,distance (start,exit),{start}};
		q1.push (source);

		int cycles = 0;
		vector<Point> vect;

		while (!q1.empty()) {

				auto [cost, dist, left, path] = q1.top();
				q1.pop();

				Point curr = path.back();
				cycles++;
        //vect.push_back(curr);
				if (cycles > 1500 || distance (curr,exit) < 0.3) {
						vect = path;
						break;
				}

        double alt = (dist + rad) * 1.0;

				for (auto &dir : direct) {
						double nx = rnd (curr.x + dir.x), ny = rnd (curr.y + dir.y);
						Point nxp = {nx, ny};
            //cout << nx << " " << ny << "\n";
						//if (graph.isinside (nxp)) {
						double heu = distance (nxp, exit);
						double fnc = alt + heu;

						if (is_valid (nxp, graph) && !visit[nxp]) {
								path.push_back(nxp);
								vertex nxv {fnc, alt, heu, path};
								visit[nxp] = true;
								q1.push(nxv);
						}
				//}
				}
		}

    cout << '{';
    for (auto p : vect) {
        cout << "{"<< p.x << "," << p.y <<  "},";
    }
    cout << "};";
 		//Draw::dots(vect);
		//Draw::img();
}
void astar2 (Point start, Point exit, vector<Circle> space) {

  struct vertex { double dist; vector<Point> path; };
  struct comp {
      bool operator() (const vertex &a, const vertex &b ) {
          return  a.dist > b.dist;
      }
  };

  const int size = space.size();
  vector<vector<int>> graph (size);
  vector<vector<Point>> edge;
  vector<Point> vect;
  priority_queue<vertex,vector<vertex>,comp> q1;
  map<Point,bool> visit;

  edge = tangraph (space);
  // cout << fixed << setprecision (5);
  for (int i = 0; i < edge.size(); i++) { // attach edges to circles
      for (int j = 0; j < edge[i].size(); j++) {
          int id = identify (edge[i][j], space);
          graph[id].push_back(i);
      }
  }

  for (auto p : find_tangent (exit,space)) { // connect exit tangents to tan visibility space
      int id = identify (p, space);
      edge.push_back({p, exit}); // -> exit return -1 !!
      graph[id].push_back(edge.size());
  }

  for (auto p : find_tangent (start,space)) {
      vertex source = {distance (start,p),{start,p}};
      q1.push(source);
  }

  int cycle = 0;

  while (!q1.empty()) {
      auto [heur,path] = q1.top();
      Point p1 = path.back();
      q1.pop();

      cycle++;

      if (cycle == 2) {
        // Draw::line(path);
          // Display::vect(path);
          cout << "\n";
          break;
      }
      // identify circle
      int id = identify (p1, space);
      auto [p3,rad] = space[id];
      const double hyp = distance (p1,p3);

      for (int i = 0; i < graph[id].size(); i++) {
          vector<Point> nxe = edge[graph[id][i]];
          int ida = identify (nxe[0], space);
          if (ida != id) swap (nxe[0],nxe[1]);
          const double ab = distance (p1,nxe[0]);
          //            arc length                      + distance to next circle
          double alt = 2 * rad * asin (0.5 * ab / hyp) + distance (nxe[0], nxe[1]);

          vector<Point> route = path;

          for (auto e : nxe) {
              route.push_back(e);
          }

          vertex nextv = {heur + alt, route};
          q1.push(nextv);

          vect.push_back(nxe[1]);
      }

      /*

      */
  }
  Draw::dots(vect);
    /*
    for (int i = 0; i < graph.size(); i++) {
        cout << "circle " << i << " :: ";
        for (int j = 0; j < graph[i].size(); j++) {
            cout << graph[i][j] << " ";
        }
        cout << "\n";
    }
  */
}
