int64_t elderAge (int64_t m, int64_t n, int64_t l, int64_t t) {
  if (m == 0 || n == 0) return 0;
  if (m < n) return elderAge (n, m, l, t);
  if (m < 4 || n < 4) return simple (n, m, l, t);

  int64_t p1 = mxpower (m), p2 = mxpower (n);

  if (p1 > p2) {
      int64_t r1 = special (p1, n, l, t);
      int64_t r2 = sub (p1, m - p1, n, l, t);

      return add(t, r1, r2);
  }
  // p1 == p2
  int64_t r1 = special (p1, p2, l, t);
  int64_t r2 = sub (p2, p1, n - p2, l, t);
  int64_t r3 = sub (p1, m - p1, p2, l, t);
  int64_t r4 = sub (p1 ^ p2, m - p1, n - p2, l, t);

  return add (t, r1, r2, r3, r4)
}
int64_t sub (int64_t base, int64_t m, int64_t n, int64_t l, int64_t t) {

  if (base >= l) {
      base -= l;
      l = 0;
  } else {
      l -= base;
      base = 0;
  }
  return add (t, mul(t, m, n, base), elderAge(m, n, l, t));
}
int64_t simple (int64_t m, int64_t n, int64_t l, int64_t t) {
  int64_t res = 0;

  for (int64_t r = 0; r < m; r++) {
      for (int64_t c = 0; c < n; c++) {
          int64_t val = r ^ c;

          res += val >= l ? val - l : 0;
          if (res >= t) res %= t;
      }
  }

  return res
}
int64_t mxpower (int64_t n) {
  int64_t k = 4;
  while (k * 2 <= n) k *= 2;
  return k;
}


int64_t special (int64_t m, int64_t n, int64_t l, int64_t t) { // requires m >= n, and m (>=4) is pow of 2
  // 0 ~ m-1, x n
  // 0 ~ m - 1 - l, x n
  if (m - 1 <= l) return 0

  // (m - 1 - l) * (m - l) * n / 2
  if ((m - l) & 1) {
      return mul (t, (m - 1 - l) / 2, m - l, n);
  } else {
      return mul (t, m - 1 - l, (m - l) / 2, n);
  }
}
int64_t mul (t, ...args) {
    return args.reduce((s, x) => safeMul (s, x, t), 1)
}
int64_t add (t, ...args) {
    return args.reduce ((s, x) => (s + x) % t, 0)
}

// https://github.com/chinesedfan/adventofcode/blob/master/lib/2019/day22.js
vector<int64_t> ones (int64_t n) {
    vector<int64_t> ret;

    while (n) {
        ret.unshift(n & 1);
        n -= (n & 1);
        n /= 2;
    }
    return ret;
}
int64_t safeMul (int64_t a, int64_t b, int64_t n) {
    // a * b mod n
    vector<int64_t> os = ones (b < 0 ? b + n : b);

    int64_t r = 0;
    for (let i = 0; i < os.length; i++) {

        if (os[i] & 1) {
            r = (r + r + a) % n;
        } else {
            r = (r + r) % n;
        }
    }
    return r;
}
