#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

enum {value, operator, variable, brace};

struct token {
    int type;
    char data[64];
} ;
struct expression {
    int ix;
    struct token t[1024];
};

struct token push (int type, const char *src) {
    struct token curr = {type};
    strcpy(curr.data, src);
    return curr;
}
const bool isparenthsis (char c) { return c == ')' || c == '('; }
const bool isoperator (char c) { return c == '+' || c == '-' || c == '*' || c == '/' || c == '^'; }
int getop (char c) {
    switch (c) {
        case '-' : return 1;
        case '+' : return 1;
        case '*' : return 2;
        case '/' : return 2;
        default  : return 0;
    }
}

struct expression tokenize (const char *input) {
    struct expression expr = {0};
    size_t size = strlen(input), i = 0;

    while (i < size) {
        char buffer[64] = {0}, it = 0;

        if (isdigit(input[i])) {
            while (isdigit(input[i]) || input[i] == '.') buffer[it++] = input[i++];
            expr.t[expr.ix++] = push(value, buffer);
        } else if (isspace(input[i])) {
            while (isspace(input[i])) i++;
        } else if (input[i] == '-') {
            if (expr.ix == 0 || expr.t[expr.ix - 1].type == operator) {
                buffer[0] = '-', i++, it = 1;
                while (isdigit(input[i]) || input[i] == '.') buffer[it++] = input[i++];
                expr.t[expr.ix++] = push(value, buffer);
            } else {
                buffer[0] = input[i++];
                expr.t[expr.ix++] = push(operator, buffer);
            }
        } else if (isoperator (input[i])) {
            buffer[0] = input[i++];
            expr.t[expr.ix++] = push(operator, buffer);
        } else if (isalpha(input[i])) {
            while (isalpha(input[i])) buffer[it++] = input[i++];
            expr.t[expr.ix++] = push(variable, buffer);
        } else {
            buffer[0] = input[i++];
            expr.t[expr.ix++] = push(brace, buffer);
        }
    }
    // for (int index = 0; index < expr.ix; index++) {
    //     printf("[%s]", expr.t[index].data);
    // }
    // printf("\n");
    return expr;
}
double operation (double a, char op, double b) {

    switch(op) {
        case '+' : return a + b;
        case '-' : return a - b;
        case '*' : return a * b;
        case '/' : return a / b;
    }
}
double calculate (const char *input) {

    struct expression code = tokenize(input);
    int index = 0, size = strlen(input);
    bool running = true;
    char *end = NULL;
    int iv = 0, io = 0;
    char ops[1024];
    double val[1024];

    while (running) {
        struct token curr = code.t[index];
        // printf("[%s]", code.t[index].data);

        if (curr.type == variable) {
            printf("variable : [%s]", curr.data);
        } else if (curr.type == value) {
            val[iv++] = strtod(curr.data, &end);
            //printf("value    : [%f]", strtod(curr.data, &end));
        } else if (curr.type == operator) {
            while (io > 0) {
                char op = ops[--io];
                double b = val[--iv], a = val[--iv];

                val[iv++] = operation(a,op,b);
            }

            ops[io++] = curr.data[0];
            // printf("operator : [%s]", curr.data);
        } else if (curr.type == brace) {
            printf("parenthes : [%s]", curr.data);
        }

        printf("\n");
        if (index >= (code.ix - 1)) running = false;
        index++;
    }

    while (io > 0) {
        char op = ops[--io];
        double b = val[--iv], a = val[--iv];

        val[iv++] = operation(a,op,b);
        printf("%i :: %f %c %f = %f", io, a, op, b, val[iv]);
    }

    printf("\n");
    return iv == 0 ? val[0] : val[--iv];
}

void shouldBe (const char *input, double expected) {

    double result = calculate (input);

    if (result != expected) {
        printf ("calculate (%s) should be %.18g, got %.18g\n", input, expected, result);
    }
}

void Test () {
    shouldBe ("1+2", 3.);
    /*
       shouldBe ("12*-1", -12.);
       shouldBe ("1 - 1", 0.);
       shouldBe ("1* 1", 1.);
       shouldBe ("1 /1", 1.);
       shouldBe ("123", 123.);
       shouldBe ("2 /2+3 * 4.75- -6", 21.25);
       shouldBe ("12* 123", 1476.);
       shouldBe ("2 / (2 + 3) * 4.33 - -6", 7.732);
       */
}


int main () {
    Test();
}
