#include <iostream>
#include <regex>
#include <cmath>

using namespace std;

//  <variable>  ::= "x"
//  <constant>  ::= [0-9]+(.[0-9]+)?
//  <func_name> ::= "sin" | "cos" | "tan" | "cot" | "log"
// 
//  <factor>     ::= <basic> ( "^" <basic> )*
//  <expression> ::= <term> ( "+" | "-"  <term> )*
//  <term>       ::= <factor> ( "*" | "/"  <factor> )*
// 
//  <func_call>  ::= <func_name> "(" <expression> ")"
//  <basic>      ::= <constant> | <variable> | <func_call> | ( "(" <expression> ")" )
//  
//  rules of derivation :
//  con : cst   => 0
//  lin : x     => 1
//  add : a + b => a' + b'
//  min : a - b => a' - b'
//  mul : a * b => a.b' + a'.b
//  div : a / b => (a'* b − b'* a) / (b * b)
//  pow : x^a   => a.x^(a - 1)
//  exp : e^x   => e^x
//  exp : a^x   => a^x . ln (a)
//  ln  : ln(x) => 1 / x
//  sin : sin x => cos x
//  cos : cos x => -sin x
//  tan : tan x => 1 / (cos²(x))
//
//  std::get<0>(f)({ 2, 2 }) == (-32, 32) Because: f(x) = 2 * x^3 => f(2 + 2i) = 2 * (2 + 2i)^3 = -32 + 32i
//  st::get<2>(f)({ 2, 2 }) == (24, 24) Because: f''(x) = (6 * x^2)' = 12 * x => f''(2 + 2i) = 12 * (2 + 2i)^2 = 24 + 24i

template<class T = void> struct power {
    const T operator ()(const T &lhs, const T &rhs) {
        return pow (lhs, rhs);
    }
};

void show (const std::vector<std::string> &vs) {

    for (int i = 0; i < vs.size(); i++) {
        std::cout << "[" << vs[i] << "]";
    }
}

string differentiate (const string &input) {

    regex term ("^[-+*/^]$");
    regex numr ("^-?[0-9]+(\\.[0-9]+)?$");
    regex func ("^[sin|cos|tan|cot|log]$");
    regex tokn ("([0-9]+(\\.[0-9]+)?)|x|[-+*^]|(sin|cos|tan|cot|log)");

    if (input == "x") return "1";
    if (regex_match(input, numr)) return "0";

    sregex_token_iterator iter (input.begin (), input.end (), tokn);
    vector<string> code (iter, sregex_token_iterator ());

    if (code.size() == 3) {
        string a = code[0], op = code[1], b = code[2];

        if (regex_match (a, numr) && regex_match (b, numr)) {
            ostringstream os;
            os << operate[op] (stod (a), stod (b));
            return os.str();
        }

        if (op == "+") {
            if (a == b) { op = "*", a = "2"; }
            if (a == "0") return b;
            if (b == "0") return a;
        } else if (op == "-") {
            if (a == b) return "0";
            if (b == "0") return a;
        } else if (op == "*") {
            if (a == b) { op = "^", a = "2"; }
            if (a == "0" || b == "0") return "0";
            if (a == "1") return b;
            if (b == "1") return a;
        } else if (op == "/") {
            if (a == b) return "1";
            if (a == "0") return "0";
            if (b == "1") return a;
        } else if (op == "^") {
            if (a == "1" || b == "1") return a;
            if (b == "0") return "1";
            if (a == "0") return "0";
        }

    }




    //    vector<string> t1, t2;
    //
    //    for (int i = 0; i < code.size(); i++) {
    //
    //        if (code[i] == "x") {
    //            cout << "variable : [" << code[i] << "]";
    //        }
    //        else if (regex_match(code[i], numr)) {
    //            cout << "number   : [" << code[i] << "]";
    //        } else if (regex_match(code[i], term)) {
    //            t1 = {code.begin(), code.begin() + i};
    //            t2 = {code.begin() + i + 1, code.end()};
    //
    //            if (code[i] == "^") {
    //
    //            }
    //            cout << "operator : " ;
    //            show (t1);
    //            cout << " <- " << code[i] << " -> ";
    //            show(t2);
    //        } else if (regex_match(code[i], func)) {
    //            cout << "function : " ;
    //            t1 = {code.begin() + i + 1, code.end()};
    //            cout << code[i] << " -> ";
    //            show(t1);
    //        }
    //
    //        cout << endl;
    //    }

    return "";
}

int main () {


    differentiate("2 * x^3");

}
