#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define HASHSIZE 101

#define IS_VAR(x) (x == '_' || isalpha(x))

typedef int Type;

enum StatusCodes { OK_CODE, EMPTY_CODE, VAR_ERROR, ZERO_DIVISON, PARENTH_ERR, OPER_ERR, INVALID_CHAR };
enum {NONE, INTEGER, CHAR, DBLE, HASHV};
int status;

struct hashv {
    char *key;
    double val;
};

typedef union _Data {
  int i;
  char c;
  double d;
  struct hashv h;
} Data;

typedef struct node {
    Data data;
    struct node *next;
} Node;

typedef struct {
    int type, size;
    Node *root;
} Stack;

const Data nul = {0};

Stack *new (int type) {
    Stack *next = malloc (sizeof(Stack));
    next->root = NULL;
    next->type = type;
    next->size = 0;
    return next;
  }
_Bool is_empty (const Stack *stack) {
    return ((stack->root == NULL) ? 1 : 0);
}
void push (Stack *stack, void *raw) {
    stack->size++;
    Node *head = stack->root;

    Data data;
    switch (stack->type) {
        case INTEGER : data.i = *(int *)raw; break;
        case CHAR : data.c = *(char *)raw; break;
        case DBLE : data.d = *(double *)raw; break;
        case HASHV :

            data.h = *(struct hashv *)raw;
            data.h.key = strdup (data.h.key);

            while (head) {
                Data *curr = &head->data;

                if (strcmp (curr->h.key, data.h.key) == 0) {
                    curr->h.val = data.h.val;
                    return;
                }
                head = head->next;
            }
            break;
        default : break;
    }

    Node *cell = stack->root;
    stack->root = malloc (sizeof (Node));
    stack->root->data = data;
    stack->root->next = cell;
  }
Data pop (Stack *stack) {

    if (is_empty(stack)) return nul;
    stack->size--;
    Node *cell = stack->root;
    Data value = stack->root->data;
    stack->root = stack->root->next;

    free (cell);

    return value;
}
Data top (const Stack *stack) {
    return (is_empty(stack)) ? nul : stack->root->data;
}

struct hashv *lookup (const Stack *stack, char *key) {

    Node *root = stack->root;

    while (root) {

        Data *curr = &root->data;
        if (strcmp (curr->h.key, key) == 0) {
            return &curr->h;
        }

        root = root->next;
    }
    return NULL;
}

int getsub (char *it) {
    int pile = 0, index = -1;
    int size = strlen (it);

    do {
        if (*it == '(') pile++;
        if (*it == ')') pile--;
        if (pile == 0) return index;
        index++;
    } while (*it++ != '\0');

    return size;
}
int getop (char c) {
    if (c == '-' || c == '+') return 1;
    if (c == '*' || c == '/' || c == '%') return 2;
    return 0;
}

double operation (double a, double b, char op) {

    if (b == 0 && (op == '/' || op == '%')) {
        status = ZERO_DIVISON;
        return 0;
    }
    double val;

    switch (op) {
        case '+' : val = a + b; break;
        case '-' : val = a - b; break;
        case '*' : val = a * b; break;
        case '/' : val = a / b; break;
        case '%' : val = fmod (a, b); break;
    }

    return val;
}
double calc (char *expr) {
    const int size = strlen (expr);
    char *it = expr;

    bool running = true;
    int i, sign = 1;
    double val;
    char buff[2048];

    Stack *ops = new (CHAR), *values = new (DBLE);
    static Stack vars = {HASHV, 0, NULL};

    while (running) {

        if (*it == '-' && getop (*(it - 1))) {
            sign = -1;
        }

        if (IS_VAR (*it)) {

            i = 0;
            while (IS_VAR (*it) || isdigit (*it)) buff[i++] = *it++;
            buff[i] = '\0';

            struct hashv *np = lookup (&vars, buff);

            if (np == NULL) {
                status = VAR_ERROR;
            } else {
                val = np->val * sign;
                sign = 1;
                push (values, &val);
            }

        } else if (isdigit (*it)) {
            val = strtod (it, &it) * sign;
            push (values, &val);

            sign = 1;
        } else if (getop (*it)) {

            while (!is_empty (ops) && getop (top(ops).c) >= getop (*it)) {
                char op = pop (ops).c;
                double b = pop (values).d, a = pop (values).d;
                val = operation (a, b, op);
                push (values, &val);
            }

            push (ops, &*it);
            it++;
        } else if (*it == '=') {
            status = OK_CODE;
            val = calc (it + 1) * sign;
            struct hashv np = {buff, val, NULL};
            push (&vars, &np);
            return val;
        } else if (*it == '(') {
            int len = getsub (it);
            memcpy (buff, it + 1, len);
            buff[len] = '\0';
            val = calc (buff);
            push (values, &val);

            it += (len + 2);
        } else {
            if (*it != ' ') status = INVALID_CHAR;

            it++;
        }

        if ((it - expr) >= size) running = false;
    }

    while (!is_empty (ops)) {
        char op = pop (ops).c;
        double b = pop (values).d, a = pop (values).d;
        val = operation (a, b, op);
        push (values, &val);
    }

    return top (values).d;
}
int check (char *input) {
    char *expr = strdup (input), *it = expr;
    const int size = strlen (expr);

    int par = 0, nop = 0, nval = 0, spa = 0;

    while (*it) {
        if (isdigit (*it)) {
            strtod (it, &it);
            nval++;
        }
        if (IS_VAR (*it)) {

            while (IS_VAR (*it) || isdigit (*it)) it++;

            nval++;
        }
        if (getop (*it)) nop++;
        if (*it == '(')  par++;
        if (*it == ')')  par--;
        if (*it == ' ')  spa++;
        if (*it == '=') {
            if (it == &expr[size - 1]) return OPER_ERR;
            nop++;
        }

        if ((it - expr) < size) it++;
    }
    if (spa == size) return EMPTY_CODE;
    if (par != 0) return PARENTH_ERR;

    if (nval > 1 && nop == 0) return OPER_ERR;
    if (nop >= nval) return OPER_ERR;

    return OK_CODE;
  }
int initInterpreter () {
    return OK_CODE;
}
void closeInterpreter (void) {
    return;
}

int evaluate (char *input, Type *result) {
  /* evaluate: evaluate the string expression, and return a status code
  (any value other than OK_CODE and EMPTY_CODE is treated as an error).
  The result of evaluating the expression is placed in a variable
  by the pointer 'result' if the function returns OK_CODE. */
    status = check (input);

    if (status == OK_CODE) {
        *result = calc (input);
    } else {
        *result = 0;
    }

    return status;
}
////////////////////////////////////////////////////////////////////////////////

void list (const Stack *stack) {

    Node *root = stack->root;
    while (root) {

        Data curr = root->data;

        switch (stack->type) {
            case INTEGER : printf ("%i\n", curr.i); break;
            case DBLE : printf ("%.2f\n", curr.d); break;
            case CHAR : printf ("%c\n", curr.c); break;
            case HASHV: printf ("[%s] = %2.2f\n", curr.h.key, curr.h.val); break;
        }

        root = root->next;
    }
    printf ("\n");
  }

int main () {

    char *input = "_i=+"; //15

    int state = check (input);
    if (state != OK_CODE) {
        printf ("error");
    }
    /*
    // Expected an empty output for input: ''
    Expected an error for input: '1 2'
    Expected an error for input: '100 -'
    Expected an error for input: '_i=+'
    */

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
