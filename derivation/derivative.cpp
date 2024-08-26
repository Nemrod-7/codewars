#include <iostream>
#include <string>
#include <vector>
#include <complex>
#include <functional>
#include <cmath>
//#include "tests"

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

//    con : cst   => 0
//    lin : x     => 1
//    add : a + b => a' + b'
//    min : a - b => a' - b'
//    mul : a * b => a.b' + a'.b
//    div : a / b => (a'* b − b'* a) / (b * b)
//    pow : x^a   => a.x^(a - 1)
//    exp : a^x   => a^x . ln (a)
//    log : ln(x) => x' / x
//    sin : sin x => cos x
//    cos : cos x => -sin x
//    tan : tan x => 1 / (cos²(x))

using namespace std;

using value_t = complex<double>;
using func_t = function<value_t(value_t)>;

struct Node {
    string token;
    Node *t1, *t2;

    Node (const string &label = "") : token (label), t1 (nullptr), t2 (nullptr) {}
};

void show (const vector<string> &vs) {

    for (int i = 0; i < vs.size(); i++) {
        cout << "[" << vs[i] << "]";
    }
    cout << endl;
}
void shownode (const Node *now) {
    cout << "[" << now->token;
}
void showtree (const Node *now) {
    if (now != nullptr) {
        shownode (now);
        showtree (now->t1);
        showtree (now->t2);
        cout << "]";
    }
}

bool isnum (const string &input) {

    if (input.size() == 0) return false;
    int i = 0, end = input.size();

    if (input.front() == '(' && input.back() == ')') {
        i += 1, end -= 1;
    }

    for (; i < end; i++) {
        if (input[i] == '-' && isdigit(input[i+1])) continue;
        if (input[i] != '.' && input[i] != ',' && !isdigit(input[i])) {
            return false;
        }
    }

    return true;
}
bool isfunc (const string &input) {
    return input == "sin" || input == "cos" || input == "tan" || input == "log" || input == "cot";
}
bool is_operator (const string &input) {
    return input == "+" || input == "-" || input == "*" || input == "^" || input == "/";
}
int order (const string &src) {
    if (src == "+" || src == "-") return 1;
    if (src == "*" || src == "/") return 2;
    if (src == "^") return 3;
    return 0;

}

vector<string> tokenize (const string &input) {

    vector<string> code;
    int i = 0;

    while (i < input.size()) {

        if (isdigit(input[i])) {
            string buffer;

            while (isdigit(input[i]) || input[i] == '.') buffer += input[i++];
            code.push_back(buffer);
        } else if (isspace(input[i])) {
            while (isspace(input[i])) i++;
        } else if (input[i] == '+') {
            code.push_back(string(1,input[i++]));
        } else if (input[i] == '*' || input[i] == '/') {
            code.push_back(string(1,input[i++]));
        } else if (input[i] == '^') {
            code.push_back(string(1,input[i++]));
        } else if (isalpha(input[i])) {
            string buffer;

            while (isalpha(input[i])) buffer += input[i++];
            code.push_back(buffer);
        } else if (input[i] == '-') {
            if (code.size() == 0 || is_operator(code.back())) {
                string buffer = "-";
                i++;

                while (isdigit(input[i]) || input[i] == '.') buffer += input[i++];
                code.push_back(buffer);
            } else {
                code.push_back(string(1,input[i++]));
            }
        }

        else {
            code.push_back(string(1,input[i++]));
        }
    }

    return code;
}
string parenthesis(vector<string>::iterator &it) {

    it += 1;
    int pile = 1;
    string sub ;

    while (true) {
        pile += (*it == "(") - (*it == ")");
        if (pile == 0) break;
        sub += *it++;
    }

    // it++;
    return sub;
}
template<class T> T getstack (vector<T> &stack) {
    T val = stack.back();
    stack.pop_back();
    return val;
}

double dround (double num) { return floor(1e5 * num) / 1e5; }
value_t cround (const value_t &zx) { return { dround(zx.real()), dround(zx.imag()) };}
value_t stoc (const string &input) {
    istringstream iss(input);
    value_t zx;
    iss >> zx;
    return zx;
}
string ctos(const value_t &zx) {
    ostringstream oss;

    if (zx.imag() == 0) {
        oss << zx.real();
    } else {
        oss << zx;
    }
    //zx.imag() == 0 ? oss << zx.real() : oss << zx;
    return oss.str();
}

Node *parse (const string &input) {

    vector<string> code = tokenize(input), oper;
    vector<string>::iterator it = code.begin();
    vector<Node*> tree;

    while (it < code.end()) {
        string cell = *it;

        if (cell == "x") {
            tree.push_back( new Node(cell));
        } else if (cell == "(") {
            //cout << "[" << sub << "]";
            tree.push_back(parse(parenthesis(it)));
        } else if (isnum(cell)) {
            tree.push_back( new Node(cell));
        } else if (is_operator(cell)) {

            while(!oper.empty() && order(oper.back()) > order(cell)) {
                Node *next = new Node(getstack(oper));
                next->t2 = getstack(tree);
                next->t1 = getstack(tree);
                tree.push_back(next);
            }

            oper.push_back(cell);
        } else if (isfunc(cell)) {
            it++;
            Node *next = new Node(cell);
            next->t1 = parse(parenthesis(it));
            tree.push_back(next);
        } else {
            cout << "invalid identifier\n";
        }

        it++;
    }


    while(!oper.empty()) {
        Node *next = new Node(getstack(oper));
        next->t2 = getstack(tree);
        next->t1 = getstack(tree);


        //shownode(next->t1);
        //shownode(next->t2);
        tree.push_back(next);
    }

    //showtree(tree.back());
    return tree.back();
}

string operate (string a, string op, string b) {

    if (isnum(a) && isnum(b)) {
        value_t z1 = stoc(a), z2 = stoc(b);
        value_t result;

        switch (op[0]) {
            case '+' : result = cround(z1 + z2) ; break;
            case '-' : result = cround(z1 - z2) ; break;
            case '*' : result = cround(z1 * z2) ; break;
            case '/' : result = cround(z1 / z2) ; break;
            case '^' : result = cround(pow(z1, z2)) ; break;
        }
        //        cout << "[" << z1 << "] " << code[i]<< " [" << z2 << "]";
        //        cout << " => " << result << "\n";
        return ctos(result);
    }

    if (op == "+") {
        if (a == b) { op = "*", a = "2"; }
        if (a == "0") return b;
        if (b == "0") return a;
    } else if (op == "-") {
        if (a == b) return "0";
        if (b == "0") return a;
    } else if (op == "*") {
        if (a == b) { op = "^", b = "2"; }
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

    return a + op + b;
}
string evaluate (Node *node, string value = "") {

    if (node != nullptr) {
        string term = node->token;
        string t1 = evaluate(node->t1, value), t2 = evaluate(node->t2, value);

        if (term == "x") {
            return value == "" ? term : value;
        } else if (is_operator(term)) {
            //cout << "[" << t1 << "]" << term << "[" << t2 << "] " << "\n";
            return operate(t1,term,t2);
        } else if (isfunc(term)) {

            if (isnum(t1)) {
                value_t val = stoc(t1);

                if (term == "cos") {
                    return ctos(cround(cos(val)));
                } else if (term == "sin") {
                    return ctos(cround(sin(val)));
                } else if (term == "tan") {
                    return ctos(cround(tan(val)));
                } else if (term == "log") {
                    return ctos(cround(log(val)));
                } else if (term == "cot") { //cot(x) = cos(x)/sin(x) or cot(x) = 1 / tan(x)
                    return ctos(cround( cos(val) / sin(val) ));
                }

            } else {
                return term + "(" + t1 + ")";
            }
        }

        return t1 + term + t2;
    }

    return "";
}
string derivate (Node *node) {

    if (node != nullptr) {
        string term = node->token;
        string t1 = evaluate(node->t1), t2 = evaluate(node->t2);

        /*cout << "[" << t1 << "]" << term << "[" << t2 << "] => " << result << "\n";*/
        if (term == "x") {
            return "1";
        } else if (isnum(term)) {
            return "0";
        } else if (term == "+") { // a' + b'
            string d1 = derivate(node->t1), d2 = derivate(node->t2);
            string result = operate(d1,term,d2);

            return result;
        } else if (term == "-") { // a' - b'
            string d1 = derivate(node->t1), d2 = derivate(node->t2);
            string result = operate(d1,term,d2);

            return result;
        } else if (term == "*") { // a'.b + a.b'
            string d1 = derivate(node->t1), d2 = derivate(node->t2);
            string result;

            return operate(operate(d1,"*",t2),"+",operate(t1,"*",d2));
        } else if (term == "/") { // (a'.b - a.b') / (b.b)
            string d1 = derivate(node->t1), d2 = derivate(node->t2);

            string a1 = operate(operate(d1,"*",t2),"-",operate(t1,"*",d2));
            string b1 = operate(t2,"*",t2);

            return operate(a1,"/",b1);
        } else if (term == "^") {
            string d1 = derivate(node->t1);
            /*cout << "[" << d1 << "]  \n";*/

            if (t2 == "x") { //  exp : a^x   => a^x . ln (a)
                string arg1 = operate(t1,"^",t2);
                string arg2 = "(" + operate("log(" + t1 + ")","+",d1) + ")";

                //cout << arg1 << " " << arg2 << '\n';
                return operate(arg1,"*",arg2);
            } else {
                cout << "[" << t1 << "]" << term << "[" << t2 << "] => \n";

                if (isnum(t2)) {
                    string dx1 = derivate(node->t1);
                    string exp = operate (t1, "^", operate (t2, "-", "1"));

                    return operate (dx1, "*", operate (t2, "*", exp));
                }
            }

        } else if (term == "cos") { // dx = -sin x
            string exp = operate("0","-",derivate(node->t1));
            string arg = "(sin (" + t1 + "))";
            //cout << "[" << term << "] => " << t1 << '\n';

            //cout << "cos => exp : " << exp << " = " << operate(exp, "*", arg) << "\n";
            return operate(exp, "*", arg);
        } else if (term == "sin") { // dx = cos x
            string exp = derivate(node->t1);
            string arg = "(cos (" + t1 + "))";

            /*cout << "sin => exp : " << t1 << "\n";*/
            /*cout << node->t1->token << "\n";*/

            return operate(exp, "*", arg);
        } else if (term == "tan") { // dx = 1 / (cos^2(x))


        } else if (term == "log") { // dx = x' / x
            string dx1 = derivate(node->t1);
            string den = "(" + t1 + ")";
            return operate(dx1,"/", "(" + t1 + ")");
        } else if (term == "cot") {

        }
    }

    return "";
}

tuple<func_t, func_t, func_t> differentiate(const string& eq) {

    string pass0 = eq;
    string pass1 = derivate(parse(pass0));
    string pass2 = derivate(parse(pass1));

    /*cout << "\nresult : \n";*/
    /*cout << "pass0  : " << pass0 << "\n";*/
    /*cout << "pass1  : " << pass1 << "\n";*/
    /*cout << "pass2  : " << pass2 << "\n";*/

    return {
        { [pass0](value_t x) { return stoc ( evaluate(parse(pass0), ctos(x))); } },
            { [pass1](value_t x) { return stoc ( evaluate(parse(pass1), ctos(x))); } },
            { [pass2](value_t x) { return stoc ( evaluate(parse(pass2), ctos(x))); } },
    };
}


int main () {

    string expression = "log(3*x)";
    Node *tree = parse(expression);
    string pass1 = derivate(tree);

    showtree(tree);
    cout << " { " << pass1 << " } " << "\n";
    // string pass2 = derivate(parse(pass1));
    // const auto [f, df_dx, d2f_dx2] = differentiate("sin(cos(x^x^2))");
    //value_t{ 0.839472, -0.0115338 })

    // cout << f({1,1});
    // cout << evaluate(tree, ctos({1,1}));

    //show(token);
    //cout << operate("x^x","^","2")t


}
