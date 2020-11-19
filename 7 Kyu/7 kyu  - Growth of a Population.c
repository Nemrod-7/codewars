int nbYear(int p0, double percent, int aug, int p) {

  int growth = p0 + p0 * (float) percent / 100 + aug;
  int cnt = 0;

  do {
    growth = growth + growth * (float) percent / 100 + aug;
    cnt++;
    printf ("%i %i\n",cnt,growth);
  }
  while (growth < p);

return cnt + 1;
}
