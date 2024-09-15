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

node *add(node *a, node *b) {
    // if (a->sym == b->sym && !is_operator(a->sym)) return new node ("*",new node("2"), a);
    if (a->sym == "" && a->val == 0.0) return b;
    if (b->sym == "" && b->val == 0.0) return a;
    if (a->sym == "" && b->sym == "") return new node(a->val + b->val);

    return new node ("+",a,b);
}
node *sub(node *a, node *b) {

    //if (a->sym == b->sym) return new node("0");
    if (b->sym == "" && b->val == 0.0) return a;
    if (a->sym == "" && b->sym == "") return new node(a->val - b->val);
    return new node ("-",a,b);
}
node *exp(node *a, node *b) {

    if (a->val == 1.0 || b->val == 1.0) return a;
    if (b->sym == "" && b->val == 0.0) return new node(1.0);
    if (a->sym == "" && b->sym == "") return new node(pow(a->val , b->val));

    return new node("^",a,b);
}
node *div(node *a, node *b) {

    //if (a->sym == b->sym) return new node (1.0);
    if (a->sym == "" && a->val == 0.0) return new node(0.0);
    if (b->sym == "" && b->val == 1.0) return a;
    if (a->sym == "" && b->sym == "") return new node(a->val / b->val);

    return new node ("/",a,b);
}
node *mul(node *a, node *b) {

    if (a->val == 1.0) return b;
    if (b->val == 1.0) return a;
    if ((a->sym == "" && a->val == 0.0) || (b->sym == "" && b->val == 0.0)) return new node (0.0);
    if (a->sym == "" && b->sym == "") return new node(a->val * b->val);
    return new node ("*",a,b);
}

int main () {

   string expr = "cot(39.6^11.5*42.3/x+93.7^73.3)";
   complex<double> x (-4.87,-7.25);

   complex<double> a(39.6), b(11.5), c(42.3), d(93.7), e(73.3);
   complex<double> val = pow(a,b) * c / x + pow(d,e);

   cout << cos(val) << "\n";
   cout << sin(val) << "\n";
   cout << tan(val) << "\n";


}
