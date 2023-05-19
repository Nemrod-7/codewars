#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <regex>

using namespace std;

vector<string> tokenize (string src) {
    regex tok ("->|_?\\w+|[{}();,]");
    sregex_token_iterator it (src.begin(), src.end(), tok);
    return vector<string> (it, sregex_token_iterator());
}

bool isvalid3 (const string expr) {

    // a() or {}() or {}{} or a(){} or {}(){}
    if (regex_search (expr, regex("^[\\s\\n]*(\\w+|(\\{[^\\{\\}]*\\}))[\\s\\n]*(\\([^\\(\\)]*\\))?[\\s\\n]*(\\{[^\\{\\}]*\\})?[\\s\\n]*$")) == false) return false ;
    if (regex_search (expr, regex("^[\\s\\n]*\\{[^\\{\\}]*\\}[\\s\\n]*$"))) return false;   // rejects {...} :  single lambda

    if (regex_search (expr, regex("^\\s*\\(|[^\\)\\}\\s\\n]$|^[\\s\\n]*$"))) return false;  // the expression begins with '(' or ends with any char exept '}' or ')'
    if (regex_search (expr, regex("([^\\w]|^)[0-9]+[A-z]+|[^,_\\w\\{\\}\\s\\n->]"))) return false;  // bad name : 1a or bad character

    if (regex_search (expr, regex("->[^\\}]*,[^\\}]*\\}"))) return false;              // rejects  -> x , y }
    if (regex_search (expr, regex("\\{[^\\}]$|^[^\\{]*\\}|^[^\\(]*\\)|\\([^\\)]$"))) return false;

    if (regex_search (expr, regex("[\\{\\(]\\s*[,-]|,\\s*[\\}\\)\\-]"))) return false;       // rejects (-> or {-> or (, or {,
    if (regex_search (expr, regex("\\{[^\\{]*\\w+\\s+\\w+[^\\}]*->"))) return false;      // rejects { x y ->

    if (regex_search (expr, regex("\\{[^>\\(]*\\w+[\\s\\n]*,[\\s\\n]*\\w+[^\\}-]*\\}|,\\s+,"))) return false;   // rejects {a,b ,c} or , ,
    if (regex_search (expr, regex("\\([^\\{]*(\\w+|(\\{[^\\{\\}]*\\}))[\\s\\n]+(\\w+|(\\{[^\\{\\}]*\\}))[^\\}]*\\)"))) return false;  // rejects ( a b c )

    return true;
}
string lambda (vector<string>::iterator &end) {

    end++;
    if (*end == "}") return "(){}";

    string os ("(");
    vector<string>::iterator start = end, mid = start;

    while (*end != "}") {
        if (*end == "->") mid = end;
        end++;
    }

    for (auto it = start; it != end; it++) {
        if (*it == ",") continue;
        if (it == mid) {
            os += "){";
        }
        if (it - start < mid - start) {
            os += *it;
            if (it - start < mid - start - 1) os += ",";

        } else if (*it != "->") {
            os += *it + ";";
        }
    }

    return os + "}";
}

const char *transpile (const char* source) {

    string expr = source, os;
    if (!isvalid3(expr)) return strdup("");

    vector<string> code = tokenize (expr);
    vector<string>::iterator it = code.begin();

    while (it != code.end()) {

        string tok = *it;

        if (tok == "(") {

            do {
                os += (*it == "{") ? lambda (it) : *it;
                it++;
            } while (*it !=  ")");

        } else if (tok == "{") {
            if (it - code.begin() >= 2) {
                if (*(it - 1) == ")" && *(it - 2) != "(") {
                    os += ',';
                }
                if (*(it - 1) == "}") {
                    os += '(';
                }
            }
            os += lambda (it);
        } else {
            os += tok;
            if (*(it + 1) != "(") os += "(";
        }

        it++;
    }

    int cnt = 0;
    for (auto &it : os) {
        if (it == '(') cnt++;
        if (it == ')') cnt--;
    }

    if (cnt > 0) os += ")";

    return strdup(os.c_str());
}

void fromTo (const char *tested, const char *expect) {

  const char *res = transpile (tested);
  static int idx;

  if (strcmp (res, expect) != 0) {
      printf ("test %i [%s] :: got : [%s] expected : [%s]\n",idx, tested, res, expect);
  }

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

  fromTo ("run(a){as we can}" , "run(a,(){as;we;can;})");
  fromTo ("f({jj->asWeCan})" , "f((jj){asWeCan;})");
  fromTo ("f({a, b->a})" , "f((a,b){a;})");
  fromTo ("{a}()" , "(){a;}()");
  fromTo ("gg     (  a  )  " , "gg(a)");

  shouldFail ("%^&*(");
  shouldFail ("x9x92xb29xub29bx120()!(");

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

int main () {

  Test();

  shouldFail ("f({p,t,})");
  shouldFail ("f({a b -> c})");

  cout << "finish";

/*

fun() => fun()
fun(a) => fun(a)
fun(a, b) => fun(a,b)

{}() => (){}()
fun {} => fun((){})

fun(a, {}) => fun(a,(){})
fun(a) {} => fun(a,(){})
fun {a -> a} => fun((a){a;})

{a -> a}(1) => (a){a;}(1)
fun { a, b -> a b } => fun((a,b){a;b;})
{a, b -> a b} (1, 2) => (a,b){a;b;}(1,2)
f { a } => f((){a;})
f { a -> } => f((a){})

{}{} => (){}((){})
*/

}
