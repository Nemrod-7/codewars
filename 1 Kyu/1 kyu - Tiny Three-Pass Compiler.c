#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define div divide

typedef struct AST {
    enum op {imm, arg, plus, min, mul, div} op;
    struct AST *a, *b;
    int n;
} AST;

typedef struct node {
    AST *data;
    struct node *next;
} Node;

typedef struct {
    Node *root;
} Stack;

const char *oper = "+-/*", *delim = "[()]";

AST *Arg (int n) {
  AST *next = malloc (sizeof (AST));
  next->op = arg;
  next->n = n;
  next->a = next->b = NULL;
  return next;
}
AST *Imm (int n) {
  AST *next = malloc (sizeof (AST));
  next->op = imm;
  next->n = n;
  next->a = next->b = NULL;
  return next;
}
AST *Bin (enum op op, AST *a, AST *b) {
    AST *next = malloc (sizeof (AST));
    next->op = op;
    next->n = 0;
    next->a = a, next->b = b;
    return next;
}

Stack *new () {
    Stack *next = malloc (sizeof(Stack));
    next->root = NULL;
    return next;
  }
bool is_empty (const Stack *stack) {
  return ((stack->root == NULL) ? true : false);
}
AST *top (const Stack *stack) {
    return (is_empty (stack)) ? NULL : stack->root->data;
}
void push (Stack *stack, AST *data) {

    Node *cell = stack->root;
    stack->root = malloc (sizeof (Node));
    stack->root->data = data;
    stack->root->next = cell;
}
AST *pop (Stack *stack) {
    if (is_empty(stack)) return 0;
    Node *cell = stack->root;
    AST *curr = stack->root->data;
    stack->root = stack->root->next;

    free (cell);

    return curr;
}

// Turn a program string into an array of tokens (last entry is 0).
// Each token is either '[', ']', '(', ')', '+', '-', '*', '/', a variable
// name or a number (as a string)
char **tokenize (const char* program) {
    
    int size = strlen (src), index = 0;
    char **tok = malloc (size * sizeof (char*)), *it = src;

    while (size > (it - src)) {
        while (*it == ' ') it++;

        char buffer[16] = {}, *ptr = buffer;

        if (isdigit (*it)) {
            while (isdigit (*it)) *ptr++ = *it++;
        } else {
            *ptr++ = *it++;
        }
        tok[index++] = strdup (buffer);
    }
    tok[index] = strdup ("\0");

    return tok;
}

Stack *postorder (AST *root) {      // post order traversal

    AST *temp = root;
    Stack *s1 = new (), *s2 = new ();
    push (s1, temp);

    while (!is_empty (s1)) {
        temp = pop (s1);
        push (s2, temp);

        if (temp->a) push(s1, temp->a);
        if (temp->b) push(s1, temp->b);
    }

    return s2;
}
int getsub (char **it) {
    int pile = 0, index = -1;

    do {
        if (!strcmp (*it, "(")) pile++;
        if (!strcmp (*it, ")")) pile--;
        if (pile == 0) return index;
        index++;
    } while (strcmp (*it++, "\0"));

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
int operation (int a, int b, char op) {

    int val;
    switch (op) {
        case '+' : val = a + b; break;
        case '-' : val = a - b; break;
        case '*' : val = a * b; break;
        case '/' : val = a / b; break;
    }

    return val;
}
int getpos (Stack *args, int val) {
    Node *root = args->root;
    int pos = 0;

    while (root) {
        if (root->data->n == val) return pos;
        pos++;
        root = root->next;
    }

    return -1;
}

AST *mktree (Stack *args, char **expr) {

    int it = 0;
    bool running = true;
    Stack *ops = new(), *val = new ();

    while (running) {

        char id = expr[it][0];
        // if (strchr (oper, *it))
        if (id == '(') {

        } else if (isalpha (id)) {

            int pos = getpos (args, id);
            if (pos != -1) push (val, Arg (pos));
            //printf ("%i", pos);

        } else if (isdigit (id)) {
            push (val, Imm (atoi (expr[it])));
        } else if (getop (id)) {
            int op;

            switch (id) {
                case '+' : op = plus; break;
                case '-' : op = min; break;
                case '*' : op = mul; break;
                case '/' : op = div; break;
                default  : op = -1; break;
            }

            if (!is_empty(val)) {
                AST *b = pop (val), *a = pop (val);

                push (ops, Bin (op, a, b));
            }
        }

        if (!strcmp (expr[it], "\0")) running = false;
        it++;
    }

    return top (ops);
}

AST *pass1 (const char *program) {  // Returns an un-optimized AST
    char **tokens = tokenize (program), **it = tokens + 1;

    Stack *args = new ();
    int i = 0;

    while (strcmp (*it, "]")) {
        push (args, Arg (*it[0]));
        it++;
    }

    return mktree (args, it + 1);
}
AST *pass2 (AST *ast) { // Returns an AST with constant expressions reduced

    if (ast->op == imm || ast->op == arg) return ast;
    ast->a = pass2 (ast->a), ast->b = pass2 (ast->b);

    if (ast->a->op == imm && ast->b->op == imm) {
        switch (ast->op) {
            case plus : ast->a->n + ast->b->n;
            case min : ast->a->n - ast->b->n;
            case mul : ast->a->n * ast->b->n;
            case div : ast->a->n / ast->b->n;
        }
        ast->a = ast->b = NULL;
    }

    return ast;
}
char *pass3 (AST *ast) {  // Returns assembly instructions

    char *Asm = malloc (1024 * sizeof (char)), *ptr = Asm;
    Stack *tree = postorder (ast);

    while (!is_empty (tree)) {
        AST *node = pop (tree);

        if (node->op == imm) {
            ptr = sprintf (ptr, "IM %i,", node->n);
        } else if (node->op == arg) {
            ptr = sprintf (ptr, "AR %i,", node->n);
        } else {
            ptr = sprintf (ptr, "PO,SW,PO,");

            if (node->op == plus) {
                ptr = sprintf (ptr, "AD,");
            } else if (node->op == min) {
                ptr = sprintf (ptr, "SU,");
            } else if (node->op == mul) {
                ptr = sprintf (ptr, "MU,");
            } else if (node->op == div) {
                ptr = sprintf (ptr, "DI,");
            }
        }
        ptr = sprintf (ptr, "PU,");
    }

    return Asm;
}
char *compile (const char* program) { return pass3 (pass2 (pass1 (program))); }


int main () {
    const char *src = "[x] x+2*5";

    pass1 (src);
}
