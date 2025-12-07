#include <iostream>

#include <cmath>
#include <map>
#include <regex>
#include <algorithm>
#include <functional>

using namespace std;
// simple interactive interpreter v4.0
//
//function        ::= fn-keyword fn-name { identifier } fn-operator expression
//fn-name         ::= identifier
//fn-operator     ::= '=>'
//fn-keyword      ::= 'fn'
//
//expression      ::= factor | expression operator expression
//factor          ::= number | identifier | assignment | '(' expression ')' | function-call
//assignment      ::= identifier '=' expression
//function-call   ::= fn-name { expression }
//
//operator        ::= '+' | '-' | '*' | '/' | '%'
//
//identifier      ::= letter | '_' { identifier-char }
//identifier-char ::= '_' | letter | digit
//
//number          ::= { digit } [ '.' digit { digit } ]
//
//letter          ::= 'a' | 'b' | ... | 'y' | 'z' | 'A' | 'B' | ... | 'Y' | 'Z'
//digit           ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'


using func = function<double(double,double)>;

template<class T = void> struct modul {
    const T operator ()(const T &lhs, const T &rhs) {
        return fmod (lhs, rhs);
    }
};
template<class T = void> struct power {
    const T operator ()(const T &lhs, const T &rhs) {
        return pow (lhs, rhs);
    }
};

map<string,int> order {{"+",1},{"-",1},{"*",2},{"/",2},{"%",2},{"^",2}};
map<string,func> operate {{"+", plus<double>()},{"-", minus<double>()},
{"*", multiplies<double>()},{"/", divides<double>()}, {"%", modul<double>()}, {"^", power<double>()} };

map<string, double> vars;               // values base : <arg name, value>
map<string, pair<string,string>> fbase; // functions base : <func name, <args, function>>

template<class T> T getstack (vector<T> &S) {
    if (S.empty()) throw runtime_error("Invalid stack size");
    T val = S.back();
    S.pop_back();
    return val;
}

vector<string> tokenize (const string &expr) {
  // ("\\s* ( => | [-+*%=\(\)] | [A-Za-z_] [A-Za-z0-9_]* | [0-9]*\.?[0-9]+ ) \\s*");
  // regex re ("=>|_?[0-9]+(\\.[0-9]+)?|[-+*/%()=\\[\\]]|_?[a-zA-Z]+_?");
  // regex re ("\\s*(=>|[-+*\/\%=\(\)]|[A-Za-z_][A-Za-z0-9_]*|[0-9]*\.?[0-9]+)\\s*");
  regex re ("\\s*(=>|[-+*%=\\/\\(\\)]|[A-Za-z_][A-Za-z0-9_]*|[0-9]*.?[0-9]+)\\s*");

    sregex_token_iterator it (expr.begin(), expr.end(), re);
    return vector<string> (it, sregex_token_iterator());
  }

vector<string> getargs (vector<string>::iterator &it, const vector<string> &expr) {

    const string name = *it;
    const int nargs = tokenize(fbase[name].first).size();
    vector<string> args;
    cout << name << " => ";

    while (it + 1 < expr.end() && args.size() < nargs) {
        it++;
        string cell = *it;

        if (cell == "(") {
            while (*it != ")") {
              cell += *++it;
            }
        } else if (fbase.find(cell) != fbase.end()) {
            vector<string> nxt = getargs (it, expr);

            for (auto &it2 : nxt) {
                cell += " " + it2;
            }
        }

        cout << "["<< cell << "]";
        /*

        */
        args.push_back(cell);
    }

    return args;
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
bool isvar (const vector<string> &expr, int index) {

    if (expr[index] != "-") return false;
    if (index == 0) return true;
    if (order[expr[index - 1]]) return true;
    return false;
}

double pass1 (vector<string> expr) {

    vector<string>::iterator it = expr.begin(), end = expr.end(), start = expr.begin();
    regex number ("^-?[0-9]+(.[0-9]+)?$");
    regex identi ("_?[a-zA-Z]+_?|_[0-9]+");

    // display(expr);
    bool running = true;

    vector<double> value;
    vector<string> oper;

    while (running) {
        string curr = *it;

        if (curr == "fn") {
            // string name = *++it;
            // auto mid = find (it, end, "=>");
            //
            // for (auto fn_ex = it + 1; fn_ex != mid; fn_ex++) {
            //     if (find (mid, end, *fn_ex) == end)
            //         throw::logic_error ("Unknown identifier.");
            //
            //     if (find (it + 1, fn_ex, *fn_ex) != fn_ex)
            //         throw::logic_error ("Invalid function.");
            // }
            //
            // fbase[name] = {getsub (it, mid), getsub (it, end)};
        } else if (curr == "(") {
            value.push_back(pass1 (tokenize(getsub (it, end))));
        } else if (regex_match(curr, number)) {
            value.push_back(stod(curr));
        } else if (regex_match(curr, identi)) {

        } else {
            throw::logic_error ("Invalid operator.");
        }
        // if (regex_match(token, identi)) {
        //     if (token == "fn") {

        //     } else if (find (it, end, "=") != end) {
        //
        //         if (fbase.find (token) != fbase.end())
        //             throw::logic_error ("Invalid initializer.");
        //
        //         vars[token] = pass1 ({it + 2, end});
        //         return vars[token];
        //     } else if (vars.find (token) != vars.end()) {
        //         value.push_back (vars[token]);
        //     } else if (fbase.find (token) != fbase.end()) {
        //
        //         vector<string> args = getargs (it, expr);
        //         vector<string> vars = tokenize (fbase[token].first), lmda = tokenize (fbase[token].second);
        //         cout << token << "\n";
        //         // display(args);
        //         if (args.size() != vars.size())
        //             throw::logic_error ("Invalid number of argument.");
        //
        //         for (auto &tok : lmda) {
        //             if (regex_match (tok, identi)) {
        //                 int pos = find (vars.begin(), vars.end(), tok) - vars.begin();
        //                 tok = to_string (pass1 (tokenize(args[pos])));
        //             }
        //         }
        //
        //         value.push_back(pass1 (lmda));
        //
        //     } else {
        //         throw::logic_error ("Invalid identifier.");
        //     }
        // } else if (token == "(") {
        //     value.push_back(pass1 (tokenize(getsub (it, end))) * sign);
        // } else if (order[token] != 0){
        //
        //     if (!oper.empty() && order[oper.back()] >= order[token]) {
        //         double b = getstack (value), a = getstack (value);
        //         value.push_back(operate[getstack (oper)] (a,b));
        //     }
        //     oper.push_back(token);
        // } else {
        //     throw::logic_error ("Invalid operator.");
        // }

        it++;
        if (it >= end) running = false;
    }

    while (!oper.empty()) {
        double b = getstack (value), a = getstack (value);
        value.push_back(operate[getstack (oper)] (a,b));
    }

    switch (value.size()) {
        case 0 : return 0; break;
        case 1 : return value.back(); break;
        default : return -1; break; // throw runtime_error("Invalid stack size.");
    }
}
double interpret (const string &input) {

    double res = 0.0;
    vector<string> expr = tokenize(input);

    if (!expr.size()) return 0;

    try {
        res = pass1(expr);
        // cout << input << " => ";
        // cout << res << "\n";
    } catch (const exception &x) {
        cout << "error : " << x.what() << "\n";
    }

    return res;
}

void getinput () {

    enum {normal, differ};

    string input;
    int mode = normal;
    cout << "Interpreter : \n"
    "commands : derivate, interpret, exit | type input\n"
    ;

    while (true) {
        getline (cin, input);

        if (input == "exit" || input == "quit") {
            break;
        } else if (input == "derivate") {
            mode = differ;
        } else if (input == "interpret") {
            mode = normal;
        } else {
            switch (mode) {
                case normal: interpret(input); break;
                case differ: break;
                default: break;
            }
        }
    }

    cout << "end";
    // Test();
}
/////////////////////////////////Assert/////////////////////////////////////////
class Assert {
    public :
      template<class T> static void That (const T& actual, const T& expression) {
          cout << fixed;
          if (actual != expression) {
              cout << "actual : " << actual;
              cout << "\nexpect : " << expression;

              cout << endl;
          }
    }
};
const double& Equals (const double& entry) { return entry;}
void display (vector<string> expr) {
    for (auto it : expr) {
        cout << "[" << it << "]";
    }
    cout << endl;
}

void Test () {

      Assert::That(interpret("fn inc x => x + 1"), Equals(0));
      Assert::That(interpret("fn avg x y => (x + y) / 2"), Equals(0));
      Assert::That(interpret("fn add x y => x + y"), Equals(0));
      Assert::That(interpret("fn echo x => x"), Equals(0));

      Assert::That(interpret("a = 2"), Equals(2));
      Assert::That(interpret("b = 3"), Equals(3));
      Assert::That(interpret("x = 7"), Equals(7));
      Assert::That(interpret("x + 6"), Equals(13));
      Assert::That(interpret("x = 13 + (y = 3)"), Equals(16));

      Assert::That(interpret("a = 0"), Equals(0));
      Assert::That(interpret("a = inc a"), Equals(1));
      Assert::That(interpret("a = inc a"), Equals(2));

      Assert::That(interpret("add a b"), Equals(5));
      Assert::That(interpret("avg a b"), Equals(2.5));
      Assert::That(interpret("add echo 3 echo 7"), Equals(10));

      Assert::That(interpret("1 + 1"), Equals(2.0));
      Assert::That(interpret("2 - 1"), Equals(1.0));
      Assert::That(interpret("2 * 3"), Equals(6.0));
      Assert::That(interpret("8 / 4"), Equals(2.0));
      Assert::That(interpret("7 % 4"), Equals(3.0));
      Assert::That(interpret("avg 4 2 + avg 10 30"), Equals(23.0));

      Assert::That(interpret("4 / 2 * 3"), Equals(6));

      Assert::That(interpret("x = -6"), Equals(-6));
}
///////////////////////////////////////////////////////////////////////////////////////
int main (int argc, char **argv) {



    // interpret("(fn f => 1)");
    // Assert::That(interpret("fn sq x => x * x"), Equals(0));
    // cout << interpret ("sq (4/2)");

    // string input = " 4 5 6";
    // vector<string> expr = tokenize (input);
    // vector<string>::iterator it = expr.begin();
    //
    // vector<string> args = getargs (it, expr);
    // display(args);

    cout << "end";
}
