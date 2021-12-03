#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

char *cat (const char *s0, ...);              // cat all strings together, stop at argument value 0


#define IS_VAR(x) ((x) == '_' || isalnum(x))
enum {NONE, FUNC, VAR, LAMB};

extern char* strdup(const char*);

struct token {
    int type;
    char *str;
};

typedef struct list {
  void *data;
  struct list *next;
} list;

list *node (void *data);           // create one-element list
list *concat (list *l1, list *l2); // concatenate two lists, modifies l1

char *getvar (char *src, char *var) {
    int i = 0;
    while (IS_VAR(*src)) var[i++] = *src++;
    var[i] = '\0';
    return src;
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

int detect (char *it) {

    while (*it) {

        if (*it == '{') return LAMB; // function
        if (*it == '(') return FUNC; // function
        if (*it == ',') return VAR; // variable
        if (*it == ')') return VAR; // variable
        if (*it == '}') return VAR; // variable
        if (*it == '-') return VAR; // variable
        it++;
    }
    return NONE;
}

char *transpile (const char* source) {

    //if (is_valid (source) == false) return strdup ("");
    const int size = strlen (source);
    char *expr = strdup (source), *it = expr;
    char *os = malloc (size * sizeof(char)), *ptr = os;
    char buff[64], *pam[10];
    bool running = true;
    int nvar = 0, npam = 0;

    printf ("\n");

    while (running) {

        if (*it == ',') nvar++;

        if (IS_VAR(*it)) {
            it = getvar (it, buff);
            char *curr = strdup (buff);

            if (detect (it) == FUNC) {
                //printf ("fnc : [%s] ", curr);
            } else if (detect (it) == LAMB){
                //printf ("fnc : [%s] ", curr);
            } else if (detect (it) == VAR){
                pam[npam++] = strdup (curr);
                //printf ("var : [%s] ", curr);
            }

        } else if (isdigit (*it) ) {
            char *var = buff;
            while (isdigit (*it)) *var++ = *it++;
        }

        else if (*it == '(') {
            char *sub = strtok (it, ")") + 1;
            it += strlen (sub) + 1;
            //printf ("npam : [(%s)] ", sub);
            transpile (sub);
        }

        else if (*it == '{') {
            char *sub = strtok (it, "}") + 1;
            it += (strlen (sub) + 1);
            lambda (sub);
            pam[npam++] = sub;
            //printf ("lam : [{%s}] ", sub);
            transpile (sub);
        } else {

            it++;
        }

        if ((it - expr) > size) running = false;
    }
  //  printf ("%i %i ", npam, nvar + 1);
    if (npam != nvar + 1) return NULL;

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

  fromTo ("{}{}","(){}((){})");         // error
  fromTo ("fun(a, b)" , "fun(a,b)");
  fromTo ("{a -> a}(1)" , "(a){a;}(1)");
  fromTo ("{a->a}(233)", "(a){a;}(233)");
  fromTo ("{a, b -> a b} (1, 2)","(a,b){a;b;}(1,2)");
  fromTo ("invoke  (       a    ,   b   )", "invoke(a,b)");
  fromTo ("invoke(a, b)", "invoke(a,b)");

  fromTo ("f { a }","f((){a;})");
  fromTo ("fun(a) {}" , "fun(a,(){})");
  fromTo ("invoke  (       a    ,   b   ) { } ", "invoke(a,b,(){})");
  fromTo ("fun(a)" , "fun(a)");

  fromTo ("call(\n){}", "call((){})");
  fromTo ("f(x){a->}", "f(x,(a){})");
  fromTo ("f(a,b){a->a}", "f(a,b,(a){a;})");


    fromTo ("fun()", "fun()");
    fromTo ("call()", "call()");
    fromTo ("{}()" , "(){}()");
    fromTo ("fun {}" , "fun((){})");
    fromTo ("fun(a, {})" , "fun(a,(){})");
    fromTo ("fun {a -> a}" , "fun((a){a;})");
    fromTo ("fun { a, b -> a b }" , "fun((a,b){a;b;})");
    fromTo ("f { a -> }","f((a){})");
    shouldFail ("%^&*(");
    shouldFail ("x9x92xb29xub29bx120()!(");

    fromTo ("call({})", "call((){})");

    fromTo ("f({a->})", "f((a){})");
    fromTo ("f({a->a})", "f((a){a;})");
    /*

    */
    fromTo ("f { a }","f((){a;})");
    fromTo ("run{a}", "run((){a;})");

    fromTo ("{}()", "(){}()");
}
void Test2 () {
    /*

Transpile from "invoke({},{})" to "invoke((){},(){})" failed, result was "invoke((){}(){})"

Transpile from "run{->a}" to "" failed, result was "run(){a;})"
Transpile from "i(,{})" to "" failed, result was "i((){})"
Transpile from "f(->)" to "" failed, result was "f()"
Transpile from "f({1a->a})" to "" failed, result was "f((1a){a;})"
Transpile from "f(a,)" to "" failed, result was "f(a)"
Transpile from "a b c" to "" failed, result was "abc)"
Transpile from "f (,a)" to "" failed, result was "f(a)"
Transpile from "f( a v)" to "" failed, result was "f(a,v)"

Transpile from "gg     (  a  )  " to "gg(a)" failed, result was "gg(a,)"
    */
}
/////////////////////////////////////////////////////////////////

char *parenth (char *expr) {

    int size = strlen (expr);
    int iv = 0, jv = 0;
    char *var[5];
    char *it = expr + 1;
    struct token os = {true, strdup (expr)};
    if (strstr (expr, "->") != NULL) return "";
    const char set[] = " ,\0";

    while (it - expr < size - 1) {
        if (*it == ',') {
            iv++;
            it++;
        } else if (*it != ' ') {
          char buff[64], *ptr = buff;
          while (!strchr (set, *it)){
              *ptr++ = *it;
              it++;
          }
          jv++;
          *ptr = '\0';
        } else {
            it++;
        }
    }

    iv++;
    printf ("%i %i", iv,jv);

    if (iv != jv) return "";
    return true;
}
bool is_valid (const char *it) {
    const char set[] = " {}()_,;->\n";
    int par = 0, brc = 0;

    while (*it) {

        if (*it == '(') par++;
        if (*it == ')') par--;
        if (*it == '{') brc++;
        if (*it == '}') brc--;
        if (!strchr (set, *it) && !isalnum (*it)) return false;

        it++;
    }

    if (brc || par) return false;

    return true;
}

char *lambda2 (char *expr) {
  const int size = strlen (expr);
  if (size == 0) return strdup ("(){}");

  char *lmb = strstr (expr, "->"), *it = expr;
  const int lim = lmb == NULL ? size : lmb - expr;
  int nvr = 0, l = 0, r = 0;

  char buff[32];
  char *lhs[5], *rhs[5];

  while (*it) {
      if (IS_VAR (*it)) {
          int pos = it - expr;
          it = getvar (it, buff);

          if (pos < lim) lhs[l] = strdup (buff);
          if (pos > lim) rhs[r++] = strdup (buff);
      }
      if (*it == ',') l++;
      it++;
  }
  //l++;
  if (lmb && r)
      for (int i = 0; i < l; i++) {
          printf ("%s %s\n", lhs[i], rhs[i]);
      }
  char *os = malloc (64 * sizeof (char)), *ptr = os;
  *ptr++ = '(';

  if (lmb) {
      for (int i = 0; i <= l; i++) {
          ptr += sprintf (ptr ,"%s", lhs[i]);
          if (i < l) *ptr++ = ',';
      }
  }

  ptr += sprintf (ptr, "){");

  for (int i = 0; i <= l; i++) {
      if (lmb) {

          if (rhs[i])
          ptr += sprintf (ptr ,"%s;", rhs[i]);
      } else
          ptr += sprintf (ptr ,"%s;", lhs[i]);
  }

  *ptr = '}';

  return os;
}
int main () {

    //fromTo (" f ({ a })","f( (){a;} )");
    //fromTo ("f( {a->} )", "f( (a){} )");
    //fromTo ("fun { a, b -> a b }" , "fun((a,b){a;b;})");

  //char *res = transpile ("invoke({})");

  char *expr = strdup ("{-> a}");
  char *sub = strtok (expr, "}") + 1;

  printf ("%s", lambda2 (sub));

  /*
  char *res;
  sprintf (res, "{%s}", sub);

  printf ("%i", 4 * 12);
  //printf ("%s", res);
  */
  //Test ();
  /*
    printf ("%s", res);
    */

  //shouldFail ("x9x92xb29xub29bx120()!(");
    //Test();

    //char *expr =  strdup ("fun {a -> a}");


    /*
    /*
    char *ret = lambda (sub);
    */
}

////////////////////////////////////////////////////////////////////////
////////////////////////    queue implementation   ///////////////////////
typedef struct _queue {
    list *frt, *bck;
} queue;

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
    int size = strlen (it);

    do {
        if (*it == '(') pile++;
        if (*it == ')') pile--;
        if (pile == 0) return index;
        index++;
    } while (*it++ != '\0');

    return size;
  }
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
/*
*/
queue *init () {
  queue *new = malloc (sizeof(queue));
  new->frt = new->bck = NULL;
  return new;
}
/////////////////////////////////////////////////////////////////////////
char *transpile2 (const char* source) {

    const int size = strlen (source);
    char *it = source;
    char *os = malloc (size * sizeof(char)), *ptr = os;
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

    //if (is_valid (source) == false) return strdup ("");
    const int size = strlen (source);
    char *expr = strdup (source), *it = expr;
    char *os = malloc (size * sizeof(char)), *ptr = os;
    char buff[64];
    bool running = true;

    while (running) {

        if (*it == '_' || isalnum (*it)) {
            it = getvar (it, buff);
            int type = detect (it);

            ptr += sprintf (ptr, "%s", buff);
            if (type == LAMB) {
                *ptr++ = '(';
            }
            if (type == VAR) {
                *ptr++ = ',';
            }

        }

        else if (*it == '(') {
            *ptr++ = *it++;
        }

        else if (*it == ')') {

            if (*(ptr - 1) == ',' && *(it + 1) == '\0')
                ptr--;

            it++;
        }
        else if (*it == '{') {
            char *ret;
            if (*(it + 1) == '}') {
                it++;
                ret = strdup ("(){}");
            } else {
                char *sub = strtok (it + 1, "}");
                ret = lambda (sub);
                it += (strlen (sub) + 1);
            }
            ptr += sprintf (ptr, "%s", ret);
        } else {
            it++;
        }

        if ((it - expr) > size) running = false;
    }

    *ptr = ')';

    //printf ("%s", os);
    return os;
}
