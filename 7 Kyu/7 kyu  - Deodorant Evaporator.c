int evaporator(double content, double evap_per_day, double threshold) {

  int cnt = 0;
  double end = content * (threshold  /100);

   do {
      content -= content * (evap_per_day / 100);
      cnt++;
    }
    while (content > end);
return cnt;
}
