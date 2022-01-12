#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

extern char *strdup (const char *);

char **tokenize (const char *src) {
    char *expr = strdup (src);
    const int size = strlen (expr) - 1;
    char *it = expr + 1, buff[32];
    char **token = malloc (3 * sizeof (char*));
    int i, j = 0;

    while (it - expr < size) {
      i = 0;

      if (*it == '(') {
        do { buff[i++] = *it; } while (*it++ != ')');
      } else {
        while (it - expr < size && *it != ' ') buff[i++] = *it++;
      }
      buff[i] = '\0';
      token[j++] = strdup (buff);
      it++;
    }

    return token;
  }
void removezero (char *src) {
  char *ptr = &src[strlen (src) - 1];

  while (*ptr == '0') *ptr-- = '\0';
  if (*ptr == '.') *ptr = '\0';
}
bool isnum (const char *expr) {

    for (int i = 0; expr[i] != '\0'; i++) {
        if (isalpha (expr[i])) return false;
        if (isspace (expr[i])) return false;
    }
    return true;
}

const char *calc (const char *arg1, char op, const char *arg2) {
    char *res = malloc (32 * sizeof (char)), *it;

    if (arg1[0] == '0' || arg2[0] == '0') {
        if (op == '*') return strdup ("0");
        if (op == '+') return arg1[0] == '0' ? arg2 : arg1;
    }

    if (strcmp (arg1, "1") == 0 || strcmp (arg2, "1") == 0) {
        if (op == '^') return arg1;
        if (op == '*') return strcmp (arg1, "1") == 0 ? arg2 : arg1;
        if (op == '/' && strcmp (arg2, "1") == 0) return arg1;
    }

    if (isnum (arg1) && isnum (arg2)) {
        double a = strtod (arg1, &it), b = strtod (arg2, &it);
        double val;

        switch (op) {
            case '+' : val = a + b; break;
            case '-' : val = a - b; break;
            case '*' : val = a * b; break;
            case '/' : val = a / b; break;
            case '^' : val = pow (a, b); break;
        }

        sprintf (res, "%f", val);
    } else {
        sprintf (res, "(%c %s %s)", op, arg1, arg2);
    }
    removezero(res);
    //printf ("%s", res);
    return res;
}
const char *diff (const char *input) {

    if (isnum(input)) return "0";
    if (input[0] == 'x') return "1";

    char **expr = tokenize (input);
    char *op = expr[0], arg1[32], buff[32];

    if (strcmp(op, "+") == 0) {   // add : a + b => a' + b'
        return calc (diff(expr[1]), '+', diff (expr[2]));
    }
    if (strcmp(op, "-") == 0) {   // add : a - b => a' - b'
        return calc (diff(expr[1]), '-', diff (expr[2]));
    }
    if (strcmp(op, "*") == 0) {   // mul : a * b => a.b' + a'.b
        const char *a = calc (expr[1], '*', diff (expr[2])), *b = calc (diff(expr[1]), '*', expr[2]);

        //printf ("%s %s", expr[1], diff (expr[2]));
        return calc (a, '+', b);
    }
    if (strcmp(op, "/") == 0) {   // div : a / b => (a'* b − b'* a) / (b * b)
        const char *a = calc (diff(expr[1]), '*', expr[2]), *b = calc (expr[1], '*', diff (expr[2]));
        const char *num = calc (a, '-', b), *den = calc (expr[2], '^', "2");
        return calc (num, '/', den);
    }
    if (strcmp(op, "^") == 0) {   // pow : x^a   => a.x^(a - 1)
        const char *ex = calc (expr[2], '-', "1");
        return calc (expr[2], '*', calc ("x", '^', ex));
    }
    if (strcmp(op, "ln") == 0) {  // ln  : ln(x) => 1 / x
        const char *ex = diff (expr[1]);
        return calc (ex, '/', expr[1]);
    }
    if (strcmp(op, "sin") == 0) { // sin : sin x => cos x
        const char *ex = diff (expr[1]);

        sprintf (buff, "(cos %s)", expr[1]);
        //printf ("%s\n", expr[1]);
        return calc (ex, '*', buff);
    }
    if (strcmp(op, "cos") == 0) { // cos : cos x => -sin x
        char ex[32];
        sprintf (ex, "-%s", diff (expr[1]));
        sprintf (buff, "(sin %s)", expr[1]);
        return calc (ex, '*', buff);
    }
    if (strcmp(op, "exp") == 0) { // exp : a^x   => a^x . ln (a)
        const char *ex = diff (expr[1]);
        sprintf (buff, "(exp %s)", expr[1]);

        return calc (ex, '*', buff);
    }
    if (strcmp(op, "tan") == 0) { // tan : tan x => 1 / (cos²(x))
        const char *ex = diff (expr[1]);
        sprintf (buff, "(cos %s)", expr[1]);
        sprintf (arg1,"%s", calc (buff, '^', "2"));

        return calc (ex, '/', arg1);
    }
    return "";
}

int main () {

    const char *src = "(sin (* 10 x))";

    const char *res = diff (src);
    double a = 1, b = 2;

    printf ("res : %s", res);
}

double operation (double a, double b, char op) {

    double val;
    switch (op) {
        case '+' : val = a + b; break;
        case '-' : val = a - b; break;
        case '*' : val = a * b; break;
        case '/' : val = a / b; break;
        //case '^' : val = pow (a, b); break;
    }

    return val;
}
