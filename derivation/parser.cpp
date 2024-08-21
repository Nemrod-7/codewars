#include <iostream>
#include <string>
#include <vector>
#include <complex>
#include <cmath>

using namespace std;

using value_t = complex<double>;

struct Node {
    string token;
    Node *t1, *t2;

    Node (const string &label = "") : token (label), t1 (nullptr), t2 (nullptr) {}
    Node (const string &label, Node *arg1, Node *arg2) : token (label), t1 (arg1), t2 (arg2) {}
};
void show (const vector<string> &vs) {

    for (int i = 0; i < vs.size(); i++) {
        cout << "[" << vs[i] << "]";
    }
    cout << endl;
}

bool isnum (const string &input) {

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
bool isfunc (const string &input) {
    return input == "sin" || input == "cos" || input == "tan" || input == "log" || input == "cot";
}
bool is_operator(const string &input) {
    return input == "+" || input == "-" || input == "*" || input == "^" || input == "/";
}
int order (const string &src) {
    if (src == "+" || src == "-") return 1;
    if (src == "*" || src == "/") return 2;
    if (src == "^") return 3;
    return 0;

}

string parenthesis(const string &code) {
    int i = 1, pile = 1;
    string buffer;

    while (true) {
        if (code[i] == '(') pile++;
        if (code[i] == ')') pile--;
        if (pile == 0) return buffer;
        buffer += code[i++];
    }

    return "";
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
        } else if (input[i] == '+' || input[i] == '-') {
            code.push_back(string(1,input[i++]));
        } else if (input[i] == '*' || input[i] == '/') {
            code.push_back(string(1,input[i++]));
        } else if (input[i] == '^') {
            code.push_back(string(1,input[i++]));
        } else if (isalpha(input[i])) {
            string buffer;

            while (isalpha(input[i])) buffer += input[i++];
            code.push_back(buffer);
        } else if (input[i] == '(') {
            string cell = parenthesis(input.substr(i));
            i += cell.size() + 2;

            code.push_back(cell);
        }

        else {
            code.push_back(string(1,input[i++]));
        }
    }

    return code;
}

template<class T> T getstack(vector<T> &stack) {
    T val = stack.back();
    stack.pop_back();
    return val;
}

void shownode (const Node *now) {
    cout << "(" << now->token;
}
void showtree (const Node *now) {
    if (now != nullptr) {
        shownode (now);
        showtree (now->t1);
        showtree (now->t2);
        cout << ")";
    }
}

Node *parse(const string &input) {

    int index = 0;
    vector<string> code = tokenize(input);
    vector<string> oper;
    vector<Node*> tree;
//    show(code);

    while (index < code.size()) {
        if (code[index] == "x") {
            tree.push_back( new Node(code[index]));
        } else if (isnum(code[index])) {
            tree.push_back( new Node(code[index]));
        } else if (is_operator(code[index])) {

            while(!oper.empty() && order(oper.back()) > order(code[index])) {
                Node *next = new Node(getstack(oper));
                next->t2 = getstack(tree);
                next->t1 = getstack(tree);
                tree.push_back(next);
            }

            oper.push_back(code[index]);
        } else if (isfunc(code[index])) {
            string token = code[index] + " " + code[index+1];
            tree.push_back( new Node(token));
            index++;
        } else {

        }

        index++;
    }

    while(!oper.empty()) {
        Node *next = new Node(getstack(oper));
        next->t2 = getstack(tree);
        next->t1 = getstack(tree);
        tree.push_back(next);
    }

    //showtree(tree.back());
    return tree.back();
}

string shownode(Node *node) {

    if (node != nullptr) {
        string term = node->token;
        string t1 = shownode(node->t1), t2 = shownode(node->t2);
        return t1 + term +  t2;
    }

    return "";
}

std::string operate(std::string a, std::string op, std::string b) {

    if (op == "+") {
        if (a == b) { op = "*", a = "2"; }
        if (a == "0") return b;
        if (b == "0") return a;
    } else if (op == "-") {
        if (a == b) return "0";
        if (b == "0") return a;
    } else if (op == "*") {
        if (a == b) { op = "^", b = "2"; }
        if (a == "0" || b == "0") return "0";
        if (a == "1") return b;
        if (b == "1") return a;
    } else if (op == "/") {
        if (a == b) return "1";
        if (a == "0") return "0";
        if (b == "1") return a;
    } else if (op == "^") {
        if (a == "1" || b == "1") return a;
        if (b == "0") return "1";
        if (a == "0") return "0";
    }

    return a + op + b;
}

std::string evaluate (Node *node, std::string value = "") {

    if (node != nullptr) {
        string term = node->token;
        string t1 = evaluate(node->t1), t2 = evaluate(node->t2);

        if (term == "x") {

        } else if (is_operator(term)) {
            if (isnum(t1) && isnum(t2)) {
                int z1 = stoi(t1), z2 = stoi(t2), result = 0;

                switch (term[0]) {
                    case '+' : result = (z1 + z2) ; break;
                    case '-' : result = (z1 - z2) ; break;
                    case '*' : result = (z1 * z2) ; break;
                    case '/' : result = (z1 / z2) ; break;
                }

                return to_string(result);
            }
        } else if (isfunc(term)) {

        }

        return t1 + term + t2;
    }

    return "";
}
string derivate (Node *node) {

    if (node != nullptr) {
        string term = node->token;

        if (is_operator(term)) {

            if (term == "+") { // a' + b'
                string t1 = derivate(node->t1), t2 = derivate(node->t2);
                string result = operate(t1,term,t2);    
                /*cout << "[" << t1 << "]" << term << "[" << t2 << "] => " << result << "\n";*/
                return result;
            } else if (term == "-") { // a' - b'
                string t1 = derivate(node->t1), t2 = derivate(node->t2);
                string result = operate(t1,term,t2);    
                /*cout << "[" << t1 << "]" << term << "[" << t2 << "] => " << result << "\n";*/
                return result;
            } else if (term == "*") { // a'.b + a.b'
                string t1 = evaluate(node->t1), t2 = evaluate(node->t2);
                string d1 = derivate(node->t1), d2 = derivate(node->t2);
                string result;

                //cout << "[" << t1 << "]" << term << "[" << t2 << "] => " << result << "\n";
                return operate(operate(d1,"*",t2),"+",operate(t1,"*",d2));
            } else if (term == "/") { // (a'.b - a.b') / (b.b)
                string t1 = evaluate(node->t1), t2 = evaluate(node->t2);
                string d1 = derivate(node->t1), d2 = derivate(node->t2);
                
                string a1 = operate(operate(d1,"*",t2),"-",operate(t1,"*",d2));
                string b1 = operate(t2,"*",t2);

                return operate(a1,"/",b1);
            } else if (term == "^") {
                string t1 = evaluate(node->t1), t2 = evaluate(node->t2);
                //string d1 = derivate(node->t1), d2 = derivate(node->t2);

                if (t2 == "x") { //  exp : a^x   => a^x . ln (a)
                    std::string arg1 = operate(t1,"^",t2);
                    std::string arg2 = "log(" + t1 + ")";
                    std::string arg3 = operate(arg1,"*",arg2);

                    return (arg3);
                }

                if (t1 == "x") { //  pow : x^a   => a.x^(a - 1)
                    std::string arg1 = operate(t2, "-", "1");
                    std::string arg2 = operate(t1,"^",arg1);
                    std::string arg3 = operate(t2, "*", arg2);

                    return (arg3);
                }
            }

        } else if (term == "x")  {
            return "1";
        } else if (isnum(term)) {
            return "0";
        } else {

        }
    }

    return "";
}

int main () {

    //auto func1 = differentiate("2 * x^3");
    //  <variable>  ::= "x"
    //  <constant>  ::= [0-9]+(.[0-9]+)?
    //  <func_name> ::= "sin" | "cos" | "tan" | "cot" | "log"
    //
    //  <factor>     ::= <basic> ( "^" <basic> )*
    //  <expression> ::= <term> ( "+" | "-"  <term> )*
    //  <term>       ::= <factor> ( "*" | "/"  <factor> )*
    //
    //  <func_call>  ::= <func_name> "(" <expression> ")"
    //  <basic>      ::= <constant> | <variable> | <func_call> | ( "(" <expression> ")" )


    string expression = "2 * x^3";

    //expression = "2 * x^3";
    Node *node = parse(expression);
    //string result = derivate(node);
    cout << derivate(parse(expression)) << "\n ";

    cout << evaluate(node->t1);

    /*cout << "\nresult : " << result << "\n";*/
    //showtree(node);

}
