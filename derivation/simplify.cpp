#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct node {
    string sym;
    node *t1, *t2;

    node (const string &label = "", node *t1 = nullptr, node *t2 = nullptr) : sym (label), t1 (t1), t2 (t2) {}

};

string evaluate (node *curr) {

    if (curr != nullptr) {
        string symbol = curr->sym;
        string a = evaluate(curr->t1), b = evaluate(curr->t2);
        //cout << "[" << a << "](" << symbol << ")[" << b << "]\n";
        return a + symbol + b;
    }

    return "";
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

node *add(node *a, node *b) {

    // if (a->sym == b->sym && !is_operator(a->sym)) return new node ("*",new node("2"), a);
    if (a->sym == "0") return b;
    if (b->sym == "0") return a;
    if (is_number(a->sym) && is_number(b->sym)) return new node(to_string(stoi(a->sym) + stoi(b->sym)));

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
node *mul(node *a, node *b) {

    // if (a->sym == b->sym) return new node ("*",a, b);
    if (a->sym == "0" || b->sym == "0") return new node ("0");
    if (a->sym == "1") return b;
    if (b->sym == "1") return a;

    return new node ("*",a,b);
}
node *div(node *a, node *b) {

    //if (a->sym == b->sym) return new node ("1");
    if (a->sym == "0") return new node ("0");
    if (b->sym == "1") return a;
    if (is_number(a->sym) && is_number(b->sym)) return new node(to_string(stoi(a->sym) / stoi(b->sym)));

    return new node ("/",a,b);
}

void showvect(const vector<string> &ve) {
    for (auto &it : ve) {
        cout << "[" << it << "]";
    }
    cout << endl;
}
string get(node *curr) {
  return curr == nullptr ? "" : curr->sym;
}

string gethash(node *curr) {
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

void bak() {
  // node *curr = tree->t2;
  // node *t1 = curr->t1, *t2 = curr->t2;
  //
  // if (curr->sym == "/") {
  //     if(t1->sym == "^") {
  //         if (t1->t1->sym == t2->sym) { // (x^4) / (x)
  //             *curr = *exp(t1->t1, sub(t1->t2,new node("1")) );
  //         }
  //     }
  // } else if (curr->sym == "*") {
  //     node *a = curr->t1, *b = curr->t2;
  //
  //     if (is_number(t1->sym)) { // (5) * (_)
  //         if (is_number(t2->sym)) { // (5) * (3)
  //             *curr = *new node(to_string(stoi(a->sym) * stoi(b->sym)));
  //         } else if (t2->sym == "*") {        // (5) * (4*x)
  //             if (is_number(t2->t1->sym)) {
  //                 *curr = *mul(mul(t1, t2->t1), t2->t2);
  //             } else if (t2->sym == "/") {    // (5) * (_/_)
  //
  //             }
  //         }
  //     }
  //
  //     if (a->sym == "/" && b->sym == "^") {
  //         node *num = a->t1, *den = a->t2, *base = b->t1, *ex = b->t2;
  //
  //         if (den->sym == base->sym) {
  //             *a = *num;
  //             *b = *exp(base, sub(ex,new node("1")));
  //         }
  //     }
  // }

}

node *simplify (node *curr) {

    string hash = gethash(curr->t1) + curr->sym + gethash(curr->t2);
    node *t1 = curr->t1, *t2 = curr->t2;

    if (hash == "1*0*x*1") { // (5) * (x * 3) => [1*0*x*1]
        return mul(mul(t1, t2->t2), t2->t1);
    } else if (hash == "1*0*1*x") { // (5) * (3 * x) => [1*0*1*x]
        return mul(mul(t1, t2->t1), t2->t2);
    } else if (hash == "1*0*1/x") { // (5) * (4 / x) => [1*0*1/x]
        return div(mul(t1, t2->t1), t2->t2);
    }

    else if (hash == "1/x*x/1") { // (5/x) * (x/2)
        return div(mul(t1->t1, t2->t2),mul(t1->t2,t2->t1));
    }
    else if (hash == "x/1*1/x") { // (x/5) * (2/x)
        return div(mul(t1->t2, t2->t1),mul(t1->t1,t2->t2));
    }
    else if (hash == "1/x*x^1") { // (5/x) * (x^3)

    }
    return curr;
}

int main () {

// (5) * (x^3)   => [1*0*x^0]
// (5) * (x / 2) => [1*0*x/1]
//
// (5) * (x * 3) => [1*0*x*1] : mul(mul(t1, t2->t2), t2->t1)
// (5) * (3 * x) => [1*0*1*x] : mul(mul(t1, t2->t1), t2->t2)
// (5) * (4 / x) => [1*0*1/x] : div(mul(t1, t2->t1), t2->t2)
//
// (5/x) * (x/2) => [1/x*x/1] : mul(t1->t1, t2->t2)
// (x/5) * (2/x) => [x/1*1/x] : mul(t1->t2, t2->t1)
// (5/x) * (x^3) => [1/x*x^1] : mul(t1->t1, exp( t2->t1, sub(t2->t2, new node("0")) )
//
// (x*5) * (x*4) => [x*1*x*0] :
// (5*x) * (4*x) => [1*x*1*x] : mul(mul(t1->t1,t2->t1), mul(t1->t2,t2->t2))
// (5*x) * (x*4) => [1*x*x*1] : mul(mul(t1->t1,t2->t2), mul(t1->t2,t2->t1))
//
// (x^3) / (x)   => [x^1/x*0] : exp(t1->t1,sub(t1->t2,new node("1")))
// (x^3) / (2*x) => [x^1/1*x] : div(exp(t1->t1, sub(t1->t2,new node("1"))),t2->t2)
// (3*x) * (2/x) => [1*x*1/x] : mul(t1->t1,t2->t1)

    node *tree = mul(new node("2"),
            mul(
                exp(new node("x"),new node("3")),
                div(new node("3"),new node("x"))
               )
            ) ;


    node *curr = tree->t2;
    string hash = gethash(curr->t1) + curr->sym + gethash(curr->t2);


    // cout << evaluate(tree) << "\n";


    //   vector<string> l2 = {get(curr->t1->t1), get(curr->t1->t2), get(curr->t2->t1), get(curr->t2->t2)};

    delete tree;

    cout << "\nend\n";
}
