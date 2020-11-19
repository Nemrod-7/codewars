typedef double (*generic_func_t) (double);


double* interp(generic_func_t f, double l, double u, int n) {

    const double d = ((u - l) / (double)n);
    const int end = n;
    double *arr = malloc (end * sizeof (double));

    for (int i = 0; i < end; i++) {
        arr[i] =  floor (f(l) * 100) / 100;
        l += d;
    }

    return arr;
}
