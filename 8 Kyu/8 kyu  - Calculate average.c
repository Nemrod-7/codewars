char* position(char alphabet) {
  int c = alphabet - 96;
  char *r = malloc(32);

  sprintf(r, "Position of alphabet: %i", c);

  return r;
}
