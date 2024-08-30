#include <iostream>

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

void simplify(node *curr, const string &cell, string oper) {

		if (curr != nullptr) {
				string symbol = curr->sym;

				if (symbol == "*") {

						//cout << "[" << a << "](" << symbol << ")[" << b << "]\n";
				}


				simplify(curr->t1, cell, oper);
				simplify(curr->t1, cell, oper);
		}
}


int main () {

		node *code = new node("*", new node("2"),
						new node ("*",
								new node("/", new node("3"), new node("x")),
								new node("^", new node("x"), new node("3"))
								)
						);




		string search = "2";
		
		node *curr = code;
		node *t1 = curr->t2, *t2 = curr->t2;
		
		if (curr->sym == "*") {

				if (t2->sym == "^") {
						node *base = t2->t1, *exp = t2->t2;

						if (t1->sym == "/") {
								node *num = t1->t1, *den = t2->t2;
						}
				}
		}

		//evaluate(code);

		//delete code;
		cout << "end";
}
