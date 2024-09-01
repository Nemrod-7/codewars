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

node *div(node *a, node *b) {

    //if (a->sym == b->sym) return new node ("1");
    if (a->sym == "0") return new node ("0");
    if (b->sym == "1") return a;
    if (is_number(a->sym) && is_number(b->sym)) return new node(to_string(stoi(a->sym) / stoi(b->sym)));

    return new node ("/",a,b);
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
node *mul(node *a, node *b) {

    // if (a->sym == b->sym) return new node ("*",a, b);
    if (a->sym == "0" || b->sym == "0") return new node ("0");
    if (a->sym == "1") return b;
    if (b->sym == "1") return a;
    if (is_number(a->sym) && is_number(b->sym)) return new node(to_string(stoi(a->sym) * stoi(b->sym)));

    return new node ("*",a,b);
}
node *exp(node *a, node *b) {

    if (a->sym == "1" || b->sym == "1") return a;
    if (b->sym == "0") return new node("1");
    if (a->sym == "0") return new node("0");
    if (is_number(a->sym) && is_number(b->sym)) return new node(to_string(pow(stoi(a->sym), stoi(b->sym))));

    return new node("^",a,b);
}

node *simplify_proto(node *curr) {

    if (curr != nullptr) {
        node *a = simplify_proto(curr->t1), *b = simplify_proto(curr->t2);

        if (curr->sym == "*") {

            if (is_number(a->sym)) {
                if (b->sym == "*") {
                    //cout << a->sym << " " << b->t1->sym << "\n";
                    if (is_number(b->t1->sym)) {                    
                        *curr = *new node("*", mul(a, b->t1), b->t2);
                    }
                }
            } else if (a->sym == "^") {
                node *base = a->t1, *ex = a->t2;

                if (b->sym == "/") {
                    node *num = b->t1, *den = b->t2;

                    if (den->sym == base->sym) {
                        *ex = *sub(ex,new node("1"));
                        *b = *num;
                        swap(*a,*b);
                    }
                }
            } else if (a->sym == "/") {
                node *num = a->t1, *den = a->t2;

                if (b->sym == "^") {
                    node *base = b->t1, *ex = b->t2;

                    if (den->sym == base->sym) {
                        *ex = *sub(ex,new node("1"));
                        *a = *num;
                    }
                }
            } 

        } else if (curr->sym == "/") {

            if (a->sym == "^") { 
                node *base = a->t1, *ex = a->t2;

                if (b->sym == base->sym) {
                    *ex = *sub(ex,new node("1"));
                } else if (b->sym == "/") {
                    node *num = b->t1, *den = b->t2;

                    if (num->sym == base->sym) { // ex: (x^4) / (5/x)
                        *ex = *sub(ex,new node("1"));
                        *b = *den;
                        swap(*a,*b);
                        curr->sym = "*";
                    } 
                } 
            } else if (b->sym == "^") {
                node *base = a->t1, *ex = a->t2;

                if (a->sym == base->sym) {
                    *ex = *sub(ex,new node("1"));
                }
            }

        }
    }

    return curr;
}
void showvect(const vector<string> &ve) {
    for (auto &it : ve) {
        cout << "[" << it << "]";
    }
    cout << endl;
}

void simplify(node *curr) {

    if (curr != nullptr) {
        simplify(curr->t1), simplify(curr->t2);

        if (curr->sym == "*") {
            node *a = curr->t1, *b = curr->t2; 

            if (is_number(a->sym)) {
                if (b->sym == "*" && is_number(b->t1->sym)) {                    
                    *curr = *new node("*", mul(a, b->t1), b->t2);
                }

            } else if (a->sym == "/" && b->sym == "^") {
                node *num = a->t1, *den = a->t2, *base = b->t1, *ex = b->t2;

                if (den->sym == base->sym) {
                    *a = *num;
                    *b = *exp(base, sub(ex,new node("1")));
                }
            }
        } 
    }
}

int main () {

    node *tree = new node("*", new node("2"),
            new node("*",
                new node("/", new node("3"), new node("x")),
                new node("^", new node("x"), new node("3"))
                )) ;

    //vector<pair<string,string>> args;
    //cout << getarg(t2);


    simplify(tree);
    // showvect(num);
    // showvect(den);


    cout << evaluate(tree) << "\n";

    cout << "\nend\n";
}
