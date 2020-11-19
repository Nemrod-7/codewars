#define EPURED_REGION(x) (x - 5) % 10 != 0 && x/10 != 5

int dontGiveMeFive(int start, int end) {
	unsigned int cnt = 0;

  do {
    	if (EPURED_REGION(end)) cnt++;
  }
  while (end-->start);
return cnt;
}
