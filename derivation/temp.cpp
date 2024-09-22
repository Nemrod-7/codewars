#include <iostream>
#include <memory>
#include <complex>
#include <functional>

struct ast {
		std::string id;
		std::complex<double> val;
		std::shared_ptr<ast> t1, t2;

		ast(const std::string &src, std::shared_ptr<ast> a = nullptr, std::shared_ptr<ast> b = nullptr) : id(src), val(0.0), t1 (a), t2 (b) {}
		ast(std::complex<double> src, std::shared_ptr<ast> a = nullptr, std::shared_ptr<ast> b = nullptr) : id(""), val(src), t1 (a), t2 (b) {}
		~ast() { std::cout << "Instance ast died" << std::endl; }
};

using namespace std;
using value_t = complex<double>;
using func_t = function<value_t(value_t)>;
using node = shared_ptr<ast>;

node add(node a, node b) {

    if (a->id == "" && a->val == 0.0) return b;
    if (b->id == "" && b->val == 0.0) return a;
    if (a->id == "" && b->id == "") return make_shared<ast>(a->val + b->val);

    return make_shared<ast> ("+",a,b);
}
node sub(node a, node b) {

    if (b->id == "" && b->val == 0.0) return a;
    if (a->id == "" && b->id == "") return make_shared<ast>(a->val - b->val);
    return make_shared<ast> ("-",a,b);
}
node exp(node a, node b) {

    if (a->val == 1.0 || b->val == 1.0) return a;
    if (b->id == "" && b->val == 0.0) return make_shared<ast>(1.0);
    if (a->id == "" && b->id == "") return make_shared<ast>(pow(a->val , b->val));

    return make_shared<ast>("^",a,b);
}
node div(node a, node b) {

    if (a->id == "" && a->val == 0.0) return make_shared<ast>(0.0);
    if (b->id == "" && b->val == 1.0) return a;
    if (a->id == "" && b->id == "") return make_shared<ast>(a->val / b->val);

    return make_shared<ast> ("/",a,b);
}
node mul(node a, node b) {

    if (a->val == 1.0) return b;
    if (b->val == 1.0) return a;
    if ((a->id == "" && a->val == 0.0) || (b->id == "" && b->val == 0.0)) return make_shared<ast> (0.0);
    if (a->id == "" && b->id == "") return make_shared<ast>(a->val * b->val);

    return make_shared<ast> ("*",a,b);
}

complex<double> stoc (const string &input) {
    istringstream iss(input);
    complex<double> zx;
    iss >> zx;
    return zx;
}
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
bool is_func (const string &input) {
    return input == "sin" || input == "cos" || input == "tan" || input == "log" || input == "cot";
}

node getstack (vector<node> &stack) {
    node val = stack.back();
    stack.pop_back();
    return val;
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
vector<string> tokenize (const string &input) {

    vector<string> code;
    int i = 0;

    while (i < input.size()) {

        if (isdigit(input[i])) {
            string buffer;

            while (isdigit(input[i]) || input[i] == '.') buffer += input[i++];
            code.push_back(buffer);
        } else if (isspace(input[i])) {
            while (isspace(input[i])) i++;
        } else if (input[i] == '+') {
            code.push_back(string(1,input[i++]));
        } else if (input[i] == '*' || input[i] == '/') {
            code.push_back(string(1,input[i++]));
        } else if (input[i] == '^') {
            code.push_back(string(1,input[i++]));
        } else if (isalpha(input[i])) {
            string buffer;

            while (isalpha(input[i])) buffer += input[i++];
            code.push_back(buffer);
        } else if (input[i] == '-') {
            if (code.size() == 0 || is_operator(code.back())) {
                string buffer = "-";
                i++;

                while (isdigit(input[i]) || input[i] == '.') buffer += input[i++];
                code.push_back(buffer);
            } else {
                code.push_back(string(1,input[i++]));
            }
        } else {
            code.push_back(string(1,input[i++]));
        }
    }

    return code;
}
int order (const string &src) {
    if (src == "+" || src == "-") return 1;
    if (src == "*" || src == "/") return 2;
    if (src == "^") return 3;
    return 0;
}

bool precedence (const vector<node> &stack, const string &cell) {
    if (stack.empty()) return false;
    if (cell == "^") return order(stack.back()->id) > order(cell);
    return order(stack.back()->id) >= order(cell);
}
node parse (const string &input) {

	vector<string> code = tokenize(input);
	vector<string>::iterator it = code.begin();
	vector<node> tree, oper;

	while (it < code.end()) {
			string curr = *it;

			if (curr == "x") {
					tree.push_back( make_shared<ast>(curr));
			} else if (curr == "(") {
					tree.push_back(parse(parenthesis(it)));
			} else if (is_number(curr)) {
					tree.push_back( make_shared<ast>(stoc(curr)));
			} else if (is_operator(curr)) {
					while (!oper.empty() && precedence(oper,curr)) {
						node next = getstack(oper);
						next->t2 = getstack(tree), next->t1 = getstack(tree);
						tree.push_back(next);
					}

					oper.push_back( make_shared<ast>(curr));
			} else if (is_func(curr)) {
					it++;
					node next = make_shared<ast>(curr);
					next->t1 = parse(parenthesis(it));
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

void showtree(const node node, bool isLeft = false, const string &prefix = "") {
    if (node != nullptr) {
        cout << prefix;
        cout << (isLeft ? "├─" : "└─" );

        if (node->id == "") {
            cout << "[" << node->val << "]" << endl;
        } else {
            cout << "[" << node->id << "]" << endl;
        }

        showtree(node->t1, true, prefix + (isLeft ? "│  " : "   "));
        showtree(node->t2, false, prefix + (isLeft ? "│  " : "   "));
    }
}
int main () {

    // exp : x^y   => x^y . (x'.(y/x) + y'.log(x))
    // x^63 => x^63 * (63/x)) => 63 * x^62

    complex<double> x(-3.35,3.35);
    // ast *t1 = mul(new ast(63.0), exp(new ast(x), new ast(62.0)));
    // ast *t2 = mul( exp(new ast(x), new ast(63.0)), div(new ast(63.0),  new ast(x)) );

    // complex<double> x1 = pow(x, 3);
    // complex<double> x2 = pow(x, 2);
    // complex<double> ex = {-16, 16} ; // pow(x, 3.0);
    // ex= pow(x, 3.0);
    //    cout << fixed;
    //
    //    cout << pow(x, 62.0) * 63.0 << "\n";
    //    cout << pow(x, 63.0) * 63.0 / x << "\n";
    //    cout << pow(x, 63.0) / x * 63.0 << "\n";


    // shared_ptr<ast> tree =



}
