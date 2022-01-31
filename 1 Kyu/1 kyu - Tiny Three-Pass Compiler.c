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

typedef struct _node {
    AST *data;
    struct _node *next;
} Node;
typedef struct _stack {
    Node *root;
} Stack;

const char *oper = "+-/*", *delim = "[()]";
////////////////////////////////////////////////////////////////////////////////
void showvect (char **code) {
    for (int i = 0; strcmp (code[i], "\0"); i++) {
        printf ("%s", code[i]);
    }
}
void shownode (AST *node) {
    const char *oper[32] = {"imm", "arg", "+", "-", "*", "/"};

    if (node) {
        if (node->op < plus)
            printf ("(%s %i", oper[node->op], node->n);
        else
            printf ("bin (%s ", oper[node->op]);
    }
}
void showtree (AST *node) {
    AST *root = node;

    if (root) {
        shownode (root);
        showtree (root->a);
        showtree (root->b);
        printf (")");
    }
}
////////////////////////////////////////////////////////////////////////////////

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

char **tokenize (const char* src) { // Turn a program string into an array of tokens (last entry is 0)

    int size = strlen (src), index = 0;
    char *expr = strdup (src);
    char **tok = malloc (size * sizeof (char*)), *it = expr;

    while (size > (it - expr)) {
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

int getarg (const Stack *args, int val) { // get arguments from stack
    Node *root = args->root;
    int pos = 0;

    while (root) {
        if (root->data->n == val) return pos;
        pos++;
        root = root->next;
    }

    return -1;
}
int getsub (char **in, char **os) { // get parenthesis routine
    int pile = 0, index = 0;

    do {
        if (!strcmp (*in, "(")) pile++;
        if (!strcmp (*in, ")")) pile--;
        if (pile == 0) {
            os[index] = strdup ("\0");
            return index;
        }
        os[index++] = strdup (*in);
    } while (strcmp (*in++, "\0"));

    return 0;
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
AST *mktree (const Stack *args, char **expr) { // make an AS tree

    char **it = expr;
    bool running = true;
    Stack *ops = new(), *tree = new ();

    while (running) {

        char id = *it[0];

        if (id == '(') {
            char *sub[64] ;// = malloc (32 * sizeof (char *));
            it += getsub (it, sub) + 1;
            push (tree, mktree (args, sub + 1));

        } else if (isalpha (id)) {

            int pos = getarg (args, id);
            if (pos != -1) push (tree, Arg (pos));
            //printf ("%i", pos);
        } else if (isdigit (id)) {
            push (tree, Imm (atoi (*it)));
        } else if (strchr (oper, id)) {
            int op;

            switch (id) {
                case '+' : op = plus; break;
                case '-' : op = min; break;
                case '*' : op = mul; break;
                case '/' : op = div; break;
                default  : op = -1; break;
            }
            AST *temp = Bin (op, NULL, NULL);

            while (!is_empty (ops) && top (ops)->op >= op) {
                AST *curr = pop (ops);
                curr->b = pop (tree), curr->a = pop (tree);
                push (tree, curr);
            }
            /*
            */
            push (ops, temp);
        }

        if (!strcmp (*(it + 1), "\0")) running = false;
        it++;
    }

    while (!is_empty (ops)) {
        AST *curr = pop (ops);
        curr->b = pop (tree), curr->a = pop (tree);
        push (tree, curr);
    }
    return top (tree);
}

AST *pass1 (const char *program) {  // Returns an un-optimized AST
    char **tokens = tokenize (program), **it = tokens + 1;

    Stack *args = new ();

    while (strcmp (*it, "]")) {
        push (args, Arg (*it[0]));
        it++;
    }

    return mktree (args, it + 1);
}
AST *pass2 (AST *ast) { // Returns an AST with constant expressions reduced

    if (ast->op == imm || ast->op == arg) return ast;
    ast->a = pass2 (ast->a), ast->b = pass2 (ast->b);
    AST *a = ast->a, *b = ast->b;

    if (a->op == imm && b->op == imm) {
        int val;

        switch (ast->op) {
            case plus : val = a->n + b->n; break;
            case min : val = a->n - b->n; break;
            case mul : val = a->n * b->n; break;
            case div : val = a->n / b->n; break;
            default : break;
        }

        ast->op = imm;
        ast->n = val;
        ast->a = ast->b = NULL;
        free (a), free (b);
    }

    return ast;
}
char *pass3 (AST *ast) {  // Returns assembly instructions

    char *Asm = malloc (1024 * sizeof (char)), *ptr = Asm;
    Stack *tree = postorder (ast);

    while (!is_empty (tree)) {
        AST *node = pop (tree);

        if (node->op == imm) {
            ptr += sprintf (ptr, "IM %i,", node->n);
        } else if (node->op == arg) {
            ptr += sprintf (ptr, "AR %i,", node->n);
        } else {
            ptr += sprintf (ptr, "PO,SW,PO,");

            if (node->op == plus) {
                ptr += sprintf (ptr, "AD,");
            } else if (node->op == min) {
                ptr += sprintf (ptr, "SU,");
            } else if (node->op == mul) {
                ptr += sprintf (ptr, "MU,");
            } else if (node->op == div) {
                ptr += sprintf (ptr, "DI,");
            }
        }

        ptr += sprintf (ptr, "PU,");
    }

    return Asm;
}
char *compile (const char* program) { return pass3 (pass2 (pass1 (program))); }

int main () {
    const char *src = "[x] x + 2";
    AST *node = pass1 (src);
    showtree (node);

    printf ("\nend");
}
////////////////////////////////////////////////////////////////////////////////
int getop (char c) { // deprecated

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
