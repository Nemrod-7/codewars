#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

extern char *strdup (const char *);

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

const char *oper = "+-/*";
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
_Bool is_empty (const Stack *stack) {
  return ((stack->root == NULL) ? 1 : 0);
}
AST *stop (const Stack *stack) {
    return (is_empty (stack)) ? NULL : stack->root->data;
}
void spush (Stack *stack, AST *data) {

    Node *cell = stack->root;
    stack->root = malloc (sizeof (Node));
    stack->root->data = data;
    stack->root->next = cell;
}
AST *spop (Stack *stack) {
    if (is_empty(stack)) return 0;
    Node *cell = stack->root;
    AST *curr = stack->root->data;
    stack->root = stack->root->next;

    free (cell);

    return curr;
}

int getop (int op) {
    if (op == plus || op == min) return 1;
    if (op == mul || op == div) return 2;
    return  0;
}
int getarg (AST **argv, int val) { // get arguments from stack

    for (int i = 0; argv[i] != NULL; i++)
        if (argv[i]->n == val) return i;

    return -1;
}
int getsub (char **in, char **os) { // get parenthesis routine
    int pile = 0, index = 0;

    do {
        if (!strcmp (*in, "(")) pile++;
        if (!strcmp (*in, ")")) pile--;
        if (pile == 0) {
            os[index] = NULL;
            return index;
        }
        os[index++] = strdup (*in);
    } while (strcmp (*in++, "\0"));

    return 0;
}
Stack *postorder (AST *root) {      // post order traversal

    AST *temp = root;
    Stack *s1 = new (), *s2 = new ();
    spush (s1, temp);

    while (!is_empty (s1)) {
        temp = spop (s1);
        spush (s2, temp);

        if (temp->a) spush(s1, temp->a);
        if (temp->b) spush(s1, temp->b);
    }

    return s2;
}
AST *mktree (AST **argv, char **expr) { // make an AS tree

    char **it = expr;
    _Bool running = 1;
    Stack *ops = new(), *tree = new ();

    while (running) {
        char id = *it[0];

        if (id == '(') {
            char *sub[64] ;// = malloc (32 * sizeof (char *));
            it += getsub (it, sub);
            spush (tree, mktree (argv, sub + 1));

        } else if (isalpha (id)) {

            int pos = getarg (argv, id);
            if (pos != -1) spush (tree, Arg (pos));
            //printf ("%i", pos);
        } else if (isdigit (id)) {
            spush (tree, Imm (atoi (*it)));
        } else if (strchr (oper, id)) {
            int op;

            switch (id) {
                case '+' : op = plus; break;
                case '-' : op = min; break;
                case '*' : op = mul; break;
                case '/' : op = div; break;
                default  : op = 0; break;
            }

            AST *temp = Bin (op, NULL, NULL);

            while (!is_empty (ops) && getop (stop (ops)->op) >= getop (op)) {
                AST *curr = spop (ops);
                curr->b = spop (tree), curr->a = spop (tree);
                spush (tree, curr);
            }
            spush (ops, temp);
        }

        if (*(it + 1) == NULL) running = 0;
        it++;
    }

    while (!is_empty (ops)) {
        AST *curr = spop (ops);
        curr->b = spop (tree), curr->a = spop (tree);
        spush (tree, curr);
    }
    /*
    */
    return stop (tree);
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
    tok[index] = NULL;

    return tok;
}
AST *pass1 (const char *program) {  // Returns an un-optimized AST
    int i = 0;
    char **tokens = tokenize (program), **it = tokens + 1;
    AST **argv = malloc (5 * sizeof (AST *));

    for (; strcmp (*it, "]") ; it++, i++) {
        argv[i] = Arg (*it[0]);
    }
    argv[i] = NULL;

    return mktree (argv, it + 1);
}
AST *pass2 (AST *ast) { // Returns an AST with constant expressions reduced

    if (ast->op == imm || ast->op == arg) return ast;
    ast->a = pass2 (ast->a), ast->b = pass2 (ast->b);
    AST *a = ast->a, *b = ast->b;

    if (a->op == imm && b->op == imm) {
        int val = 0;

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
        AST *node = spop (tree);

        if (node->op == imm) {
            ptr += sprintf (ptr, "IM %i\n", node->n);
        } else if (node->op == arg) {
            ptr += sprintf (ptr, "AR %i\n", node->n);
        } else {
            ptr += sprintf (ptr, "PO\nSW\nPO\n");

            if (node->op == plus) {
                ptr += sprintf (ptr, "AD\n");
            } else if (node->op == min) {
                ptr += sprintf (ptr, "SU\n");
            } else if (node->op == mul) {
                ptr += sprintf (ptr, "MU\n");
            } else if (node->op == div) {
                ptr += sprintf (ptr, "DI\n");
            }
        }

        ptr += sprintf (ptr, "PU\n");
    }

    return Asm;
}
char *compile (const char* program) { return pass3 (pass2 (pass1 (program))); }

void swap (int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int simulate (const char *ins, const int *args) {
  int r0 = 0, r1 = 0, t;
  for (; ins && *ins; (ins = strchr (ins, '\n')) ? ++ins : 0x60d1510f)

  printf ("%s", ins);
  /*
         if (!memcmp (ins, "IM", 2)) r0 = atoi (ins+3);
    else if (!memcmp (ins, "AR", 2)) r0 = args[atoi (ins+3)];
    else if (!memcmp (ins, "SW", 2)) t = r0, r0 = r1, r1 = t;
    else if (!memcmp (ins, "PU", 2)) spush (r0);
    else if (!memcmp (ins, "PO", 2)) r0 = spop ();
    else if (!memcmp (ins, "AD", 2)) r0 += r1;
    else if (!memcmp (ins, "SU", 2)) r0 -= r1;
    else if (!memcmp (ins, "MU", 2)) r0 *= r1;
    else if (!memcmp (ins, "DI", 2)) r0 /= r1;
    */
  return r0;
}

int main () {
    const char *src = "[ x y z ] ( 2*3*x + 5*y - 3*z ) / (1 + 3 + 2*2)";
    AST *node = pass1 (src);

    showtree(node);
    //char *Asm = compile (src);
    //const int argv[] = {4,0,0};
    //printf ("%s", Asm);
    //printf ("%i", simulate (Asm, (int[]) {4,0,0}));

    printf ("\nend");
}
////////////////////////////////////////////////////////////////////////////////

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
