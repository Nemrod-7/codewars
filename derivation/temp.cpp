#include <iostream>
#include <complex>
#include <iomanip>

using namespace std;

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
    if (b->sym == "" && b->val == zero) return a;
    if (a->sym == "" && b->sym == "") return new node(a->val - b->val);
    return new node ("-",a,b);
}
node *exp(node *a, node *b) {

    if (a->val == one || b->val == one) return a;
		if (b->sym == "" && b->val == zero) return new node(one);
    if (a->sym == "" && b->sym == "") return new node(pow(a->val , b->val));

    return new node("^",a,b);
}

node *div(node *a, node *b) {

    //if (a->sym == b->sym) return new node ("1");
    if (a->sym == "" && a->val == zero) return new node(zero);
    if (b->sym == "" && b->val == one) return a;
		if (a->sym == "" && b->sym == "") return new node(a->val / b->val);

    return new node ("/",a,b);
}

node *mul(node *a, node *b) {

    if (a->val == one) return b;
    if (b->val == one) return a;
    if (a->sym == "" && a->val == zero || b->sym == "" && b->val == zero) return new node (zero);
    if (a->sym == "" && b->sym == "") return new node(a->val * b->val);
    return new node ("*",a,b);
}

void showtree(const node *node, bool isLeft = false, const string &prefix = "") {
    if (node != nullptr) {
        cout << prefix;
				cout << (isLeft ? "├─" : "└─" );

				if (node->sym == "") {
						cout << "[" << node->val << "]" << endl;
				} else {

						cout << "[" << node->sym << "]" << endl;
				}

				showtree(node->t1, true, prefix + (isLeft ? "│  " : "   "));
				showtree(node->t2, false, prefix + (isLeft ? "│  " : "   "));
		}
}
int main () {

		node *tree = mul(new node(2), mul(new node(3), exp( new node("x"), new node(2)))) ;

		showtree(tree);
		//cout << curr->t2->sym;


		//showtree(tree);
		cout << "\nend\n";
}
