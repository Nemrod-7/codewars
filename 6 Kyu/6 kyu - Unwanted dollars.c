#define IS_DIGIT(x) (((x) >= '0') && ((x)<= '9'))
#define IS_GARB(x) ((x) == ' ' ||(x) == '\n' ||(x) == '\t' ||(x) == '$')

double money_value(char s[]) {

  int i, sign;
  double val = 0, power = 0;

  for (i = 0; IS_GARB(s[i]); ++i)
    ;
  sign = ((s[i] == '-') ? -1 : 1);
  if (s[i] == '-'|| s[i] == '+')
    ++i;
  for (; IS_GARB(s[i]); ++i)
    ;
  for (val = 0.0; IS_DIGIT(s[i]); ++i)
    val = val * 10.0 + (s[i] - '0');
  if (s[i] == '.')
    ++i;
  for (power = 1.0; IS_DIGIT(s[i]); ++i) {
    val = val * 10.0 + (s[i] - '0');
    power *= 10.0;
  }

return sign * val / power;
}
