#include <iostream>
#include <vector>
#include <cmath>
#include <complex>

using namespace std;

void showvect(const vector<string> &ve) {
    for (auto &it : ve) {
        cout << "[" << it << "]";
    }
    cout << endl;
}

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

    // cout << a->sym << "/" << b->sym << endl;
    // if (a->sym == b->sym) return new node (1.0);
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

bool is_func (const string &input) {
    return input == "sin" || input == "cos" || input == "tan" || input == "log" || input == "cot";
}
bool is_term(const string &sym) { return sym == "+" || sym == "-"; }
bool is_fact(const string &sym) { return sym == "*" || sym == "/"; }
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
bool is_operator (const string &input) {
    return is_fact(input) || is_term(input) || input == "^";
}

template<class T> T get(node* curr) {
    if (curr->sym == "") return curr->val;
    return curr->sym;
}
string evaluate (node *node) {

      if (node != nullptr) {
          string term = node->sym;

          if (is_func(term)) {
              cout << term << "(";
              evaluate(node->t1);
              cout << ")";
          } else {
              evaluate(node->t1);

                if (term == "") {
                    cout << node->val;
                } else {
                    cout << term;
                }
              evaluate(node->t2);
          }
      }
    return "";
}


string gethash(node *curr) {

    if (curr == nullptr) return "";
    string hash;

    if (is_number(curr->sym)) {
        hash = "1*0";
    } else if (is_operator(curr->sym)) {
        hash += is_number(curr->t1->sym) ? '1' : 'x';
        hash += curr->sym;
        hash += is_number(curr->t2->sym) ? '1' : 'x';
    } else {
        hash = "x*0";
    }

    return hash;
}
node *simplify (node *curr) {

    if (curr != nullptr) {
        string hash = gethash(curr->t1) + curr->sym + gethash(curr->t2);
				curr->t1 = simplify(curr->t1), curr->t2 = simplify(curr->t2);
        node *a = curr->t1, *b = curr->t2;

				if (hash == "1*0*x*1") { // (5) * (x * 3)
						return mul(mul(a, b->t2), b->t1);
				} else if (hash == "1*0*1*x") { // (5) * (3 * x)
				//cout << "[" << evaluate(a) <<  "]" << curr->sym << "[" << evaluate(b) << "]\n";
						return mul(mul(a, b->t1), b->t2);
				} else if (hash == "1*0*1/x") { // (5) * (4 / x)
						return div(mul(a, b->t1), b->t2);
				}
				//    else if (hash == "1/x*x/1") { // (5/x) * (x/2)
				//        return mul(div(t1->t1,t2->t2), div(t1->t2,t2->t1));
				//    }
				//    else if (hash == "x/1*1/x") { // (x/5) * (2/x)
				//        return mul(div(t1->t1,t2->t2), div(t1->t2,t2->t1));
				//    }
				//    else if (hash == "1/x*x*1") { // (5/x) * (x*4)
				//        return mul(mul(t1->t1,t2->t2), div(t1->t2,t2->t1));
				//    }
				//
				//    else if (hash == "1/x*x^1") { // (5/x) * (x^3)
				//                                  //return mul(t1->t1, exp( t2->t1, sub(t2->t2, new node("1")) );
				//    }
				//
				//    else if (hash == "1*x*1*x") { // (5*x) * (4*x)
				//        return mul(mul(t1->t1,t2->t1), mul(t1->t2,t2->t2));
				//    }
				//    else if (hash == "1*x*x*1") { // (5*x) * (x*4)
				//        return mul(mul(t1->t1,t2->t2), mul(t1->t2,t2->t1));
				//    }
				//
				//    else if (hash == "1*x*1/x") { // (3*x) * (2/x)
				//        return mul(mul(t1->t1,t2->t1), div(t1->t2,t2->t2)) ;
				//    }
				//    else if (hash == "x*1*1/x") {
				//        return mul(mul(t1->t2,t2->t1), div(t1->t1,t2->t2));
				//    }

				if (hash == "x^1*1/x") {
						if (a->t1->sym == b->t2->sym) {
								return mul(exp(a->t1, sub(a->t2,new node(1.0))), b->t1);
						}
				} else if (hash == "x^1/x*0") {
						if (a->t1->sym == b->sym) {
								return exp(a->t1, sub(a->t2,new node(1.0)));
						}
				} else if (hash == "x^x/x*0") {
            if (a->t1->sym == b->sym) {
                return exp(a->t1, sub(a->t2, new node(1.0)));
            }
        } else if (hash == "x*0/x^x") {
            if (a->sym == b->t1->sym) {
                // cout << a->sym << "\n";
                return exp(a, sub(new node(1.0), a) );
            }
        }
		}

		return curr;
}

int main () {

		// (5) * (x^3)   => [1*0*x^0]
		// (5) * (x / 2) => [1*0*x/1]
		// (x*5) * (x*4) => [x*1*x*1] :
		//
		// (x^3) / (x)   => [x^1/x*0] : exp(t1->t1,sub(t1->t2,new node("1")))
		// (x^3) / (2*x) => [x^1/1*x] : div(exp(t1->t1, sub(t1->t2,new node("1"))),t2->t2)

		node *tree = div(
      exp( new node("x"), new node("x")),
      new node("x")
  ) ;

		tree = simplify(tree);
		cout << evaluate(tree) << "\n";


		delete tree;
		cout << "\nend\n";
}
