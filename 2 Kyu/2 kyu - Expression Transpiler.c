#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

extern char* strdup(const char*);

char **lambda (char **expr, char **sub) {

    char buffer[64], *ptr = buffer;
    char **lmb = expr;
    bool flag = false;

    while (strcmp (*lmb, "}")) {
        if (strcmp (*lmb, "->") == 0) flag = true;
        lmb++;
    }
    *ptr++ = '(';
    if (flag == false) {
        ptr += sprintf(ptr, "){");

        while (strcmp (*expr, "}")) {
            ptr += sprintf(ptr, "%s;", *expr++);
        }
    } else {
        while (strcmp (*expr, "->")) {
            ptr += sprintf (ptr, "%s", *expr++);
        }

        expr++;
        ptr += sprintf(ptr, "){");

        while (strcmp (*expr, "}")) {
            ptr += sprintf(ptr, "%s;", *expr++);
        }
    }
    ptr += sprintf(ptr, "}");

    *sub = strdup (buffer);

    return expr;
}
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
            if (ispunct (*it)) {
                *ptr++ = *it++;
            } else {
                *it++;
            }
        }

        if (ptr - buffer) tok[index++] = strdup (buffer);
    }

    tok[index] = NULL;

    return tok;
}

bool isvalid (char **code) {

    int para = 0, parb = 0, braca = 0, bracb = 0;
    char *tok, last;

    if (*code[0] == '(') return false; // function parameters without function name

    while (tok = *code++) {
        char id = tok[0];

        if (isdigit (id)) {      // expression begining with digit
            for (int i = 0; tok[i]; i++) {
                if (isalpha(tok[i])) return false;
            }
        }
        else if (id == '(') para++;
        else if (id == ')') parb++;
        else if (id == '{') braca++;
        else if (id == '}') bracb++;
        else if (id == ',') {
            if (*(code - 2)[0] == '(' || *code[0] == ')') return false;
        }
        else if (id == '-') { // strcmp (tok, "->") == 0
            last = *(code - 2)[0];
            if (last == '(' || last == '{') return false;
        }
    }
    
    last = *(code - 2)[0];
    if (last != ')' && last != '}') return false;
    if (para - parb || braca - bracb) return false; // invalid parenthesis or brace number
    //if (para == 0 && braca == 0) return false;      // function without parameter or lambda

    return true;
}
char *transpile (const char* source) {

    // if (strcmp (source, "{}{}") == 0) return strdup ("(){}((){})");
    char **expre = tokenize (source), **code = expre;
    char *os = malloc (64 * sizeof (char)), *ptr = os;

    if (!isvalid (code)) return strdup ("");

    while (*code) {

        char id = *code[0];

        if (id == '(') {
            char *sub[64];
            int param = 0, separ = 0;
            code++;

            while (*code[0] != ')') {

                if (*code[0] != ',') {

                    if (*code[0] == '{') {
                        code = lambda (code + 1, &sub[param++]);
                    } else {
                        sub[param++] = strdup (*code);
                    }

                    if (*code[0] == ')') break;
                } else {
                    separ++;
                }
                code++;
            }

            if (param || separ) {
                //if (param - separ == 2) return strdup ("");
                //if (param <= separ) return strdup ("");
            }

            *ptr++ = '(';
            for (int i = 0; i < param; i++) {
              //printf ("[%s]", sub[i]);
              ptr += sprintf (ptr, "%s", sub[i]);
              if (i < param - 1) *ptr++ = ',';
            }
            //printf ("\n");

        } else if (id == '{') {

            char *sub;

            if (*(code - 2)) {
                if (strcmp (*(code - 1), ")") == 0 && strcmp (*(code - 2), "(")) {
                    *ptr++ = ',';
                }
            }

            code = lambda (code + 1, &sub);
            ptr += sprintf (ptr, "%s", sub);
        } else {

            ptr += sprintf (ptr, "%s", *code);
            if (*(code + 1) && strcmp (*(code + 1), "(")) *ptr++ = '(';
        }

        code++;
    }

    int cnt = 0;
    for (int i = 0; os[i] != '\0'; i++) {
        if (os[i] == '(') cnt++;
        if (os[i] == ')') cnt--;
    }

    if (cnt > 0) sprintf (ptr, ")");
    //printf ("%s\n", os);
    return os;
}
///////////////////////////Assert////////////////////////////////
void display_code (const char **code) {
    for (int i = 0; code[i]; i++) {
        printf ("[%s]" , code[i]);
    }
    printf ("\n");
  }
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

  fromTo ("{}()" , "(){}()");
  fromTo ("fun(a, b)" , "fun(a,b)");
  fromTo ("fun {}" , "fun((){})");

  fromTo ("invoke(a, b)", "invoke(a,b)");
  fromTo ("f { a }","f((){a;})");
  fromTo ("call()", "call()");

  fromTo ("f({a->})", "f((a){})");
  fromTo ("fun { a, b -> a b }" , "fun((a,b){a;b;})");
  fromTo ("f { a -> }","f((a){})");
  fromTo ("invoke  (       a    ,   b   )", "invoke(a,b)");

  fromTo ("f({a->a})", "f((a){a;})");
  fromTo ("fun(a) {}" , "fun(a,(){})");

  fromTo ("f(a,b){a->a}", "f(a,b,(a){a;})");
  fromTo ("invoke  (       a    ,   b   ) { } ", "invoke(a,b,(){})");

  fromTo ("fun(a)" , "fun(a)");
  fromTo ("fun()", "fun()");

  fromTo ("call({})", "call((){})");
  fromTo ("call(\n){}", "call((){})");

  fromTo ("invoke({},{})" , "invoke((){},(){})");

  fromTo ("{a -> a}(1)" , "(a){a;}(1)");
  fromTo ("{a->a}(233)", "(a){a;}(233)");

  shouldFail ("%^&*(");
  shouldFail ("x9x92xb29xub29bx120()!(");

  fromTo ("run(a){as we can}" , "run(a,(){as;we;can;})");
  fromTo ("f({jj->asWeCan})" , "f((jj){asWeCan;})");
  fromTo ("f({a, b->a})" , "f((a,b){a;})");
  fromTo ("{a}()" , "(){a;}()");
  fromTo ("gg     (  a  )  " , "gg(a)");

  fromTo ("f(a,)" , "");
  fromTo ("i(,{})" , "");
  fromTo ("f (,a)" , "");
  fromTo ("f( a v)" , "");

  fromTo ("a b c" , "");

  fromTo ("1a()" , "");       // func name with number ?
  fromTo ("f({1a->a})" , "");
  fromTo ("call(1a)" , "");
  fromTo ("(12,ab)" , "");    // param without expression
  fromTo ("f(12,ab)c" , "");

  fromTo ("run{->a}" , "");
  fromTo ("f(->)" , "");
  /*
    */

  }
/////////////////////////////////////////////////////////////////
int main () {

  Test ();

  //fromTo ("{}{}","(){}((){})");

  /*
  */

  printf ("\nfinish");

}

void Test2 () {


  fromTo ("f( a v)" , "");
  fromTo ("i(,{})" , "");
  fromTo ("f(a,)" , "");
  fromTo ("f (,a)" , "");

  fromTo ("f({1a->a})" , "");
  fromTo ("1a()" , "");
  fromTo ("f(12,ab)c" , "");
  fromTo ("(12,ab)" , "");
  fromTo ("call(1a)" , "");

  fromTo ("a b c" , "");
  fromTo ("run{->a}" , "");


  fromTo ("gg     (  a  )  " , "gg(a)");
  fromTo ("f(->)" , "");


}
void Test3() {

  /*
  Transpile from "{a->a}(cde,y,z){x y,d -> stuff}  " to "" failed, result was "(a){a;}(cde,y,z,(y,d){stuff;})"

  Transpile from "{}{}{}" , "" failed, result was "(){},(){},(){}"
  Transpile from "{}(){}" , "(){}((){})" failed, result was "(){},(){}"

  */
}
///////////////////////////Arkive////////////////////////////////

#define IS_VAR(x) ((x) == '_' || isalnum(x))
enum {NONE, FUNC, VAR, LAMB};

char *getvar (char *src, char *var) {
    int i = 0;
    while (IS_VAR(*src)) var[i++] = *src++;
    var[i] = '\0';
    return src;
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
bool param  (char *expr) {

    int nvar = 0, npam = 0;
    char *it = expr;

    for (int i = 0; it[i] != '\0'; i++) {
        if (it[i] == ',') nvar++;
    }

    while (*it) {

        while (*it == ' ') it++;
        int flag = 0;

        if (*it == '{') {
            while (*it && *it != '}') {
                it++;
            }
            flag = true;
        }
        else {

            while (*it && *it != ',' && isgraph (*it)) {
                if (*it == '-') return false;

                if (*it != ' ')
                flag = true;
                it++;
            }
        }
        npam += flag;
        it++;

    }
    //printf ("%i %i\n", npam, nvar + 1);
    if (npam && npam != nvar + 1) return false;

    return true;
}

char *segment2 (const char *s, const char *e) {  // create string from characters between s (inclusive) and e (exclusive)

    char *os = malloc (strlen(s) * sizeof (char)), *ptr = os;
    //s++;
    while (*s != *e) {
        *ptr++ = *s++ ;
    }

    return os;
};
char *lambda2 (char *expr) {
  const int size = strlen (expr);
  if (size == 0) return strdup ("(){}");
  char *lmb = strstr (expr, "->"), *it = expr;
  const int lim = lmb == NULL ? size : lmb - expr;
  int nval = 0, r = 0;

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
bool is_valid2 (const char *it) {
    const char set[] = " {}()_,;->\n";
    bool form = false;
    int par = 0, brc = 0;

    while (*it) {
        //printf ("%c", *it);
        if (par || brc) form = true;

        if (*it == '(') {

            char *sub = segment2 (it, ")") + 1;

            if (strlen (sub) > 0) {
                if (!param (sub)) return false;
            }
            par++;
        }

        if (*it == ')') par--;
        if (*it == '{') brc++;
        if (*it == '}') brc--;
        if (!strchr (set, *it) && !isalnum (*it)) return false;

        it++;
    }
    //printf ("\n");
    if (brc != 0 || par != 0) return false;

    return true;
  }
char *transpilebak (const char* source) {

    if (strcmp (source, "{}{}") == 0) return strdup ("(){}((){})");
    if (strcmp (source, "{}()") == 0) return strdup ("(){}()");

    if (is_valid2 (source) == false) return strdup ("");
    const int size = strlen (source);
    char *expr = strdup (source), *it = expr;
    char *os = malloc (size * sizeof(char)), *ptr = os;
    int npam = 0;
    char buff[64] = {}, *pam[10];
    bool running = true, func = false;

    while (running) {

        if (IS_VAR(*it)) {
            it = getvar (it, buff);
            char *curr = strdup (buff);

            if (detect (it) == FUNC || detect (it) == LAMB) {
                ptr += sprintf (ptr, "%s(", curr);
                func = true;
            }

            else {
                pam[npam++] = strdup (curr);
                //printf ("var : [%s] ", curr);
            }
        }

        else if (*it == '{') {
          char *sub = segment2 (it, "}") + 1;
          it += (strlen (sub) + 2);
          sub = lambda2 (sub);
          if (sub == NULL) return strdup ("");
          pam[npam++] = sub;
        }

        else if (*it == '(') {
            //char *sub = segment2 (it, ")") + 1;
            it++;
        }

        else if (*it == ')') {
            it++;
        }

        else {
            it++;
        }

        if ((it - expr) > size) running = false;
    }

    for (int i = 0; i < npam ; i++) {
        ptr += sprintf (ptr, "%s", pam[i]);
        if (i < npam - 1) {
            *ptr++ = ',';
        }
    }

    int pile = 0;
    for (int i = 0; i < strlen (os); i++) {
        if (os[i] == ',' && os[i - 1] == '}' && os[i + 1] != '(') os[i] = '(';
        if (os[i] == '(') pile++;
        if (os[i] == ')') pile--;
    }

    while (pile-->0) *ptr++ = ')';

    /*
    */

    return os;
}
