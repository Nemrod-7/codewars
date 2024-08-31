#include <iostream>
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

				cout << "[" << a << "](" << symbol << ")[" << b << "]\n";

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

    if (is_number(a->sym) && is_number(b->sym)) return new node(to_string(stoi(a->sym) / stoi(b->sym)));
    if (a->sym == b->sym) return new node ("1");
    if (a->sym == "0") return new node ("0");
    if (b->sym == "1") return a;

    return new node ("/",a,b);
}
node *add(node *a, node *b) {

    if (is_number(a->sym) && is_number(b->sym)) return new node(to_string(stoi(a->sym) + stoi(b->sym)));
    if (a->sym == b->sym) return new node ("*",new node("2"),b);
    if (a->sym == "0") return b;
    if (b->sym == "0") return a;

    return new node ("+",a,b);
}
node *sub(node *a, node *b) {

    if (is_number(a->sym) && is_number(b->sym)) return new node(to_string(stoi(a->sym) - stoi(b->sym)));

    if (a->sym == b->sym) return new node("0");
    if (b->sym == "0") return a;

    return new node ("-",a,b);
}
node *mul(node *a, node *b) {

    if (is_number(a->sym) && is_number(b->sym)) return new node(to_string(stoi(a->sym) * stoi(b->sym)));
    if (a->sym == b->sym) return new node ("^",a, new node("2"));
    if (a->sym == "0" || b->sym == "0") return new node ("0");
    if (a->sym == "1") return b;
    if (b->sym == "1") return a;

    return new node ("*",a,b);
}
node *exp(node *a, node *b) {

    if (is_number(a->sym) && is_number(b->sym)) return new node(to_string(pow(stoi(a->sym), stoi(b->sym))));
    if (a->sym == "1" || b->sym == "1") return a;
    if (b->sym == "0") return new node("1");
    if (a->sym == "0") return new node("0");
    return new node("^",a,b);
}

node *simplify(node *curr) {

		if (curr != nullptr) {
			node *t1 = simplify(curr->t1), *t2 = simplify(curr->t2);

			if (curr->sym == "*") {

				if (is_number(t1->sym)) {
						string mult = t1->sym;

						if (is_number(t2->sym)) {
							// string res = to_string(stoi(t1->sym) * stoi(t2->sym));
						} else if (t2->sym == "*") {
							*curr = *mul(mul(curr->t1, curr->t2->t1), curr->t2->t2);

							// cout << curr->t1->sym << "*" << curr->t2->t1->sym << "||" << next->sym << "\n";
						}
				} else if (is_number(t2->sym)) {
					string mult = t2->sym;

				} else {
					string op1 = t1->sym, op2 = t2->sym;

					if (is_operator(t1->sym) && is_operator(t2->sym)) {
						if (op1 == "/" && op2 == "/") {

						} else if (op1 == "/" && op2 == "*") {

						} else if (op1 == "/" && op2 == "^") {
							node *num = t1->t1, *den = t1->t2, *base = t2->t1, *ex = t2->t2;

							if (den->sym == base->sym) {
								*t1 = *num;
								*t2 = *exp(base, sub(ex, new node("1")));
							}

						}
					}
				}
			}
		}

		return curr;
}

int main () {

		node *code = new node("*", new node("2"),
						new node ("*",
								new node("/", new node("3"), new node("x")),
								new node("^", new node("x"), new node("3"))
								)
						);


	// x*3 * 2 + x
	// x/4 * 2/x
	// 2/x * x^3
	// 2-x * x^3
		node *curr = code;

		if (curr->sym == "*") {
				simplify(curr);
		}

		cout << "\n";
		evaluate(code);

		//delete code;
		cout << "end";
}
