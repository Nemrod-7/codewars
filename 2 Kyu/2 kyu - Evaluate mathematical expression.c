#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IS_DIGIT(x) ((x) >= '0' && (x) <= '9')

typedef struct node {
    double data;
    struct node *next;
} Node;

typedef struct {
    Node *root;
} Stack;

void push (Stack *stack, double data) {

    Node *cell = stack->root;
    stack->root = malloc (sizeof (Node));
    stack->root->data = data;
    stack->root->next = cell;
}
_Bool is_empty (const Stack *stack) {
  return ((stack->root == NULL) ? 1 : 0);
}
double pop (Stack *stack) {
    if (is_empty(stack)) return 0;
    Node *cell = stack->root;
    double value = stack->root->data;
    stack->root = stack->root->next;

    free (cell);

    return value;
}
double top (const Stack *stack) {
    return (is_empty(stack)) ? 0 : stack->root->data;
}
Stack *new () {
    Stack *next = malloc (sizeof(Stack));
    next->root = NULL;
    return next;
}

char *clean (const char *src) {
    const int size = strlen(src);
    int i = 0, it = 0;
    char *os = malloc (size * sizeof(char));

    for (; i < size; i++)
        if (src[i] != ' ')
            os[it++] = src[i];

    os[it] = '\0';
    return os;
  }

int getsub (char *it) {
    int pile = 0, index = -1;

    do {
        if (*it == '(') pile++;
        if (*it == ')') pile--;
        if (pile == 0) return index;
        index++;
    } while (*it++ != '\0');

    return 0;
}
int getop (char c) {
    switch (c) {
        case '-' : return 1;
        case '+' : return 1;
        case '*' : return 2;
        case '/' : return 2;
        default  : return 0;
    }
}
double operation (double a, double b, char op) {

    double val;
    switch (op) {
        case '+' : val = a + b; break;
        case '-' : val = a - b; break;
        case '*' : val = a * b; break;
        case '/' : val = a / b; break;
    }

    return val;
}

double calc (char *expr) {
       // convert expression to reverse polish notation using shunting-yard algorithm
    char *it = expr;
    Stack *val = new ();
    char ops[1024];
    int size = strlen (expr), o = 0, sign = 1;

    while ((it - expr) < size) {

        int op = getop (*it);

        if (*it == '-' && getop (*(it - 1))) {
            sign = -1;
            op = 0;
        }
        if (*it == '(') {
            int len = getsub (it);
            char sub[2048] = {};
            memcpy (sub, it + 1, len);
            sub[len] = '\0';
            push (val, calc (sub) * sign) ;
            sign = 1;
            it += len + 1;

        } else if (IS_DIGIT (*it)) {
            double num = strtod (it, &it) * sign;
            push (val, num);
            sign = 1;
        } else {
            if (op) {
                while (o != 0 && getop (ops[o - 1]) >= op) {

                    char ope = ops[--o];
                    double b = pop (val), a = pop (val);
                    //printf ("%f %c %f = %f \n", a, ope, b, operation (a, b, ope));
                    push (val, operation (a, b, ope));
                }

                ops[o++] = *it;
            }
            it++;

        }

    }

    while (o > 0) {
        char op = ops[--o];
        double b = pop (val), a = pop (val);
        //printf ("%f %c %f = %f \n", a, op, b, operation (a, b, op));
        push (val, operation (a, b, op));
    }

    return pop (val);
}
double calculate (const char *src) {
    //printf ("[%s]\n", src);
    char *expr = clean (src);
    double res =  calc (expr);
    free (expr);
    return res;
}
double calculate2 (const char *src) {
    // convert expression to reverse polish notation using shunting-yard algorithm
    char *expr = clean (src), *it = expr;
    double val[1024], sign = 1;
    char sub[1024] = {}, ops[1024] = {};
    int size = strlen(expr), v = 1, o = 0;

    while ((it - expr) < size) {

        int op = getop (*it);

        if (*it == '-' && getop (*(it - 1))) {
            sign = -1;
            op = 0;
        }

        if (*it == '(') {
            int len = getsub (it);
            memcpy (sub, it + 1, len);
            sub[len] = '\0';

            val[v++] = calculate2 (sub) * sign;
            sign = 1;
            it += len + 1;
        } else if (IS_DIGIT (*it)) {
            val[v++] = strtod (it, &it) * sign;
            sign = 1;
        } else {
            if (op) {
                while (o != 0 && getop (ops[o - 1]) >= op) {
                    char ope = ops[--o];
                    double b = val[--v], a = val[--v];
                    //printf ("%4.2f %c %2.2f = %4.2f\n", a, ope, b, operation (a, b, ope));
                    val[v++] = operation (a, b, ope);
                }

                ops[o++] = *it;
            }
            it++;
        }

    }

    free (expr);
    //printf ("%i ", o);
    while (o != 0) {
        char op = ops[--o];
        double b = val[--v], a = val[--v];

        while (v < 0) v++;
        val[v++] = operation (a, b, op);
        //printf ("%4.2f %c %2.2f = %f\n", a, op, b, operation (a, b, op));
    }

    //printf ("%f ", (v == 0 ? val[0] : val[--v]));
    return v == 0 ? val[0] : val[--v];
}
void shouldBe (const char *input, double expected) {
  double result = calculate (input);
  if (result != expected)
    printf ("calculate (%s) should be %.18g, got %.18g\n", input, expected, result);
}

void Test () {
    shouldBe ("12*-1", -12.);
    shouldBe ("1+1", 2.);
    shouldBe ("1 - 1", 0.);
    shouldBe ("1* 1", 1.);
    shouldBe ("1 /1", 1.);
    shouldBe ("123", 123.);
    shouldBe ("2 /2+3 * 4.75- -6", 21.25);
    shouldBe ("12* 123", 1476.);
    shouldBe ("2 / (2 + 3) * 4.33 - -6", 7.732);
    /*
    */
}

int main () {

    const char *expr = "((((9.68/7.49+6.55-8.62-8.11*9.49*9.27/8.00/ -4.91/8.31/ -4.06-8.41-9.67+6.10-9.98))))";
    // 84.9887910188346183

    printf ("%f" ,calculate (expr));

    //printf ("%f", calculate2 (expr));

    //printf ("%f ", calculate ("((63 - -20 / 84 - 33 * 87 + -17 * 41 + -10))"));
    // should be 19.5741151934738689, got -2.914
    //Test ();
}
/*
double calculate (const char *src) {
    // convert expression to reverse polish notation using shunting-yard algorithm
    char *expr = clean (src), *it = expr;
    double val[1024], sign = 1;
    char sub[1024] = {}, ops[1024] = {};
    int size = strlen(expr), v = 1, o = 0;

    while ((it - expr) < size) {

        int op = getop (*it);

        if (*it == '-' && getop (*(it - 1))) {
            sign = -1;
            op = 0;
        }

        if (*it == '(') {
            int len = getsub (it);
            memcpy (sub, it + 1, len);
            sub[len] = '\0';

            val[v++] = calculate (sub) * sign;
            sign = 1;
            it += len + 1;
        } else if (IS_DIGIT (*it)) {
            val[v++] = strtod (it, &it) * sign;
            sign = 1;
        } else {
            if (op) {
                while (o != 0 && getop (ops[o - 1]) >= op) {
                  //printf ("%c %c\n", ops[o - 1], *it);
                    char ope = ops[--o];
                    double b = val[--v], a = val[--v];
                    //printf ("%4.2f %c %2.2f = %4.2f\n", a, ope, b, operation (a, b, ope));
                    val[v++] = operation (a, b, ope);
                }

                ops[o++] = *it;
            }
            it++;
        }

    }

    free (expr);
    //printf ("%i ", o);
    while (o != 0) {
        char op = ops[--o];
        double b = val[--v], a = val[--v];

        while (v < 0) v++;
        val[v++] = operation (a, b, op);
        //printf ("%4.2f %c %2.2f = %f\n", a, op, b, operation (a, b, op));
    }

    //printf ("%f ", (v == 0 ? val[0] : val[--v]));
    return v == 0 ? val[0] : val[--v];
  }
*/
