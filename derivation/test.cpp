
#include <iostream>
#include <string>
#include <vector>
// #include <regex>

using namespace std;

// vector<string> tokenize1 (const string &expr) {
//     // show(expression);// ("\\s* ( => | [-+*%=\(\)] | [A-Za-z_] [A-Za-z0-9_]* | [0-9]*\.?[0-9]+ ) \\s*");
//     // regex re ("=>|_?[0-9]+(\\.[0-9]+)?|[-+*/%()=\\[\\]]|_?[a-zA-Z]+_?");
//     regex re ("=>|(-)?[0-9]+(\\.[0-9]+)?|[-+*%=/\\(\\)]|[A-Za-z_][A-Za-z0-9_]*");
//
//     sregex_token_iterator it (expr.begin(), expr.end(), re);
//     return vector<string> (it, sregex_token_iterator());
// }

bool is_term (const string &src) { return src == "+" || src == "-"; }
bool is_fact (const string &src) { return src == "*" || src == "/"; }
bool is_func (const string &input) { return input == "sin" || input == "cos" || input == "tan" || input == "log" || input == "cot"; }
bool is_oper (const string &input) { return is_fact(input) || is_term(input) || input == "^"; }
bool is_nume (const string &input) {
    if (input.size() == 0) return false;

    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] == '-' && isdigit(input[i+1])) continue;
        if (input[i] != '.' && input[i] != ',' && !isdigit(input[i])) {
            return false;
        }
    }

    return true;
}
vector<string> tokenize2 (const string &input) {
    vector<string> code;
    size_t i = 0;

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
            if (code.size() == 0 || is_oper(code.back())) {
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


void show(const vector<string> &vs) {
    for (auto &it : vs) {
        cout << "[" << it <<"]";
    }
    cout << "\n";
}
int main () {


    show(tokenize2("1 - 5"));
    show(tokenize2("1+-5"));

}
