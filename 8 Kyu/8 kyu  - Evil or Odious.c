const char *evil(int value) {
	int countbit(unsigned n);

	return countbit(value) % 2 ? "It's Odious!" : "It's Evil!" ;

}

int countbit(unsigned n) {

	unsigned count = 0;

	while (n!=0) {
		n = n&(n-1);
		count++;
	}
return count;
}
