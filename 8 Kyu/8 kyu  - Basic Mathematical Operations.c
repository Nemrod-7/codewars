int basic_op(char op, int value1, int value2) {

	int res;

	switch (op) {
		case '+':
			res = value1 + value2;
			break;
		case '-':
			res = value1 - value2;
			break;
		case '/':
			res = value1 / value2;
			break;
		case '*':
			res = value1 * value2;
			break;
		default :
			printf ("unknown");
			break;
		}
  return res;
}
