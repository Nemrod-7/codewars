#include <iostream>
#include <complex>
#include <cmath>

using namespace std;

struct node {
    string sym;
    complex<double> val;
    node *t1, *t2;

    node (const string &label, node *t1 = nullptr, node *t2 = nullptr) : sym (label), val(0.0,0.0), t1 (t1), t2 (t2) {}
    node (const complex<double> &value, node *t1 = nullptr, node *t2 = nullptr) : sym (""), val(value), t1 (t1), t2 (t2) {}
};


void getnode (node *now) {
    if (now != nullptr) {
        if (now->sym == "") {
            cout << "(" << now->val.real() << "+" << now->val.imag() << "i)";
        } else {
            cout << now->sym;
        }
    }
}

void freenode(node *now) {

    if (now != nullptr) {
        freenode(now->t1);
        freenode(now->t2);

        delete now;
    }
}
node *operate(node *a, char op, node *b) {
    node *next = nullptr;

    switch(op) {
        case '+' : next = new node(a->val + b->val); break;
        case '-' : next = new node(a->val - b->val); break;
        case '*' : next = new node(a->val * b->val); break;
        case '/' : next = new node(a->val / b->val); break;
        case '^' : next = new node(pow(a->val,b->val)); break;
    }

    freenode(a);
    freenode(b);
    return next;
}
node *add(node *a, node *b) {
    // if (a->sym == b->sym && !is_operator(a->sym)) return new node ("*",new node("2"), a);
    if (a->sym == "" && a->val == 0.0) return b;
    if (b->sym == "" && b->val == 0.0) return a;
    if (a->sym == "" && b->sym == "") return operate(a,'+',b);

    return new node ("+",a,b);
}
node *sub(node *a, node *b) {

    if (b->sym == "" && b->val == 0.0) return a;
    if (a->sym == "" && b->sym == "") return operate(a,'-',b);

    return new node ("-",a,b);
}
node *exp(node *a, node *b) {

    if (a->val == 1.0 || b->val == 1.0) return a;
    if (b->sym == "" && b->val == 0.0) return new node(1.0);
    if (a->sym == "" && b->sym == "") return operate(a,'^',b);

    return new node("^",a,b);
}
node *div(node *a, node *b) {

    // if (a->sym == b->sym) return new node (1.0);
    if (a->sym == "" && a->val == 0.0) return new node(0.0);
    if (b->sym == "" && b->val == 1.0) return a;
    if (a->sym == "" && b->sym == "") return operate(a,'/',b);

    return new node ("/",a,b);
}
node *mul(node *a, node *b) {

    if (a->val == 1.0) return b;
    if (b->val == 1.0) return a;
    if ((a->sym == "" && a->val == 0.0) || (b->sym == "" && b->val == 0.0)) return new node (0.0);
    if (a->sym == "" && b->sym == "") return operate(a,'*',b);

    return new node ("*",a,b);
}

int main () {

    // exp : x^y   => x^y . (x'.(y/x) + y'.log(x))
    // x^63 => x^63 * (63/x)) => 63 * x^62

    complex<double> x(-3.35,3.35);
    // node *t1 = mul(new node(63.0), exp(new node(x), new node(62.0)));
    // node *t2 = mul( exp(new node(x), new node(63.0)), div(new node(63.0),  new node(x)) );

    // complex<double> x1 = pow(x, 3);
    // complex<double> x2 = pow(x, 2);
    // complex<double> ex = {-16, 16} ; // pow(x, 3.0);
    // ex= pow(x, 3.0);
    //    cout << fixed;
    //
    //    cout << pow(x, 62.0) * 63.0 << "\n";
    //    cout << pow(x, 63.0) * 63.0 / x << "\n";
    //    cout << pow(x, 63.0) / x * 63.0 << "\n";

    node *curr = new node ("*");
    node *a = new node(3.0), *b = new node(2.0);

    curr->t1 = a, curr->t2 = b;

    node *next = mul(a,b);




}
