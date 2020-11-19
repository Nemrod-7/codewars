int min(int* array, int arrayLength) {

	const int *end = array + arrayLength;
	int *i = array, *minv = array, *maxv = array;

	for (; i < end; ++i) {
		minv = (*i < *minv) ? i : minv;
		maxv = (*i > *maxv) ? i : maxv;
	}
return *minv;
}

int max(int* array, int arrayLength) {

	const int *end = array + arrayLength;
	int *i = array, *minv = array, *maxv = array;

	for (; i < end; ++i) {
		minv = (*i < *minv) ? i : minv;
		maxv = (*i > *maxv) ? i : maxv;
	}
return *maxv;
}
