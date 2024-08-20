#include <iostream>
#include <string>
#include <vector>
#include <complex>
#include <cmath>

using namespace std;

using value_t = complex<double>;

void show (const vector<string> &vs) {

    for (int i = 0; i < vs.size(); i++) {
        cout << "[" << vs[i] << "]";
    }
    cout << endl;
}

bool isnum (const string &input) {

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

string getstack(vector<string> &stack) {
    string val = stack.back();
    stack.pop_back();
    return val;
}
string parse(const string &input) {

    if (input == "x") return "1"; // constant
    if (isnum(input)) return "0"; // variable

    int index = 0;
    vector<string> code = tokenize(input);
    vector<string> vars, oper;
    show(code);

    while (index < code.size()) {
        if (code[index] == "x") {
            vars.push_back(code[index]);
        } else if (isnum(code[index])) {
            vars.push_back(code[index]);
        } else if (is_operator(code[index])) {
            oper.push_back(code[index]);
        } else if (isfunc(code[index])) {

        } else {

        }

        index++;
    }

    return vars.back();
}


struct Node {
    string token, type;
    Node *t1, *t2;

    Node (const string &label = "") : token (label), t1 (nullptr), t2 (nullptr) {}
    Node (const string &label, Node *arg1, Node *arg2) : token (label), t1 (arg1), t2 (arg2) {}
};
void shownode (const Node *now) {
    cout << now->token << " ";
    /*
    if (oper[now->op]) {
        cout << "Bin(" << now->op << ", ";
    } else {
        cout << "Arg(" << now->op << "," << now->n;
    }
    */
}
void showtree (const Node *now) {
    if (now != nullptr) {
        shownode (now);
        showtree (now->t1);
        showtree (now->t2);
        cout << ")";
    }
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

    vector<string> vars {"2","x","3"}, oper = {"*","^"};

    vector<Node*> tree;

    if (!oper.empty()) {
        string t2 = getstack(vars), t1 = getstack(vars), op = getstack(oper);
        string type;
        Node *curr = new Node(getstack(oper), new Node(t1), new Node(t1));

        if (op == "+" || op == "-") {
          type = "term";
        } else if (op == "*" || op == "/") {
          type = "factor";
        } else {
          type = "basic";
        }
    }

}
