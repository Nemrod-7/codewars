#include <iostream>
#include <vector>
#include <string>

using namespace std;

void show (const std::vector<std::string> &vs) {

    for (int i = 0; i < vs.size(); i++) {
        std::cout << "[" << vs[i] << "]";
    }
    std::cout << std::endl;
}

bool isfunc (const std::string &input) {
    return input == "sin" || input == "cos" || input == "tan" || input == "log" || input == "cot";
}
bool isnum (const std::string &input) {

    int i = 0, end = input.size();

    if (input.front() == '(' && input.back() == ')') {
        i += 1, end -= 1;
    }

    for (; i < end; i++) {
        if (input[i] != '.' && input[i] != ',' && !isdigit(input[i]) && input[i] != '-') {
            return false;
        }
    }

    return true;
}
bool is_operator(const string &input) {
    return input == "*" || input == "/" || input == "+" || input == "-" || input == "^";
}
int order (const string &src) {
    if (src == "+" || src == "-") return 1;
    if (src == "*" || src == "/") return 2;
    if (src == "^") return 3;

    return 0;
}
template<class T> T getstack(vector<T> &stack) {
    T val = stack.back();
    stack.pop_back();
    return val;
}

std::string join(const std::vector<std::string> &input) {
    std::string os;
    for (auto &it : input) os += it;
    return os;
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
        } else {
            code.push_back(string(1,input[i++]));
        }
    }

    return code;
}

int main () {

    string expression = "4 * log(x) + ((x^2) / (2^x))";
    auto code = tokenize(expression);

    int i = 0;

    while (i < code.size()) {

        if (code[i] == "x") {

        }
        else if (code[i] == "(") {
            int j = i + 1, pile = 1;
            string buffer;

            while (true) {
                if (code[j] == "(") pile++;
                if (code[j] == ")") pile--;

                if (pile == 0) break;
                buffer += code[j++];
            }

            i = j + 1;
        }
        else if (isnum(code[i])) {

        } else if (is_operator(code[i])) {

        } else if (isfunc(code[i])){

        } else {

            cout << "invalid identfier\n";
        }



        i++;
    } 




}
