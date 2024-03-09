#include <stdio.h>
#include <math.h>

struct RK4_result {
    unsigned len;
    double *y;
};

struct RK4_result RK4(double x0, double y0, double h, double (*f)(double x,double y), double x1) {
    struct RK4_result r;
    // compute r.len
    // allocate r.y, tests will free it
    r.y = (double*)malloc(r.len*sizeof(double));

    while (x0 <= x1) {

        x += h;
    }

    // compute r.y
    return r;
}

int main () {

// double k1 ​= hf(x,y);
// double k2 ​= hf(x + 2 * h​, y + 2 * k1​​);
// double k3 ​= hf(x + 2 * h​, y + 2 * k2​​);
// double k4 ​= hf(x + h, y + k3​);

// x0 + k * h <= x1
}
