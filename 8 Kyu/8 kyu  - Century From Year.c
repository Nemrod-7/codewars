int centuryFromYear(int year) {
  int cnt = 1;
  while (year > 100) year -= 100, cnt++;
  return cnt;
}
