typedef struct Vector {
    double x;
    double y;
    double z;
} vect;

vect crossProduct(vect a, vect b) {
    vect c;
    c.x = a.y * b.z - a.z * b.y;
    c.y = a.z * b.x - a.x * b.z;
    c.z = a.x * b.y - a.y * b.x;
    return c;
}
