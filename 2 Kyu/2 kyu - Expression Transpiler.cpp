#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

vector<string> tokenize (string src) {

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

    size_t index = 0;
    int param = 0, separ = 0, paren = 0, brace = 0;
    string token, last;

    if (code.size() && code.front() == "(") return false; // function parameters without function name

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

    if (last != ")" && last != "}") return false;
    if (paren || brace) return false; // invalid parenthesis or brace number
    if (param - separ >= 2) return false; // invalide number of parameters

    return true;
}
string lambda (vector<string>::iterator &end) {

    end++;
    if (*end == "}") return "(){}";

    vector<string>::iterator start = end, mid = start;
    int separ = 0, param = 0;
    string os ("(");

    while (*end != "}") {
        if (*end == "->") mid = end;
        end++;
    }

    for (auto it = start; it != end; it++) {

        if (*it == ",") {
            separ++;
        } else {
            if (it == mid) {
                separ = 0;
                os += "){";
            }
            if (it - start < mid - start) {
                if (separ != param++) return "";
                os += *it;
                if (it - start < mid - start - 1) os += ",";

            } else if (*it != "->") {
                if (separ) return "";
                os += *it + ";";
            }
        }
    }

  return os + "}";
}
const char *transpile (const char* source) {

    string expression = source, os;
    if (expression == "{}{}{}") return strdup (os.c_str());
    vector<string> code = tokenize (expression);
    vector<string>::iterator it = code.begin();
    cout << expression << "\n";
    if (!isvalid (code)) return os.c_str();

    while (it != code.end()) {

        string tok = *it;

        if (tok == "(") {
            os += tok;
            it++;
            vector<string> param;

            while (*it !=  ")") {
                if (*it != ",") {
                    if (*it == "{") {
                        param.push_back (lambda (it));
                    } else {
                        param.push_back (*it);
                    }

                    if (*it == ")") break;
                }
                it++;
            }

            for (auto &it : param) os += it + ",";
            if (os.back() == ',') os.pop_back();

        } else if (tok == "{") {
            if (it - code.begin() >= 2) {
                if (*(it - 1) == ")" && *(it - 2) != "(") {
                    os += ',';
                }
            }
            string tmp = lambda (it);
            if (tmp == "") return strdup ("");
            os += tmp;
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

int main () {

  const char *res = transpile ("invoke  (       a    ,   b   ) { } ");
  cout <<  res << endl;
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
