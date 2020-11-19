void fakeBin(const char *digits, char *buffer) {

  while (*digits) {
   *buffer = ((*digits < '5') ?  '0' : '1');
    ++digits, ++buffer;
  };
  *buffer = '\0';

}
