int enough(int cap, int on, int wait) {
  int fit = cap - on - wait;
  return (fit < 0) ? -fit : 0;
}
