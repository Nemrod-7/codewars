#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define MAX(x,y) ((x) > (y)) ? (x) : (y)
#define MIN(x,y) ((x) < (y)) ? (x) : (y)

const double eps = 1e-8, infinity = 9999.0;

typedef struct _point { double x, y; } Point;
typedef struct _geom {
    size_t size;
    Point *data;
} Geom;
typedef struct _node {
    Geom g;
    struct _node *next;
} Node;
typedef struct {
    Node *root;
} link;
int cmp (const void *a, const void *b) {
    double arg1 = *(const double*)a, arg2 = *(const double*)b;
    return arg2 - arg1;
}
int verticmp (const void *a, const void *b) {
    const Point *ia = a, *ib = b;
    return ia->x != ib->x ? ia->x < ib->x : ia->y < ib->y;
}

////////////////////////////////////trigo///////////////////////////////////////
double rnd (const double x) { return round (x * 1e8) / 1e8 ; }
bool equal (const Point a, const Point b) { return a.x == b.x && a.y == b.y; }
double distance (const Point a, const Point b) { return hypot (a.x - b.x, a.y - b.y); }
double ccw (const Point *o, const Point *a, const Point *b) { return (a->x - o->x) * (b->y - o->y) - (a->y - o->y) * (b->x - o->x); }
Point circumcenter (const Geom *tri) { // get triangle circumcenter
    const Point a = tri->data[0], b = tri->data[1], c = tri->data[2];
    const double acx = a.x - c.x, bcy = b.y - c.y, bcx = b.x - c.x, acy = a.y - c.y;
    const double D = acx * bcy - bcx * acy;
    Point p;

    p.x = ((acx * (a.x + c.x) + acy * (a.y + c.y)) / 2 * bcy - (bcx * (b.x + c.x) + bcy * (b.y + c.y)) / 2 * acy) / D;
    p.y = ((bcx * (b.x + c.x) + bcy * (b.y + c.y)) / 2 * acx - (acx * (a.x + c.x) + acy * (a.y + c.y)) / 2 * bcx) / D;

    return p;
}
double area (const Geom *poly) { // area of a convex polygon :: all the point must be ordered clockwise
    double area = 0;

    for (size_t i = 0; i < poly->size; i++) {
        size_t j = (i + 1) % poly->size;
        area += ((poly->data[i].x * poly->data[j].y) - (poly->data[j].x * poly->data[i].y));
    }

    return fabs (area) * 0.5;
}

//////////////////////////////////display and unused///////////////////////////////////////
int horizcmp (const void *a, const void *b) {
  const Point *ia = a, *ib = b;
  return ia->y != ib->y ? ib->y < ia->y : ib->x < ia->x;
}
void showpoint (Point p) {
  printf ("[%5.2f, %5.2f]", p.x, p.y);
}
void showvect (unsigned n, double ar[]) {
    for (unsigned i = 0; i < n; i++) {
        printf("%f ", ar[i]);
    }
    printf ("\n");
}
void showgeom (Geom g) {
  for (size_t i = 0; i < g.size; i++){
      showpoint(g.data[i]);
  }
  printf("\n");
}
void showstack (link *net) {
    Node *curr = net->root;

    while (curr != NULL) {
        showgeom(curr->g);
        curr = curr->next;
    }
}
/////////////////////////////////polygon////////////////////////////////////////
Geom new_geom (size_t size) {
    Geom next = {size, (Point *) malloc (size * sizeof (Point))};
    return next;
}
Geom edge (Point a, Point b) {
    Geom next = new_geom(2);
    next.data[0] = a, next.data[1] = b;
    qsort (next.data, next.size, sizeof (Point), verticmp);
    return next;
}
Geom triangle (Point a, Point b, Point c) {
    Geom next = new_geom(3);
    next.data[0] = a, next.data[1] = b, next.data[2] = c;
    qsort (next.data, next.size, sizeof (Point), verticmp);
    return next;
}
void free_geom (Geom now) {
    free (now.data);
}
bool collinear (const Geom *graph, const Point *p) {

    for (size_t i = 0; i < graph->size; ++i) {
        size_t j = (i + 1) % graph->size;
        const Point a = graph->data[i], b = graph->data[j];

        if (rnd(ccw (&a, &b, p)) == 0.0) {
            return true;
        }
    }

    return false;
}
bool g_equal (const Geom *a, const Geom *b) {
    if (a->size != b->size) return false;

    for (size_t i = 0; i < a->size; i++) {
        if (a->data[i].x != b->data[i].x || a->data[i].y != b->data[i].y) {
            return false;
        }
    }

    return true;
}
////////////////////////////////linked-list/////////////////////////////////////
void push (link *stack, Geom data) {
    Node *cell = stack->root;
    stack->root = (Node *) malloc (sizeof (Node));
    stack->root->g = data;
    stack->root->next = cell;
}
link *new_stack () {
    link *next = (link *) malloc (sizeof(link));
    next->root = NULL;
    return next;
}
Geom convex_hull (Geom map) { // monotone chain convex hull algorithm

    qsort (map.data, map.size, sizeof (Point), verticmp);
    int size = map.size, i, t, k = 0;
    Geom hull = new_geom (map.size * 2);
    Point *H = hull.data, *p = map.data;

    for (i = 0; i < size; i++) {
        while (k >= 2 && ccw (&H[k - 2], &H[k - 1], &p[i]) <= 0) {
            k--;
        }

        H[k++] = p[i];
    }

    for (i = size - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && ccw (&H[k - 2], &H[k - 1], &p[i]) <= 0) {
            k--;
        }

        H[k++] = p[i];
    }

    hull.size = k - 1;
    return hull;
}

link *triangulation (const Point seed[], unsigned size) { // bowyer-watson algorithm => returns a delauney triangulation
  ////////////////////////plane limits//////////////////////////
  double Xmin = infinity, Xmax = -infinity, Ymin = infinity, Ymax = -infinity;
  link *network = new_stack();
  // make a super-triangle that contains all points
  for (unsigned i = 0; i < size; i++) {
      Xmin = MIN (Xmin, seed[i].x), Ymin = MIN (Ymin, seed[i].y);
      Xmax = MAX (Xmax, seed[i].x), Ymax = MAX (Ymax, seed[i].y);
  }
  const double dx = (Xmax - Xmin) * 10, dy = (Ymax - Ymin) * 10;
  Geom master = triangle ((Point){Xmin - dx, Ymin - dy * 3}, (Point) {Xmin - dx, Ymax + dy}, (Point) {Xmax + dx * 3, Ymax + dy});
  push (network, master);

  for (unsigned i = 0; i < size; i++) {
      Point site = seed[i];

      unsigned nbad = 0, nedge = 0;
      Geom bad[64], edges[64];

      Node *curr = network->root, **pp = &network->root /* pointer to a pointer */;

      while (curr) { // search for triangles to divide
          Geom tri = curr->g;
          const Point ctr = circumcenter (&tri);
          const double rad = distance (ctr, tri.data[0]);

          if (distance (site, ctr) < rad) {
              bad[nbad++] = tri;
              *pp = curr->next;
          } else {
              pp = &curr->next;
          }
          curr = curr->next;
      }

      for (unsigned j = 0; j < nbad; j++) { // get uniques edges
          for (int k = 0; k < 3; k++) {
              bool isfree = true;
              Geom e1 = edge (bad[j].data[k], bad[j].data[(k+1)%3]);

              for (unsigned l = 0; l < nbad; l++) {
                  if (l == j) continue;

                  for (unsigned m = 0; m < 3; m++) {
                      Geom e2 = edge (bad[l].data[m], bad[l].data[(m+1)%3]);

                      if (g_equal (&e1, &e2)) {
                          isfree = false;
                      }
                  }
              }

              if (isfree == true) {
                  edges[nedge++] = e1;
              }
          }
      }

      for (unsigned j = 0; j < nedge; j++) { // re-triangulate
          Geom edge = edges[j];
          Point a  = edge.data[0], b = edge.data[1];
          push(network, triangle (a,b,site));
      }
  }

  Node *curr = network->root, **pp = &network->root;

  while (curr) { // remove all points from the initial super-triangle
      Geom tri = curr->g;
      const Point *pt = tri.data;

      bool found = false;

      for (int i = 0; i < 3; i++) {
          Point e = master.data[i];

          if (equal (pt[0], e) || equal (pt[1], e) || equal (pt[2], e)) {
              found = true;
              break;
          }
      }

      if (found == true) {
          *pp = curr->next;
      } else {
          pp = &curr->next;
      }

      curr = curr->next;
  }

  return network;
}
void voronoi_areas (Point p[], unsigned size, double areas[]) {

    if (size < 4) {
        while (size-->0) areas[size] = -1;
        return;
    }
    Point seed[size];
    memcpy (seed, p, size * sizeof(Point));

    Geom hull = convex_hull ((Geom){.size = size, .data = seed});
    Geom cell[size];
    link *network = triangulation (seed, size);

    for (unsigned i = 0; i < size; i++) {
        cell[i] = new_geom(10);
        cell[i].size = 0;
    }

    Node *curr = network->root;
    while (curr) {
        Geom tri = curr->g;
        const Point ctr = circumcenter (&tri);

        for (unsigned i = 0; i < size; i++) {
            if (equal (tri.data[0], p[i])) cell[i].data[cell[i].size++] = ctr;
            if (equal (tri.data[1], p[i])) cell[i].data[cell[i].size++] = ctr;
            if (equal (tri.data[2], p[i])) cell[i].data[cell[i].size++] = ctr;
        }

        curr = curr->next;
    }

    for (unsigned i = 0; i < size; i++) {
        if (collinear (&hull, &p[i]) == true) {
            areas[i] = -1;
        } else {
            cell[i] = convex_hull (cell[i]);
            areas[i] = area (&cell[i]);
        }
    }

}

void test() {

     {
       unsigned n = 5;
       Point p[] = { {0.0, 0.0}, {2.0, 0.0}, {-2.0, 0.0}, {0.0, 2.0}, {0.0, -2.0} };
       double a[n], ans[] = {4.0, -1, -1, -1, -1};

       voronoi_areas(p, n, a);
       // showgeom((Geom) {n, p});

     }


    {
       unsigned n = 6;
       Point p[] = { {2,1}, {2,-1}, {4.4, 2.2}, {4.4, -2.2}, {-0.4, 2.2}, {-0.4, -2.2} };
       double a[n], ans[] = {8, 8, -1, -1, -1, -1};
       // voronoi_areas(p, n, a);
    }

}

int main () {

    int size = 15;
    Point seed[] = {{-1.049,4.626},{1.024,-3.809},{-4.049,-0.776},{-2.348,4.552},{2.094,-4.806},{-3.240,0.227},{3.575,-3.069},{-1.497,2.294},{-3.876,1.089},
        {1.491,-3.934},{4.817,3.037},{-3.551,-1.305},{1.023,3.013},{2.418,-4.567},{4.432,3.426}};
    double areas[size];
    // voronoi_areas (seed, size, areas);


    size_t ns = 7;
    Point p[] = {{4,1},{3,4},{3,7},{4,8},{7,9},{9,6},{7,1}};
    Geom cell = {ns, p};

    Geom graph = {size, seed};
    test();


    // test ();
    // showstack(network);

    printf("\nexit\n");
}
