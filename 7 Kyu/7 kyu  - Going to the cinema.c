int movie(int card, int ticket, double perc) {

  float sysA = 0, sysB = 0;
  int cnt = 0;
  double price = (double)ticket, totalB = 0;

  if (card == 0) return 2;
  do {
    cnt++;
    price *= perc;
    totalB += price;
    sysB = card + totalB;
    sysA += ticket;

  }
  while ((sysA - sysB) < 1);

return cnt;
}
