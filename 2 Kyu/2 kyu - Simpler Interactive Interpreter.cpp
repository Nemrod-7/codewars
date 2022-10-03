#include <iostream>

#include <cmath>
#include <map>
#include <regex>
#include <algorithm>
#include <functional>

using namespace std;

/////////////////////////////////Assert/////////////////////////////////////////
class Assert {
  public :
  static void That (const double& actual, const double& expression) {
    cout << fixed;
    if (actual != expression) {
      cout << "actual : " << actual;
      cout << "\nexpect : " << expression;

      cout << endl;
    }
  }
};
const double& Equals (const double& entry) { return entry;}
void Test();
////////////////////////////////////////////////////////////////////////////////
/*

function        ::= fn-keyword fn-name { identifier } fn-operator expression
fn-name         ::= identifier
fn-operator     ::= '=>'
fn-keyword      ::= 'fn'

expression      ::= factor | expression operator expression
factor          ::= number | identifier | assignment | '(' expression ')' | function-call
assignment      ::= identifier '=' expression
function-call   ::= fn-name { expression }

operator        ::= '+' | '-' | '*' | '/' | '%'

identifier      ::= letter | '_' { identifier-char }
identifier-char ::= '_' | letter | digit

number          ::= { digit } [ '.' digit { digit } ]

letter          ::= 'a' | 'b' | ... | 'y' | 'z' | 'A' | 'B' | ... | 'Y' | 'Z'
digit           ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'

*/

using func = function<double(double,double)>;

template<class T = void> struct modul {
    const T operator ()(const T &lhs, const T &rhs) {
        return fmod (lhs, rhs);
    }
};

map<char,int> order {{'+', 1},{'-',1},{'*',2},{'/',2},{'%',2}};
map<char,func> operate {{'+', plus<double>()},{'-', minus<double>()},
{'*', multiplies<double>()}, {'/', divides<double>()}, {'%', modul<double>()} };

map<string, double> vars;
map<string, string> fbase;

template<class T> T getstack (vector<T> &S) {
    if (S.empty()) throw runtime_error("Invalid stack size");
    T val = S.back();
    S.pop_back();
    return val;
}

vector<string> tokenize (const string &expr) {
    regex token ("=>|(_|-)?[0-9]+(.[0-9]+|_)?|[-+*/%()=\\[\\]]|_?[a-zA-Z]+_?");
    sregex_token_iterator it (expr.begin(), expr.end(), token);
    return vector<string> (it, sregex_token_iterator());
}
string mk_func (const vector<string> &expr) {

    regex variab ("_?[a-zA-Z]+_?|_[0-9]+|[0-9]+_");
    string name = expr[0], os;
    vector<string> fn = tokenize (fbase[name]), vars;
    auto fn_op = find (fn.begin(), fn.end(), "=>");

    for (int i = 1; i != expr.size(); i++) {
        vars.push_back(expr[i]);
    }

    for (auto fn_ex = fn_op + 1; fn_ex != fn.end(); fn_ex++) {
        string token = *fn_ex;

        if (regex_match (token, variab)) {
            auto pos = find (fn.begin(), fn_op, token) - fn.begin();
            token = vars[pos];
        }
        os += token + " ";
    }

    return os;
}
string getsub (vector<string>::iterator &it, vector<string>::iterator nd) {
    int pile = 1;
    string sub;

    for (it = it + 1; pile != 0 && it != nd ; it++) {
        pile += (*it == "(") - (*it == ")");
        if (pile == 0) break;
        sub += *it + " ";
    }

    return sub;
}

double interpret (std::string src) {

    vector<string> expr = tokenize(src);
    if (!expr.size()) throw runtime_error ("Empty expression.");

    vector<string>::iterator it = expr.begin(), end = expr.end();
    regex number ("^-?[0-9]+(.[0-9]+)?$");
    regex variab ("_?[a-zA-Z]+_?|_[0-9]+|[0-9]+_");

    map<string,int> order {{"+",1},{"-",1},{"*",2},{"/",2},{"%",2}};
    map<string,func> operate {{"+", plus<double>()},{"-", minus<double>()},
    {"*", multiplies<double>()},{"/", divides<double>()}, {"%", modul<double>()}};

    bool running = true;

    vector<double> value;
    vector<string> oper;

    while (running) {

        int sign = 1;

        if (*it == "-" && order[*(it - 1)]) {
            sign = -1;
            it++;
        }

        string token = *it;

        if (token == "fn") {
            string name = *++it;
            auto fn_op = find (it, end, "=>");

            for (auto fn_ex = fn_op + 1; fn_ex != end; fn_ex++) {
                if (regex_match (*fn_ex, variab)) {
                    if (find (it, fn_op, *fn_ex) == fn_op)
                        throw runtime_error ("Unknown identifier");
                }
            }
            fbase[name] = getsub (it, end);

        } else if (regex_match(token, number)) {
            value.push_back(stod(token));
        } else if (regex_match(token, variab)) {
            string id = token;

            if (find (it, end, "=") != end) { //if (it + 1 < end && *(it + 1) == "=") {
                it++;
                vars[id] = interpret (getsub (it, end)) * sign;
                return vars[id];
            } else {
                if (vars.find (id) != vars.end()) {
                    value.push_back (vars[id] * sign);
                } else if (fbase.find (id) != fbase.end()) {

                    vector<string> lambda = tokenize (fbase[id]);
                    int nvar = find (lambda.begin(), lambda.end(), "=>") - lambda.begin();

                    int index = 0;
                    vector<string> args (nvar);

                    for (auto tok = it + 1; tok != expr.end(); tok++) {
                        string curr = *tok;
                        args[index] += curr + " ";

                        if (regex_match (curr, number) || vars.find (curr) != vars.end()) {
                            double val = interpret (args[index]);
                            args[index++] = to_string (val);
                        }
                    }
                    string sub;

                    for (auto i = nvar + 1; i != lambda.size(); i++) {
                        if (regex_match (lambda[i], variab)) {
                            int pos = find (lambda.begin(), lambda.begin() + nvar, lambda[i]) - lambda.begin();
                            lambda[i] = args[pos];
                        }
                        sub += lambda[i] + " ";
                    }

                    value.push_back(interpret (sub));

                    it = end;
                } else {
                    throw::logic_error ("Invalid identifier.");
                }
            }

        } else if (token == "(") {
            value.push_back(interpret (getsub (it, end)) * sign);
        } else if (token != ")") {

            if (!oper.empty() && order[oper.back()] >= order[token]) {
                double b = getstack (value), a = getstack (value);
                value.push_back(operate[getstack (oper)] (a,b));
            }
            oper.push_back(token);
        }

        it++;
        if (it >= end) running = false;
    }
    while (!oper.empty()) {
        double b = getstack (value), a = getstack (value);
        value.push_back(operate[getstack (oper)] (a,b));
    }

    return !value.empty() ? value.back() : 0;
}

int main () {

    Assert::That(interpret("a = 2"), Equals(2));
    Assert::That(interpret("b = 3"), Equals(3));
    Assert::That(interpret("fn inc x => x + 1"), Equals(0));
    Assert::That(interpret("fn avg x y => (x + y) / 2"), Equals(0));
    Assert::That(interpret("fn add x y => x + y"), Equals(0));
    Assert::That(interpret("fn echo x => x"), Equals(0));

    cout << interpret ("inc a");
    //cout << interpret ("add echo 7 echo 3");


    /*
    Assert::That(interpret("x = 7"), Equals(7));
    Assert::That(interpret("x + 6"), Equals(13));
    Assert::That(interpret("x = 13 + (y = 3)"), Equals(16));

    Assert::That(interpret("a = inc a"), Equals(1));
    Assert::That(interpret("add a b"), Equals(5));
    Assert::That(interpret("a = inc a"), Equals(2));
    Assert::That(interpret("avg a b"), Equals(2.5));
    */
    // add echo 4 echo 3
    //Test();
    cout << "end";
}


void Test () {

    interpret ("afkd = -72 % (-80 / 64 * -69) - -96");
    interpret ("urvv = -12 % (34 % -19 * -70) - -15");
    interpret ("eef = 24 * (-5 * 14 - -25) - -53");
    interpret ("mzcol = 46 / -(-22 / -51 - -17) * 32");
    interpret ("iyinw = -9 - (38 + -95 + 68) - 100");

    //vars = {{"afkd", 24},{"eef", -1027}, {"iyinw", -120}, {"mzcol", -84.4454},{"urvv", 3}};

    Assert::That(interpret("x = 1"), Equals(1.0));
    Assert::That(interpret("x"), Equals(1.0));

    Assert::That(interpret("1 + 1"), Equals(2.0));
    Assert::That(interpret("2 - 1"), Equals(1.0));
    Assert::That(interpret("2 * 3"), Equals(6.0));
    Assert::That(interpret("8 / 4"), Equals(2.0));
    Assert::That(interpret("7 % 4"), Equals(3.0));

    Assert::That(interpret("x + 3"), Equals(4.0));
    Assert::That(interpret("(4 + 2) * 3"), Equals(18));

    /*
    try {
        interpret("y");
        Assert::That(0, Equals(1));
    } catch (...) {
        Assert::That(1, Equals(1));
    }

    */
}

/*

double operation (double a, char op, double b) {

    switch (op) {
        case '+' : return a + b; break;
        case '-' : return a - b; break;
        case '*' : return a * b; break;
        case '/' : return a / b; break;
        case '%' : return fmod (a,b); break;
    }

    return 0.0;
}
int getop (char c) {
    if (c == '-' || c == '+') return 1;
    if (c == '*' || c == '/' || c == '%') return 2;
    return 0;
}

double getnum (string::iterator &it) {
    string num;

    num += *it++;
    while (isdigit(*it) || *it == '.') num += *it++;

    return stod (num);
}
string getid (string::iterator &it) {
    string id;

    while (isalnum (*it)) id += *it++;

    return id;
}
string getsub (string::iterator &it) {
    int pile = 1;
    string sub;

    while (*it++) {
        if (*it == '(') pile++;
        if (*it == ')') pile--;
        if (pile == 0) return sub;

        sub += *it;
    }
    return sub;
}

double interpret (std::string expr) {

    expr.erase (remove (expr.begin(), expr.end(), ' '), expr.end());
    if (!expr.size()) throw runtime_error ("Empty expression");

    string::iterator it = expr.begin();
    bool running = true;
    vector<double> val;
    vector<char> ops;

    int sign = 1;

    while (running) {

        int op = order[*it];

        if (*it == '-' && order[*(it - 1)]) {
            sign = -1, op = 0;
        }
        if (*it == '(') {
            string sub = getsub (it);
            double num = interpret (sub) * sign;
            val.push_back (num);
            sign = 1;
        } else if (isalpha (*it)) {
            string id = getid (it);

            if (*it == '=') {
                string sub = getsub (it);
                vars[id] = interpret (sub) * sign;
                return vars[id];
            } else {
                if (vars.find (id) == vars.end())
                    throw::logic_error ("invalid identifier");

                val.push_back (vars[id] * sign);
                sign = 1;
            }

        } else if (isdigit (*it)) {
            val.push_back (getnum (it) * sign);
            sign = 1;

        } else {

            if (op) {

                while (!ops.empty() && order[ops.back()] >= op) {
                    double b = getstack(val), a = getstack (val);
                    val.push_back (operate[getstack (ops)] (a, b));
                }
                ops.push_back(*it);
            }

            it++;
        }

        if (it >= expr.end()) running = false;
    }

    while (!ops.empty()) {
        double b = getstack (val), a = getstack (val);
        val.push_back (operate[getstack (ops)] (a, b));
    }

    return !val.empty() ? val.back() : 0;
}

*/
