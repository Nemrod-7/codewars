#include <iostream>
#include <complex>
#include <iomanip>

using namespace std;

complex<double> stoc (const string &input) {
    istringstream iss(input);
    complex<double> zx;
    iss >> zx;
    return zx;
}
string ctos(const complex<double> &zx) {
    ostringstream oss;
    oss << fixed << setprecision(30) << zx;
    return oss.str();
}

const complex<double> zero = {0.0,0.0}, one = {1.0,0.0};

struct node {
    string sym;
    complex<double> val;
    node *t1, *t2;


    node (const string &label, node *t1 = nullptr, node *t2 = nullptr) : sym (label), val(0.0,0.0), t1 (t1), t2 (t2) {}
    node (const complex<double> &value, node *t1 = nullptr, node *t2 = nullptr) : sym (""), val(value), t1 (t1), t2 (t2) {}
};

bool is_term (const string &sym) { return sym == "+" || sym == "-"; }
bool is_fact (const string &sym) { return sym == "*" || sym == "/"; }
bool is_operator (const string &input) {
    return is_fact(input) || is_term(input) || input == "^";
}
bool is_number (const string &input) {

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

node *add(node *a, node *b) {

    // if (a->sym == b->sym && !is_operator(a->sym)) return new node ("*",new node("2"), a);
    if (a->sym == "" && a->val == zero) return b;
    if (b->sym == "" && b->val == zero) return a;
    if (a->sym == "" && b->sym == "") return new node(a->val + b->val);

    return new node ("+",a,b);
}
node *sub(node *a, node *b) {

    //if (a->sym == b->sym) return new node("0");
    if (b->sym == "0") return a;
    if (is_number(a->sym) && is_number(b->sym)) return new node(to_string(stoi(a->sym) - stoi(b->sym)));

    return new node ("-",a,b);
}
node *exp(node *a, node *b) {

    if (a->sym == "1" || b->sym == "1") return a;
    if (b->sym == "0") return new node("1");
    if (a->sym == "0") return new node("0");
    if (is_number(a->sym) && is_number(b->sym)) return new node(to_string(pow(stoi(a->sym), stoi(b->sym))));

    return new node("^",a,b);
}

node *div(node *a, node *b) {

    //if (a->sym == b->sym) return new node ("1");
    if (a->sym == "0") return new node ("0");
    if (b->sym == "1") return a;
    if (is_number(a->sym) && is_number(b->sym)) return new node(to_string(stoi(a->sym) / stoi(b->sym)));

    return new node ("/",a,b);
}

// complex<double> evaluate (node *node, complex<double> value) {
//
//     if (node == nullptr) return 0;
//
//     string term = node->sym;
//     complex<double> a = evaluate(node->t1, value), b = evaluate(node->t2, value);
//     // cout << "[" << t1 <<  "]" << term << "[" << t2 << "]\n";
//
//     if (term == "x") {
//         return value;
//     } else if (is_operator(term)) {
//         switch (term[0]) {
//             case '+' : return a + b; break;
//             case '-' : return a - b; break;
//             case '*' : return a * b; break;
//             case '/' : return a / b; break;
//             case '^' : return pow(a, b); break;
//         }
//     } else {
//         complex<double> val = a;
//
//         if (term == "cos") {
//             return cos(val);
//         } else if (term == "sin") {
//             return sin(val);
//         } else if (term == "tan") {
//             return tan(val);
//         } else if (term == "log") {
//             return log(val);
//         } else if (term == "cot") { //cot(x) = cos(x)/sin(x) or cot(x) = 1 / tan(x)
//             return cos(val) / sin(val);
//         }
//     }
//
//     return 0;
// }
node *mul(node *a, node *b) {

    if (a->val == one) return b;
    if (b->val == one) return a;

    if (a->sym == "" && a->val == zero || b->val == zero) return new node (zero);
    if (a->sym == "" && b->sym == "") return new node(a->val * b->val);
    return new node ("*",a,b);
}

int main () {

    // expr = "x^73.4+70.1^73.8*x^58.9";
    complex<double> zx (-0.2,-0.5), zy (73.40,0);

    string a = ctos(zx), b = ctos(zy);
    complex<double> rx = pow(stoc(a), stoc(b));

    node *tree = mul(new node(2), mul( new node(3), new node("x")) ) ;

    // cout << rx << "\n";

    cout << "\nend\n";
}
