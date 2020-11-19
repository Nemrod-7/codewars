#define IS_OR(x) ((x) % 3 == 0 || (x) % 5== 0)

int solution (int number) {

  int sum = 0;
  number = number - 1;

 do {
    if (IS_OR(number))
      sum += number;
  }
  while (number-->0);

return sum;
}
