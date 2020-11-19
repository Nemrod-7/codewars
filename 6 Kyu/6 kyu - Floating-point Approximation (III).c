double quadratic(double a, double b, double c) {

    double x1 , x2, delta;

    delta = b * b - 4 * a * c;

    x1 = -(b + sqrt (delta)) / (2 * a);
    x2 = c / (a * x1);

    return x2;
}
