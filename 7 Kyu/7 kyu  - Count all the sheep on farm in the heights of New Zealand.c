int sum (int *src);

int lostSheep(const int *friday, const int* saturday, int total) {

return total - (sum (friday) + sum (saturday));
}

int sum (int *src) {

  int i, sum = 0;
  for (i = 0; src[i] != '\0'; ++i)
    sum += src[i];

return sum;
}
