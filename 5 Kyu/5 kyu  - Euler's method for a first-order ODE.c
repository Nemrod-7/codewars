double exEuler(int n) {

    double h = 1/(double)n, x = 0, y = 1, z = 1;
    double med = 0;

    for (int i = 0; i < n; ++i) {
        y = y + (2 - exp (-4 * x) - 2 * y) * h;
        x += h;
        z = 1 + 0.5 * exp (-4 * x) - 0.5 * exp (-2 * x);

        med += fabs(y - z)/z;
    }
    return floor (med / (n + 1) * 1e6)/1e6;
}
