////////////////////////////////////////////////////////
// shape.h
typedef struct _shape_vtable_t {
  double (*get_area)();
  double (*get_perimenter)();
  void (*destroy)();
} shape_vtable_t;

typedef struct _shape_t {
  shape_vtable_t *vtable;
} shape_t;

unsigned int shape_get_area(const void *shape);
unsigned int shape_get_perimeter(const void *shape);
void shape_destroy(const void *shape);

////////////////////////////////////////////////////////
// right_triangle.h

typedef struct _right_triangle_t {
  shape_t shape;
  double side1;
  double side2;
} right_triangle_t;

right_triangle_t *right_triangle_create(double side1, double side2);

////////////////////////////////////////////////////////
// right_triangle.c

static double get_area_tri (const right_triangle_t *shape) {
    return shape->side1 * shape->side2 * 0.5;
}
static double get_perimeter_tri (const right_triangle_t *src) {
    double square =  src->side1 * src->side1 + src->side2 * src->side2;
    return src->side1 + src->side2 + sqrt (square);
}
static void destroy_tri (const right_triangle_t *triangle) {
    free (triangle);
}
static shape_vtable_t triangle_vshape = {
  get_area_tri,
  get_perimeter_tri,
  destroy_tri
};
right_triangle_t *right_triangle_create (double side1, double side2) {

    right_triangle_t *triangle = malloc (sizeof (right_triangle_t));
    triangle->shape.vtable = &triangle_vshape;
    triangle->side1 = side1;
    triangle->side2 = side2;

    return triangle;
}
