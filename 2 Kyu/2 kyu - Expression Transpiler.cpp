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

    // vector<string> badexp {"[0-9]+[A-z]+", "[^\\)\\}\\s\\n]$|^\\s*\\(", "[\\(|\\{]\\s*->", "\\([\\s\\n]*,|,[\\s\\n]*\\)|\\(.*\\w+\\s+\\w+.*\\)", "->[^\\}]*,[^\\}]*\\}", "\\{([^>]*\\w+,\\w+.*\\}|.*\\w+\\s+\\w+.*(->))"};
    if (regex_search (expr, regex("[0-9]+[A-z]+"))) return false;    // parameter or function name with mixed digit and alpha characters
    if (regex_search (expr, regex("[^\\)\\}\\s\\n]$|^\\s*\\("))) return false;   // the epression begins with '(' or ends with any character except ')' or '}' or ' ' or '\n'

    if (regex_search (expr, regex("[\\(\\{]\\s*->"))) return false;
    if (regex_search (expr, regex("\\([\\s\\n]*,|,[\\s\\n]*\\)|\\(.*\\w+\\s+\\w+.*\\)"))) return false;  // invalid argument

    if (regex_search (expr, regex("->[^\\}]*,[^\\}]*\\}"))) return false;
    if (regex_search (expr, regex("\\{([^>]*\\w+,\\w+.*\\}|.*\\w+\\s+\\w+.*->)"))) return false;

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

  // expre : [A-z]+|\\{.*?\\}
  // param : (\\(.*?\\))?

  // lmbda : \\{    (->)?  \\}
  // \\{
  //  [A-z]+,?
  //   (->)?
  //
  // \\}
  // shouldFail ("{a,b->c}");

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

vector<string> tokenize2 (string src) {
    string::iterator it = src.begin();
    vector<string> tok;

    while (it < src.end()) {
        while (*it == ' ') it++;
        string buffer;

        if (isalnum (*it)) {
            while (isalnum (*it)) buffer += *it++;

        } else {
            if (*it == '-' && *(it + 1) == '>')
                buffer += *it++;

            if (ispunct (*it)) buffer += *it;
            it++;
        }

        if (buffer.size()) tok.push_back (buffer);
    }

    return tok;
}
bool isvalid (const vector<string> &code) {

    size_t index = 0, size = code.size();
    int param = 0, separ = 0;
    string token, last;
    int hist[10] = {0};

    if (code[size-1] != ")" && code[size-1] != "}") return false;

    for (int i = 0; i < size; i++) {
        if (regex_match (code[i], regex("[0-9]+[A-Za-z]+"))) {
           return false;
        }
    }

    if (code[0] == "(") {
        return false;
    } else if (code[0] == "{") {

    } else {

    }
    return true;
}
bool isvalid1 (const vector<string> &code) {

    size_t index = 0, size = code.size();
    int param = 0, separ = 0, paren = 0, brace = 0;
    string token, last;

    if (code.size() && code[0] == "(") return false; // function parameters without function name
    if (code[size-1] != ")" && code[size-1] != "}") return false;

    while (index < code.size()) {
        token = code[index], last = index > 0 ? code[index - 1] : " ";

        if (isdigit (token[0])) {      // expression begining with digit
            for (size_t i = 1; i < token.size(); i++) {
                if (isalpha (token[i])) return false;
            }
        }
        else if (token == "(") paren++;
        else if (token == ")") paren--;
        else if (token == "}") brace++;
        else if (token == "{") {
            brace--;
            if (paren) param++;
        }
        else if (token == ",") {
            if (last == "(") return false;
            if (index + 1 < code.size() && code[index + 1]== ")") return false;
            if (paren) separ++;
        }
        else if (token == "->") {
            if (last == "(" || last == "{") return false;
        }
        else {
            if (paren && !brace) param++;
        }
        index++;
    }

    last = index > 0 ? code[index - 1] : " ";

    if (paren || brace) return false; // invalid parenthesis or brace number
    if (param - separ >= 2) return false; // invalide number of parameters

    return true;
}
bool isvalid2 (vector<string> code) {
    size_t index = 0, size = code.size();
    vector<string> brc;

    if (code[0] == "(") return false;
    if (code[size-1] != ")" && code[size-1] != "}") return false;

    while (index < size) {
        string cell = code[index];

             if (cell == "[") { brc.push_back("]"); }
        else if (cell == "{") { brc.push_back("}"); }
        else if (cell == "(") { brc.push_back(")"); }
        else if (cell == "]") { if (brc.size() && cell == brc.back()) brc.pop_back(); }
        else if (cell == "}") { if (brc.size() && cell == brc.back()) brc.pop_back(); }
        else if (cell == ")") { if (brc.size() && cell == brc.back()) brc.pop_back(); }
        else if (cell == ",") {
            if (regex_match (code[index+1], regex("\\)|\\}"))) {
                return false;
            }
            if (regex_match (code[index-1], regex("\\(|\\{"))) {
                return false;
            }
        }
        else if (cell == "->") {
            if (regex_match (code[index+1], regex("\\)"))) {
                return false;
            }
            if (regex_match (code[index-1], regex("\\{|\\("))) {
                return false;
            }
        }
        else if (regex_match (cell, regex("[0-9]+[A-Za-z]+"))) {
              return false;
        }

        index++;
    }

    return brc.empty();
}
const char *transpile2 (const char* src) {

    string expr = src, os = "result";

    if (!isvalid3(src)) return strdup("");


    // cout << "\n";

    //
    // if (regex_search (expr, regex("[\\{\\(]\\w+\\s+\\w+"))) { // \\s*[->|\\)]
    //
    // }

    return strdup(os.c_str());
}
