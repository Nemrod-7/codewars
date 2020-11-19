int bouncingBall(double h, double bounce, double window) {
      if (h <= 0 || h <= window || bounce >= 1 || bounce <= 0) return -1;

  int cnt = -1;

  while (h > window) {
    cnt += 2;
    h = h * bounce;
  }

  return cnt;
}
