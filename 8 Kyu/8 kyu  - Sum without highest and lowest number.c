int sum(int* source, int count)
{
	int* i, temp = 0, sum;
	int* const end = source + count;
	int* min = source, *max = source;

	for (i = source; i < end; ++i) {
		min = (*i < *min) ? i : min ;
		max = (*i > *max) ? i : max ;
	}

	if (count <= 1) {
		return 0;
	} else {
		for (; source < end; ++source)
			temp += *source;
	}
		sum = temp - *min - *max;
return sum;
}
