int* nbMonths(int startPriceOld, int startPriceNew,
              int savingperMonth, double percentLossByMonth) {

    double old = (double)startPriceOld, new = (double)startPriceNew, savings = 0;
    double rate = (double)percentLossByMonth/100;
    double inc = (double)0.5/100;
    int i;
    int *r = malloc (2 * sizeof(int));

    i = 0;
    while ((old + savings) < new) {
      if (i % 2 == 1) {
        rate += inc;
      }
        savings += savingperMonth;
        old -= old * rate;
        new -= new * rate;
        ++i;
    }
    *r = i, *(r + 1) = nearbyint((old + savings) - new);
  return r;
  }
