#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

extern char* strdup (const char*);

char **tokenize (const char* src) {

    int size = strlen (src), index = 0;
    char *expr = strdup (src);
    char **tok = malloc ((size + 1) * sizeof (char*)), *it = expr;

    while (size > (it - expr)) {
        while (*it == ' ') it++;
        char buffer[16] = {}, *ptr = buffer;

        if (isalnum (*it)) {
            while (isalnum (*it)) *ptr++ = *it++;

        } else {
            if (*it == '-' && *(it + 1) == '>') {
                *ptr++ = *it++;
            }

            if (ispunct (*it)) *ptr++ = *it;
            it++;
        }

        if (ptr - buffer) tok[index++] = strdup (buffer);
    }

    tok[index] = NULL;

    return tok;
}
bool isvalid2 (char **code) {

    int index = 0;
    int pam = 0, sep = 0;
    int paren = 0, brace = 0;
    char id, last;

    if (code[index] && code[index][0] == '(') return false; // function parameters without function name

    while (code[index]) {
        id = code[index][0], last = index > 0 ? code[index - 1][0] : ' ';

        if (isdigit (id)) {      // expression begining with digit
            for (int i = 1; code[index][i]; i++) {
                if (isalpha (code[index][i])) return false;
            }
        }
        else if (id == '(') paren++;
        else if (id == ')') paren--;
        else if (id == '}') brace++;
        else if (id == '{') {
            brace--;
            if (paren) pam++;
        }
        else if (id == ',') {
            if (last == '(' || code[index][0] == ')') return false;
            //if (code[index + 1] && code[index + 1][0] == ')') return false;
            if (paren) sep++;
        }
        else if (id == '-') {
            if (last == '(' || last == '{') return false;
        }
        else {
            if (paren && !brace) pam++;
        }
        index++;
    }

    last = index > 0 ? code[index - 1][0] : ' ';

    if (last != ')' && last != '}') return false;
    if (paren || brace) return false; // invalid parenthesis or brace number
    if (pam - sep >= 2) return false;
    // printf ("%d %d ", pam , sep);
    return true;
}
char *lambda5 (char **expr, int *it) {

      int size = *it + 1, mid = size;
      char buffer[64] = {'('}, *ptr = buffer + 1;

      if (expr[*it + 1][0] == '}') {
          *it = size; return strdup ("(){}");
      }

      while (expr[size][0] != '}') {
          if (strcmp (expr[size], "->") == 0) mid = size;
          size++;
      }

      int sep = 0, param = 0;

      for (int i = *it + 1; i < size; i++) {
          if (expr[i][0] == ',') {
              sep++;
          } else {
              if (i == mid) {
                  sep = 0;
                  ptr += sprintf (ptr, "){");
              }

              if (i < mid) {
                  if (sep != param++) return NULL;
                  ptr += sprintf (ptr, "%s", expr[i]);
                  if (i < mid - 1) *ptr++ = ',';
              } else if (expr[i][0] != '-') {
                  if (sep) return NULL;
                  ptr += sprintf (ptr, "%s", expr[i]);
                  if (i < size ) *ptr++ = ';';
              }
          }
      }

      sprintf (ptr, "}");

      *it = size;
      return strdup (buffer);
}
char *transpile (const char* source) {

    // if (strcmp (source, "{}{}{}") == 0) return strdup ("");
    int index = 0;
    char **expre = tokenize (source), **code = expre;
    char *os = malloc (64 * sizeof (char)), *ptr = os;
    if (!isvalid2 (code)) return strdup ("");

    while (code[index]) {

        char id = code[index][0];

        if (id == '(') {
            char *sub[64];
            int param = 0;
            index++;

            while (code[index][0] != ')') {

                if (code[index][0] != ',') {

                    if (code[index][0] == '{') {
                        sub[param++] = lambda5 (code, &index);
                    } else {
                        sub[param++] = strdup (code[index]);
                    }

                    if (code[index][0] == ')') break;
                }
                index++;
            }

            *ptr++ = '(';
            for (int i = 0; i < param; i++) {
              ptr += sprintf (ptr, "%s", sub[i]);
              if (i < param - 1) *ptr++ = ',';
            }

        } else if (id == '{') {
            if (index >= 2) {
                if (code[index - 1][0] == ')' && code[index - 2][0] != '(') {
                    *ptr++ = ',';
                }
                if (code[index - 1][0] == '}') {
                    *ptr++ = '(';
                }
            }
            char *sub = lambda5 (code, &index);
            ptr += sprintf (ptr, "%s", sub);
        } else {
            ptr += sprintf (ptr, "%s", code[index]);
            if (code[index + 1] && code[index + 1][0] != '(') *ptr++ = '(';
        }

        index++;
    }

    int cnt = 0;
    for (int i = 0; os[i] != '\0'; i++) {
        if (os[i] == '(') cnt++;
        if (os[i] == ')') cnt--;
    }

    if (cnt > 0) sprintf (ptr, ")");

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
      printf ("test %i [%s] :: got : [%s] expected : [%s]\n",idx, tested, res, expect);
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

  fromTo ("{a->a}(cde,y,z){x y d -> stuff}","");
  fromTo ("{a->a}(cde,y,z){x y,d -> stuff}","");
  fromTo ("{a->a}(cde,y,z){x,    y,d ->stuff,g,h}","");
  fromTo ("{a}(cde,y,z){x,y,d jj}","");
  fromTo ("{a o , p}(cde,y,z){x,y,d,jj}","");
  /*
    */

  }
/////////////////////////////////////////////////////////////////

int main () {

  //Test ();
//Transpile from "{a,b,c->a  b c }  (233 , 666, 999)" to "(a,b,c){a;b;c;}(233,666,999)" failed, result was ""

  const char source[32] = {32};


  //char **expr = tokenize (source), **code = expr;
  char *res = transpile ("f({a b -> c})");

  printf ("[%s]\n", res);

  /*
  "{f({_->})"
  "{f({asWeCan->})"
  "{f({a->)"
  "{f(a->})"
  "{"

  */

  int index = 12;



  /*
  :: transpile({32});


 :: {10,10};
   call    (     )      :: {32,32,32,99,97,108,108,32,32,32,32,40,32,32,32,32,32,41,32,32,32,32,32};

  call


(


 )

    :: {32,10,32,32,99,97,108,108,10,32,32,10,32,32,10,40,10,32,32,32,10,32,10,32,41,10,32,32,10,32,32,32};
*/


  printf ("\nfinish");

}
