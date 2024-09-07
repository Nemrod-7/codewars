#include <iostream>
#include <vector>
#include <cmath>
#include <tuple>
#include <complex>
#include <functional>
// #include <tests>

using namespace std;
using value_t = complex<double>;
using func_t = function<value_t(value_t)>;

// Let f be a function.
// The derivative function, denoted by f′, is the function whose domain consists of those values of x
// such that the following limit exists:
// f′(x) = lim h→0 of (f(x + h) − f(x)) / h.
//
// con : cst   => 0
// lin : x     => 1
// add : a + b => a' + b'
// min : a - b => a' - b'
// mul : a * b => a.b' + a'.b
// div : a / b => (a'* b − b'* a) / (b * b)
// exp : x^y   => x^y . (x'.(y/x) + y'.log(x))
// log : ln(x) => x' / x
// sin : sin x => cos x
// cos : cos x => -sin x
// tan : tan x => x' / (cos(x))^2
// cot = cot x = -1 / (sin(x))^2

struct node {
    string sym;
    node *t1, *t2;

    node (const string &label = "", node *t1 = nullptr, node *t2 = nullptr) : sym (label), t1 (t1), t2 (t2) {}
};

void showtree(const node *node, bool isLeft = false, const string &prefix = "") {
    if (node != nullptr) {
        cout << prefix;
        cout << (isLeft ? "├─" : "└─" );
        cout << "[" << node->sym << "]" << endl;

        showtree(node->t1, true, prefix + (isLeft ? "│  " : "   "));
        showtree(node->t2, false, prefix + (isLeft ? "│  " : "   "));
    }
}

double round(double x) {
    return floor(x * 1e8) / 1e8;
}
complex<double> round(complex<double> x) {
    return { round(x.real()),round(x.imag()) };
}
complex<double> power(complex<double> x, complex<double> y) {
    return  exp(y * log(x));
}

complex<double> stoc (const string &input) {
    istringstream iss(input);
    complex<double> zx;
    iss >> zx;
    return zx;
}
string ctos(const complex<double> &zx) {
    ostringstream oss;
    zx.imag() == 0 ? oss << zx.real() : oss << zx;
    return oss.str();
}

int order (const string &src) {
    if (src == "+" || src == "-") return 1;
    if (src == "*" || src == "/") return 2;
    if (src == "^") return 3;
    return 0;
}
template<class T> T getstack (vector<T> &stack) {
    T val = stack.back();
    stack.pop_back();
    return val;
}

bool is_term (const string &sym) { return sym == "+" || sym == "-"; }
bool is_fact (const string &sym) { return sym == "*" || sym == "/"; }
bool is_func (const string &input) {
    return input == "sin" || input == "cos" || input == "tan" || input == "log" || input == "cot";
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
bool is_operator (const string &input) {
    return is_fact(input) || is_term(input) || input == "^";
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

string operate (const string &t1, const string &oper, const string &t2) {

    switch (oper[0]) {
        case '+' : return ctos (round(stoc(t1) + stoc(t2))) ; break;
        case '-' : return ctos (round(stoc(t1) - stoc(t2))) ; break;
        case '*' : return ctos (round(stoc(t1) * stoc(t2))) ; break;
        case '/' : return ctos (round(stoc(t1) / stoc(t2))) ; break;
        case '^' : return ctos (round(pow(stoc(t1), stoc(t2)))) ; break;
    }

    return t1 + oper + t2;
}
node *div(node *a, node *b) {

    if (a->sym == "0") return new node ("0");
    if (b->sym == "1") return a;
		if (a->sym == b->sym && !is_operator(a->sym)) return new node ("1");
    if (is_number(a->sym) && is_number(b->sym)) return new node(ctos(round(stoc(a->sym) / stoc(b->sym))));

    return new node ("/",a,b);
}
node *add(node *a, node *b) {

    if (a->sym == "0") return b;
    if (b->sym == "0") return a;
		if (a->sym == b->sym && !is_operator(a->sym)) return new node ("*",new node("2"), a);
    if (is_number(a->sym) && is_number(b->sym)) return new node(ctos(round(stoc(a->sym) + stoc(b->sym))));

    return new node ("+",a,b);
}
node *sub(node *a, node *b) {

    if (b->sym == "0") return a;
		if (a->sym == b->sym && !is_operator(a->sym)) return new node("0");
    if (is_number(a->sym) && is_number(b->sym)) return new node(ctos(round(stoc(a->sym) - stoc(b->sym))));

    return new node ("-",a,b);
}
node *mul(node *a, node *b) {

    if (a->sym == "1") return b;
    if (b->sym == "1") return a;
		if (a->sym == "0" || b->sym == "0") return new node ("0");
		if (a->sym == b->sym && !is_operator(a->sym)) return new node ("^",a, new node("2"));
    if (is_number(a->sym) && is_number(b->sym)) return new node(ctos(round(stoc(a->sym) / stoc(b->sym))));

    return new node ("*",a,b);
}
node *exp(node *a, node *b) {

    if (a->sym == "1" || b->sym == "1") return a;
    if (b->sym == "0") return new node("1");
    if (a->sym == "0") return new node("0");

    if (is_number(a->sym) && is_number(b->sym)) return new node(ctos(round(pow(stoc(a->sym), stoc(b->sym)))));
    return new node("^",a,b);
}

node *parse (const string &input) {

    vector<string> code = tokenize(input), oper;
    vector<string>::iterator it = code.begin();
    vector<node*> tree;

    while (it < code.end()) {
        string cell = *it;

        if (cell == "x") {
            tree.push_back( new node(cell));
        } else if (cell == "(") {
            tree.push_back(parse(parenthesis(it)));
        } else if (is_number(cell)) {
            tree.push_back( new node(cell));
        } else if (is_operator(cell)) {

            while(!oper.empty() && order(oper.back()) > order(cell)) {
                node *next = new node(getstack(oper));
                next->t2 = getstack(tree);
                next->t1 = getstack(tree);
                tree.push_back(next);
            }

            oper.push_back(cell);
        } else if (is_func(cell)) {
            it++;
            node *next = new node(cell);
            next->t1 = parse(parenthesis(it));
            tree.push_back(next);
        } else {
            cout << "invalid identifier\n";
        }

        it++;
    }


    while(!oper.empty()) {
        node *next = new node(getstack(oper));
        next->t2 = getstack(tree);
        next->t1 = getstack(tree);
        tree.push_back(next);
    }

    return tree.back();
}

string evaluate (node *node, string value = "") {

    if (node == nullptr) return "";

    string term = node->sym;
    string t1 = evaluate(node->t1, value), t2 = evaluate(node->t2, value);

    if (term == "x") {
        return value == "" ? term : value;
    } else if (is_operator(term)) {
        // cout << "[" << t1 << "]" << term << "[" << t2 << "]\n";
        if (is_number(t1) && is_number(t2)) {
            return operate(t1,term,t2);
        }
    } else if (is_number(t1)) {
        value_t val = stoc(t1);

        if (term == "cos") {
            return ctos(round(cos(val)));
        } else if (term == "sin") {
            return ctos(round(sin(val)));
        } else if (term == "tan") {
            return ctos(round(tan(val)));
        } else if (term == "log") {
            return ctos(round(log(val)));
        } else if (term == "cot") { //cot(x) = cos(x)/sin(x) or cot(x) = 1 / tan(x)
            return ctos(round( cos(val) / sin(val) ));
        }
    }

    return t1 + term + t2;
}
node *derivate(node *curr) {

    string term = curr->sym;
    node *t1 = curr->t1, *t2 = curr->t2;

    if (term == "x") {
        return new node("1");
    } else if (is_number(term)) {
        return new node("0");
    } else if (term == "+") {
        return add(derivate(t1), derivate(t2));
    } else if (term == "-") {
        return sub(derivate(t1), derivate(t2));
    } else if (term == "*") {
        return add(mul(t1,derivate(t2)), mul(derivate(t1),t2));
    } else if (term == "/") {
        node *num = sub(mul(derivate(t1),t2),mul(t1,derivate(t2)));
        node *den = mul(t2,t2);
        return div(num, den) ;
    } else if (term == "^") {
        if (t1->sym == "x" && is_number(t2->sym)) {
            return mul(t2, exp( t1, sub(t2, new node("1")) ) ) ;
        }
        node *outer = exp(t1, t2);
        node *inner = add( mul( derivate(t1), div(t2,t1) ), mul( derivate(t2), new node("log", t1) ));
        //cout << "[" << evaluate(inner) << "](" << "*" << ")[" << evaluate(outer) << "]\n";
        return mul(inner,outer);
    } else if (term == "cos") {
        return sub(new node("0"), mul(derivate(t1), new node("sin", t1)));
    } else if (term == "sin") {
        return mul(derivate(t1), new node("cos", t1)) ;
    } else if (term == "tan") { // dx = 1 / (cos(x))^2
        return div(derivate(t1), exp(new node("cos", t1), new node("2")) );
    } else if (term == "log") { // dx = x' / x
        return div(derivate(t1),t1);
    } else if (term == "cot") {
        return sub(new node("0"), div(derivate(t1), new node("^", new node("sin", t1), new node("2")))) ;
    }

    return nullptr;
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
								return mul(exp(a->t1, sub(a->t2,new node("1"))), b->t1);
						}
				} else if (hash == "x^1/x*0") {
						if (a->t1->sym == b->sym) {
								return exp(a->t1, sub(a->t2,new node("1")));
						}
				}
		}

		return curr;
}
tuple<func_t,func_t,func_t> differential(const string &expression) {

    node *pass0 = parse(expression);
    node *pass1 = derivate(pass0);
    node *pass2 = derivate(pass1);

    return {
        [pass0](value_t x) { return stoc(evaluate(pass0, ctos(x))); },
            [pass1](value_t x) { return stoc(evaluate(pass1, ctos(x))); },
            [pass0](value_t x) { return stoc(evaluate(pass0, ctos(x))); },
    };
}

int main () {

    string input = "sin(cos(x^x^2))";

    node *pass0 = parse(input);
    node *pass1 = derivate(pass0);
    node *pass2 = derivate(pass1);

    //showtree(pass1);
		//node *simpl = simplify(pass1);

    //showtree(simpl);

   cout << evaluate(pass0,"(1,1)") << "\n"; // EqualsAdaptive(value_t{ 0.839472, -0.0115338 })
   cout << evaluate(pass1,"(1,1)") << "\n"; // EqualsAdaptive(value_t{ 0.0752251, -0.0149614 })
   //cout << evaluate(pass2) << "\n";

    delete pass0;
    delete pass1;
    delete pass2;

    cout << "\nend\n";
}
