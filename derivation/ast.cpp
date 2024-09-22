#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <complex>

enum struct type {empty,cst,num,add,sub,mul,div,exp,cos,sin,tan,cot,log };

struct ast {
		type id;
		std::complex<double> val;
		std::shared_ptr<ast> t1, t2;

		ast(type src, std::shared_ptr<ast> a = nullptr, std::shared_ptr<ast> b = nullptr) : id(src), val(0.0), t1 (a), t2 (b) {}
		ast(std::complex<double> src, std::shared_ptr<ast> a = nullptr, std::shared_ptr<ast> b = nullptr) : id(type::num), val(src), t1 (a), t2 (b) {}
		// ~ast() { std::cout << "Instance died" << std::endl; }
};

using namespace std;
using node = shared_ptr<ast>;


int order (const type &src) {
    if (src == type::add || src == type::sub) return 1;
    if (src == type::mul || src == type::div) return 2;
    if (src == type::exp) return 3;
    return 0;
}
complex<double> stoc (const string &input) {
    istringstream iss(input);
    complex<double> zx;
    iss >> zx;
    return zx;
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
node getstack (vector<node> &stack) {
    node val = stack.back();
    stack.pop_back();
    return val;
}
bool precedence (const vector<node> &stack, const type &cell) {
    if (stack.empty()) return false;
    if (cell == type::exp) return order(stack.back()->id) > order(cell);
    return order(stack.back()->id) >= order(cell);
}
node parse (const string &input) {

	vector<string> code = {"2","*","x","^","3"};

	vector<string>::iterator it = code.begin();
	vector<shared_ptr<ast>> tree, oper;

	map<string,type> term {{"+", type::add},{"-",type::sub}};
	map<string,type> fact {{"*", type::mul},{"/",type::div},{"^",type::exp}};
	map<string,type> func {{"cos", type::cos},{"sin",type::sin},{"tan",type::tan},{"log",type::log},{"cot",type::cot}};

	while (it < code.end()) {
			string curr = *it;

			if (curr == "x") {
					tree.push_back( make_shared<ast>(type::cst));
			} else if (curr == "(") {
					// tree.push_back(parse(parenthesis(it)));
			}

			else if (is_number(curr)) {
					tree.push_back( make_shared<ast>(stoc(curr)));
			} else if (fact[curr] != type::empty) {
					while (!oper.empty() && precedence(oper,fact[curr])) {
						node next = getstack(oper);
						next->t2 = getstack(tree), next->t1 = getstack(tree);
						tree.push_back(next);
					}

					oper.push_back( make_shared<ast>(fact[curr]));
			} else if (func[curr] != type::empty) {
					it++;
					node next = make_shared<ast>(func[curr]);
					// next->t1 = parse(parenthesis(it));
					tree.push_back(next);
			}

			else {
					cout << "Invalid identifier\n";
			}

			it++;
	}

	while (!oper.empty()) {
			node next = getstack(oper);
			next->t2 = getstack(tree), next->t1 = getstack(tree);
			tree.push_back(next);
	}

	return tree.back();
}
int main() {


		// shared_ptr<node> t1 = make_shared<node>("*", new node(2.0),make_shared<node>(3.0));
		// make_shared<node>
		// new node
  	std::cout << "\nexit\n" << std::endl;
}
