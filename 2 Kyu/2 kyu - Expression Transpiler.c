#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

extern char* strdup(const char*);
void display_code (const char **code) {
    for (int i = 0; code[i]; i++) {
        printf ("[%s]" , code[i]);
    }
    printf ("\n");
  }

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
    // char **error = malloc (sizeof(char*));
    // *error = NULL;
    int par = 0, brace = 0;

    while (size > (it - expr)) {
        while (*it == ' ') it++;
        char buffer[16] = {}, *ptr = buffer;
        // check for validity

        if (*it == '(') par++;
        if (*it == ')') par--;
        if (*it == '{') brace++;
        if (*it == '}') brace--;

        if (isdigit (*it)) {
            index = 0;
            break;
        }

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

    if (par || brace) index = 0; // invalid braces or parenthesis

    tok[index] = NULL;

    return tok;
}

bool isvalid (const char *source) {

    int par = 0, brace = 0;
    char c;

    while (c = *source++) {

        if (c == '(') par++;
        if (c == ')') par--;
        if (c == '{') brace++;
        if (c == '}') brace--;
        if (isdigit (c)) return false;
    }

    if (par || brace) return false;
    return true;
}
char *transpile (const char* source) {

    if (!isvalid(source)) return strdup ("");
    // if (strcmp (source, "{}{}") == 0) return strdup ("(){}((){})");
    char **expre = tokenize (source), **code = expre;
    char *os = malloc (64 * sizeof (char)), *ptr = os;

    int size = 0;

    while (*code) {

        char id = *code[0];

        if (id == '(') {
            char *sub[64];
            code++;

            while (strcmp (*code, ")")) {

                if (strcmp (*code, "{") == 0) {
                    code = lambda (code + 1, &sub[size++]) + 1;
                } else if (strcmp (*code, ",") /* && strcmp (*code, "}") */) {
                    sub[size++] = strdup (*code);
                } else {

                }

                if (strcmp (*code, ")") == 0) break;
                code++;
            }

            *ptr++ = '(';
            for (int i = 0; i < size; i++) {
              ptr += sprintf (ptr, "%s", sub[i]);
              if (i < size - 1) *ptr++ = ',';
            }
            //*ptr++ = ')';
        } else if (id == '{') {
            char *sub;
            //printf ("%d %s\n", index, expre[index]);
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
  /*
  fromTo ("{}{}","(){}((){})");
    */

  }
/////////////////////////////////////////////////////////////////
int main () {

  //Test ();

  fromTo ("i(,{})" , "");
  fromTo ("f(a,)" , "");
  fromTo ("f (,a)" , "");
  fromTo ("f( a v)" , "");

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
