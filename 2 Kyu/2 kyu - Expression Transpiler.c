#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

extern char* strdup(const char*);

#define IS_VAR(x) ((x) == '_' || isalnum(x))
enum {NONE, FUNC, VAR, LAMB, VAR2};

typedef struct AST {
    enum op {func, pam, lmbd, tmt} op;
    char n;
    struct AST *a, *b;
} AST;

typedef struct _node {
    AST *data;
    struct _node *next;
} Node;
typedef struct _stack {
    Node *root;
} Stack;

/*
enum {FUNC, PARAM, LAMBD, LMBPAM, LMBTMT};

    source :
function    ::= expression "(" [parameters] ")" [lambda] | expression lambda
expression  ::= nameOrNumber | lambda
parameters  ::= expression ["," parameters]

lambda      ::= "{" [lambdaparam "->"] [lambdastmt] "}"
lambdaparam ::= nameOrNumber ["," lambdaparam]
lambdastmt  ::= nameOrNumber [lambdastmt]
--------------------------------------------------------------------------------
    target :
function    ::= expression "(" [parameters] ")"
expression  ::= nameOrNumber | lambda
parameters  ::= expression ["," parameters]

lambda      ::= "(" [lambdaparam] "){" [lambdastmt] "}"
lambdaparam ::= nameOrNumber ["," lambdaparam]
lambdastmt  ::= nameOrNumber ";" [lambdastmt]
*/

char **tokenize (const char* src) { // Turn a program string into an array of tokens (last entry is 0)

    int size = strlen (src), index = 0;
    char *expr = strdup (src);
    char **tok = malloc (size * sizeof (char*)), *it = expr;

    while (size > (it - expr)) {
        while (*it == ' ') it++;
        char buffer[16] = {}, *ptr = buffer;

        if (isalnum (*it)) {
            while (isalnum (*it)) *ptr++ = *it++;
        } else {
            if (*it == '-' && *(it + 1) == '>') {
                *ptr++ = *it++;
            }
            *ptr++ = *it++;
        }
        tok[index++] = strdup (buffer);
    }
    tok[index] = NULL;

    return tok;
}

int detect (char *it) {

    while (*it) {

        if (*it == '{') return LAMB; // function
        if (*it == '(') return FUNC; // function
        if (*it == ',') return VAR; // variable
        if (*it == ')') return VAR; // variable
        if (*it == '}') return VAR; // variable
        if (*it == '-') return VAR; // variable
        //if (isalnum(*it)) return VAR2; // variable
        //if (*it == ' ') it++;
        it++;
    }

    return NONE;
}
char *mktree (char **expr) {

    char *os = malloc (64 * sizeof (char)), *ptr = os;
    bool running = true;
    int mode;

    while (running) {
        char id = *expr[0];

        if (id == '(') {
          char *sub[64] ;
          //expr++;
          //printf ("param ::= ");
          while (strcmp (*expr, ")")) {
              ptr += sprintf (ptr, "%s", *expr);
              //if (strcmp (*expr, ","))
                  //printf ("[%s]", *expr);
              expr++;
          }
          //printf ("\n");
        }
        else if (id == '{') {
            expr++;
            *ptr++ = '(';
            //printf ("lambd ::= pam : ");
            /*
            while (strcmp (*expr, "->")) { //printf ("[%s]", *expr);
                ptr += sprintf (ptr, "%s", *expr++);
                //expr++;
            }
            expr++;
            */
            ptr += sprintf(ptr, "){");
            //printf (" tmt : ");
            while (strcmp (*expr, "}")) {
                ptr += sprintf(ptr, "%s;", *expr++); //printf ("[%s]", *expr);
                //expr++;
            }

            ptr += sprintf(ptr, "}");
        }

        else {

            ptr += sprintf (ptr, *expr);

            if (*(expr + 1)) {
                if (strcmp (*(expr + 1), "(")) *ptr++ = '(';
            }

            //printf ("funct ::= %s\n", *expr);
        }

        if (*(expr + 1) == NULL) running = false;
        expr++;
    }

    int cnt = 0;
    for (int i = 0; os[i] != '\0'; i++) {
        if (os[i] == '(') cnt++;
        if (os[i] == ')') cnt--;
    }

    if (cnt > 0) ptr += sprintf (ptr, ")");
    /*
    */
    printf ("%s\n", os);
    return os;
}

char *transpile (const char* source) {

    // if (strcmp (source, "{}{}") == 0) return strdup ("(){}((){})");
    // if (strcmp (source, "{}()") == 0) return strdup ("(){}()");

    const int size = strlen (source);
    char *os = malloc (size * sizeof(char));
    char **tokens = tokenize (source), **it = tokens;
    AST **argv = malloc (sizeof (AST *));

    os = mktree (tokens);

    //printf ("funct ::= %s \n", segment2 (expr, "("));
    //printf ("param ::= %s \n", segment2 (expr, ")"));

    return os;
}
///////////////////////////Assert////////////////////////////////
void fromTo (const char *tested, const char *expect) {

    char *res = transpile (tested);
    static int idx;

    if (strcmp (res, expect) != 0) {
        printf ("test %i got : [%s] expected : [%s]\n",idx, res, expect);
    }
    /*
    */
    idx++;
}
void shouldFail (const char *source) { fromTo (source, ""); }

void Test () {

  //fromTo ("{}{}","(){}((){})");
  //fromTo ("{}()" , "(){}()");

  fromTo ("fun(a, b)" , "fun(a,b)");
  fromTo ("{a -> a}(1)" , "(a){a;}(1)");
  fromTo ("f(x){a->}", "f(x,(a){})");

  /*
  fromTo ("f({a->})", "f((a){})");
  fromTo ("f({a->a})", "f((a){a;})");
  fromTo ("{a->a}(233)", "(a){a;}(233)");
  fromTo ("invoke  (       a    ,   b   )", "invoke(a,b)");
  fromTo ("invoke(a, b)", "invoke(a,b)");
  fromTo ("f { a }","f((){a;})");
  fromTo ("fun(a) {}" , "fun(a,(){})");
  fromTo ("invoke  (       a    ,   b   ) { } ", "invoke(a,b,(){})");
  fromTo ("fun(a)" , "fun(a)");
  fromTo ("call(\n){}", "call((){})");
  fromTo ("f(a,b){a->a}", "f(a,b,(a){a;})");
    fromTo ("fun()", "fun()");
    fromTo ("call()", "call()");
    fromTo ("fun {}" , "fun((){})");
    fromTo ("fun { a, b -> a b }" , "fun((a,b){a;b;})");
    fromTo ("f { a -> }","f((a){})");
    shouldFail ("%^&*(");
    shouldFail ("x9x92xb29xub29bx120()!(");
    fromTo ("call({})", "call((){})");


    */

}
void Test2 () {
    fromTo ("f(->)" , "");
    fromTo ("a b c" , "");

    fromTo ("invoke({},{})" , "invoke((){},(){})");
    fromTo ("run{->a}" , "");
    fromTo ("f({1a->a})" , "");

    fromTo ("i(,{})" , "");
    fromTo ("f(a,)" , "");
    fromTo ("f (,a)" , "");
    fromTo ("f( a v)" , "");
    fromTo ("gg     (  a  )  " , "gg(a)");
    fromTo("a b c","" );

    fromTo ("1a()" , "");
    fromTo ("f(12,ab)c" , "");

    fromTo ("run(a){as we can}" , "run(a,(){as;we;can;})");
    fromTo ("f({jj->asWeCan})" , "f((jj){asWeCan;})"); // failed, result was ""
    fromTo ("f({a, b->a})" , "f((a,b){a;})");
    fromTo ("call(1a)" , "");

    fromTo ("{a}()" , "(){a;}()");

    fromTo ("(12,ab)" , "");
}
void Test3() {

  /*
  Transpile from "{a->a}(cde,y,z){x y,d -> stuff}  " to "" failed, result was "(a){a;}(cde,y,z,(y,d){stuff;})"

  Transpile from "{}{}{}" , "" failed, result was "(){},(){},(){}"
  Transpile from "{}(){}" , "(){}((){})" failed, result was "(){},(){}"

  */
}
/////////////////////////////////////////////////////////////////
int main () {


  //Test ();

  transpile ("invoke  (       a    ,   b   ) { } "); //, "invoke(a,b,(){})"
  printf ("finish");
  /*
  Test2 ();

  //fromTo ("f { a }","f((){a;})");
  printf ("%i", param ("a, b"));
  */
}

////////////////////////////////////////////////////////////////////////
////////////////////////    queue implementation   ///////////////////////
struct token {
    int type;
    char *str;
};

char *cat (const char *s0, ...);              // cat all strings together, stop at argument value 0

typedef struct list {
  void *data;
  struct list *next;
} list;

list *node (void *data);           // create one-element list
list *concat (list *l1, list *l2); // concatenate two lists, modifies l1

typedef struct _queue {
    list *frt, *bck;
} queue;

bool is_empty (const queue *Q) { return (!Q->frt || !Q->bck);}
void push (queue *Q, struct token *data) {
    list *cell = malloc (sizeof (list));
    cell->data = data;
    cell->next = NULL;

    if (is_empty (Q)) {
        Q->bck = Q->frt = cell;
    } else {
        Q->bck = Q->bck->next = cell;
    }
}
void pop (queue *Q) {

    list *frtnode = Q->frt;
    Q->frt = Q->frt->next;

    free (frtnode);
}
void delete (queue *Q) {
  list *nd = Q->frt;
  while (nd) {
    list *tmp = nd->next;
    free(nd);
    nd = tmp;
  }
  free(Q);
}

queue *init () {
  queue *new = malloc (sizeof(queue));
  new->frt = new->bck = NULL;
  return new;
}
/////////////////////////////////////////////////////////////////////////

char *segment2 (const char *s, const char *e) {  // create string from characters between s (inclusive) and e (exclusive)

    char *os = malloc (strlen(s) * sizeof (char)), *ptr = os;
    //s++;
    while (*s && *s != *e) {
        *ptr++ = *s++ ;
    }

    return os;
};
int getsub (char **in, char **os) { // get parenthesis routine
    int pile = 0, index = 0;

    do {
        if (!strcmp (*in, "(")) pile++;
        if (!strcmp (*in, ")")) pile--;
        os[index++] = strdup (*in);

        if (pile == 0) {
          os[index] = NULL;
          return index;
        }
    } while (strcmp (*in++, "\0"));

    return 0;
  }
/*
char *getvar (char *src, char *var) {
    int i = 0;
    while (IS_VAR(*src)) var[i++] = *src++;
    var[i] = '\0';
    return src;
}
bool valid_var (char *it) {
    bool dig = false, alp = false;
    if (*it == '_') return true;

    while (*it) {
        if (isdigit (*it)) dig = true;
        if (isalpha (*it)) alp = true;
        it++;
    }
    if (dig && alp) return false;
    return true;
  }

char *lambda3 (char *expr) {
  char *lmb = strstr (expr, "->"), *it = expr;
  const int size = strlen (expr);
  const int lim = lmb == NULL ? size : lmb - expr;
  if (size == 0) return strdup ("(){}");
  int nval = 0;
  char buff[32], *val[5];

  while (it - expr < lim) {                               // geting values
      while (*it == ' ') it++;
      if (IS_VAR (*it)) {
          it = getvar (it, buff);
          if (valid_var (buff) == false) return NULL;
          val[nval++] = strdup (buff);
      }
      it++;
  }

  char *os = malloc (64 * sizeof (char)), *ptr = os;
  *ptr++ = '(';

  if (lmb) {                                  // making param side
      for (int i = 0; i < nval; i++) {
          ptr += sprintf (ptr ,"%s", val[i]);
          if (i < nval - 1) *ptr++ = ',';
      }
  }

  ptr += sprintf (ptr, "){");

  if (!lmb) {
      for (int i = 0; i < nval; i++) {         // making lambda side
          ptr += sprintf (ptr ,"%s;", val[i]);
      }
  } else {
        if (nval == 0) return NULL;

        while (it - expr < size) {
            if (IS_VAR (*it)) {
                it = getvar (it, buff);
                ptr += sprintf (ptr ,"%s;", buff);
            }
            it++;
        }
  }
  *ptr = '}';
  return os;
}

bool param  (char *expr) {

    int size = strlen (expr);
    int nvar = 0, npam = 0;
    char *it = expr;

    while (it - expr < size) {
        while (*it == ' ') it++;

        char buff[32];
        int flag = 0;

        if (*it == '{') {
            while (*it && *it != '}') it++;

            flag = true;
        } else {
            while (*it && *it != ',' && isgraph (*it)) {
                if (*it == '-') return false;

                if (*it != ' ') flag = true;
                it++;
            }
            if (*it == ',') nvar++;
        }
        npam += flag;
        it++;
    }
    //printf ("%i %i\n", npam, nvar + 1);
    if (npam && npam != nvar + 1) return false;

    return true;
}
char *clean (const char *src) {
  const int size = strlen(src);
  char *os = malloc (size * sizeof(char)), *ptr = os;

  for (int i = 0; i < size; i++)
      if (isgraph (src[i]) || (src[i] == ' ' && isalnum (src[i + 1])))
          *ptr++ = src[i];

  *ptr = '\0';
  return os;
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
bool is_valid (const char *it) {
    const char set[] = " {}()_,;->\n";
    bool form = false;
    int par = 0, brc = 0;

    while (*it) {
        //printf ("%c", *it);
        //if (par || brc) form = true;

        if (*it == '(') {

            char *sub = segment2 (it, ")") + 1;

          //  if (strlen (sub) > 0) {
                if (!param (sub)) return false;
          //  }
            par++;
        }

        if (*it == ')') par--;
        if (*it == '{') brc++;
        if (*it == '}') brc--;
        //if (!strchr (set, *it) && !isalnum (*it)) return false;
        it++;
    }

    return true;
  }
char *lambda (char *src) {

    const int size = strlen (src);

    if (size == 0) {
        return strdup ("(){}");
    }

    int i = 0, v = 0, nvr = 0;
    char *lmb = strstr (src, "->"), *it = src;
    char var[64];
    struct token vtable[10];

    while (*it != '\0') {
        if (*it == ',') nvr++;

        if (IS_VAR (*it)) {
            int pos = it - src;
            it = getvar (it, var);

            vtable[v++] = (struct token) {pos, strdup (var)};
        } else {
            it++;
        }
    }
    nvr++;

    for (int i = 0, j = nvr; i < nvr; i++, j++) {
        //printf ("%s %s\n", vtable[i].str, vtable[j].str);
    }
    char *os = malloc (32 * sizeof (char)), *ptr = os;

    i = 0;
    *ptr++ = '(';
    if (lmb != NULL) {
        int lim = lmb - src;
        for (; i < v && vtable[i].type < lim; i++) {
            ptr += sprintf (ptr,"%s,", vtable[i].str);
        }
        ptr--;
    }

    ptr += sprintf (ptr,"){");

    for (; i < v; i++) {
        ptr += sprintf (ptr,"%s;", vtable[i].str);
    }
    *ptr = '}';

    return os;
  }
char *lambda2 (char *expr) {
  const int size = strlen (expr);
  if (size == 0) return strdup ("(){}");
  char *lmb = strstr (expr, "->"), *it = expr;
  const int lim = lmb == NULL ? size : lmb - expr;
  int nvr = 0, nval = 0, r = 0;

  char buff[32];
  char *lhs[5], *rhs[5];

  rhs[0] = lhs[0] = NULL;

  while (it - expr < size) {                               // geting values

      if (IS_VAR (*it)) {
          int pos = it - expr;
          it = getvar (it, buff);
          if (pos < lim) lhs[nval] = strdup (buff);
          if (pos > lim) rhs[r++] = strdup (buff);
      }
      if (*it == ',') nval++;
      it++;
  }

  if (lmb) {                                  // catching errors
      if (lhs[0] == NULL) return NULL;
      if (r > nval + 1) return NULL;

      if (r == nval + 1) {
          for (int i = 0; i <= nval; i++) {
              if (strcmp (lhs[i], rhs[i]) != 0) return NULL;
          }
      }
  }

  printf ("%i \n", nval);
  char *os = malloc (64 * sizeof (char)), *ptr = os;
  *ptr++ = '(';

  if (lmb) {                                  // making param side
      for (int i = 0; i <= nval; i++) {
          ptr += sprintf (ptr ,"%s", lhs[i]);
          if (i < nval) *ptr++ = ',';
      }
  }

  ptr += sprintf (ptr, "){");

  for (int i = 0; i <= nval; i++) {         // making lambda side
      if (lmb) {
          if (rhs[i])
              ptr += sprintf (ptr ,"%s;", rhs[i]);
      } else {
          if (lhs[i])
              ptr += sprintf (ptr ,"%s;", lhs[i]);
      }
  }

  *ptr = '}';
  return os;
}
char *transpile2 (const char* source) {
    const int size = strlen (source);
    char *os = malloc (size * sizeof(char)), *ptr = os;

    char *it = source;
    char buff[64];
    bool running = true, brace = false;

    while (running) {

        if (*it == '_' || isalpha (*it)) {
            int i = 0;
            while (*it == '_' || isalnum (*it)) buff[i++] = *it++;
            buff[i] = '\0';

            if (*it == '(' || *it == '{') { // type = bufftion
                //printf ("%s(", buff);
                char *sub = transpile (it);
                it += strlen (sub);

                ptr += sprintf (ptr, "%s(%s)", buff, sub);
            } else { // type = variable
                ptr += sprintf (ptr, "%s", buff);
            }
        } else if (*it == '(') {
            int len = getsub (it);
            memcpy (buff, it + 1, len);
            buff[len] = '\0';

            *ptr++ = *it++;
        } else if (*it == ')') {

            *ptr++ = *it++;
        } else if (*it == '{') {
            if (*(ptr - 1) != ')') {
                ptr += sprintf (ptr, "()");
            }
            *ptr++ = *it++;
        } else if (*it == '}') {
            //printf ("[%c]\n", *it);
            *ptr++ = *it++;
        } else {
            it++;
        }

        if ((it - source) > size) running = false;

    }

    return os;
}
char *transpile3 (const char* source) {

    if (strcmp (source, "{}{}") == 0) return strdup ("(){}((){})");
    if (strcmp (source, "{}()") == 0) return strdup ("(){}()");

    const int size = strlen (source);
    char *expr = strdup (source), *it = expr;
    char *os = malloc (size * sizeof(char)), *ptr = os;
    int npam = 0;
    char buff[64] = {}, *pam[10];
    bool running = true, func = false, par = false, lam = false;

    while (running) {

        if (IS_VAR (*it)) {
            it = getvar (it, buff);
            char *curr = strdup (buff);

            if (!valid_var (curr)) return strdup ("");

            if (detect (it) == FUNC || detect (it) == LAMB) {
                ptr += sprintf (ptr, "%s(", curr);
                func = true;
            } else if (detect (it) == VAR){
                pam[npam++] = strdup (curr);
                //printf ("var : [%s] ", curr);
            } else {
                return strdup ("");
                //printf ("var : [%s] ", curr);
            }
        } else if (*it == '{') {
            lam = true;
            char *sub = segment2 (it, "}") + 1;
            it += (strlen (sub) + 2);
            //printf ("%s", it);
            sub = lambda3 (sub);
            if (sub == NULL) return strdup ("");
            if (detect (it) == FUNC) strcat (sub, "(");

            pam[npam++] = sub;
        } else if (*it == '(') {
            par = true;
            char *sub = segment2 (it, ")") + 1;
            //char *pch = strtok (it, ")") + 1;
            if (!param (sub)) return strdup ("");
            it++;
        } else if (*it == ')') {
            it++;
        } else {
            it++;
        }

        if ((it - expr) > size) running = false;
    }

    if (!func && par && !lam) return strdup ("");
    for (int i = 0; i < npam ; i++) {
        ptr += sprintf (ptr, "%s", pam[i]);

        if (i < npam - 1 && *(ptr - 1) != '(') {
            *ptr++ = ',';
        }
    }

    int pile = 0;
    for (int i = 0; i < strlen (os); i++) {

        if (os[i] == '(') pile++;
        if (os[i] == ')') pile--;
    }

    while (pile-->0) *ptr++ = ')';
    return os;
}

*/
