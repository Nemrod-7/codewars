int gps(int s, double* x, int sz) {

double max = 0.0, temp = 0.0;

  do {
    temp = x [sz] - x[sz - 1];
    if (temp > max) max = temp;
  }
  while (sz-->0);

 return (s > 0) ? (max * 3600) /  s : 0;
}
