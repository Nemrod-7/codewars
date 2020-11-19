double weight (int row, int pos) {

  int shft = 1, i = row;
  double res;

  while (i--> 0)
    shft *= 2;

    if(pos == 0 || pos == row) {
      res = (double)(shft - 1) / shft;
      return res;
    }

    res = row * (row + 1) / 2 * shft - (shft - 1) * 2;
    res /= (row - 1);

    return res / shft;
  }
