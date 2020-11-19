unsigned bin_to_decimal(const char *src) {

	int dec = 0;

	while (*src) {
		dec = dec * 2 + (*src - '0');
		src++;
	}

  return dec;
}
