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
bool isoper(const string &input) {
    return input == "*" || input == "/" || input == "+" || input == "-";
}

std::string join(const std::vector<std::string> &input) {
    std::string os;
    for (auto &it : input) os += it;
    return os;
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

int main () {

    string eq = "4 * log(x) + x^2 / 2^x";
    vector<string> code = tokenize(eq);
    int i = 0;

    while (i < code.size()) {


        if (code[i] == "x") {
            /*cout << "   < variable | ";*/
        } else if (isnum(code[i])) {
            /*cout << "   < constant | ";*/
        } else if (isfunc(code[i])) {
            //cout << " < function | ";       
            //cout << "t1 : " << code[i+1]; 
        } else if (isoper(code[i])) {
            cout << "[" << code[i] << "]";
            /**/
            /*if (code[i] == "+" || code[i] == "-") {*/
            /*    cout << "   < term     | ";*/
            /*} else {*/
            /*    cout << "   < factor   | ";*/
            /*}*/

            int j = i - 1;
            string t1,t2;

            while (j--> 0) {
                if (isoper(code[j])) { break; };
            }            

            t1 = join({code.begin() + j + 1, code.begin() + i});

            j = i;
            while (j++ < code.size()) {
                if (isoper(code[j])) { break; } 
            }

            t2 = join({code.begin() + i + 1, code.begin() + j });
            cout << "t1 : [" << t1 << "] ";
            cout << "t2 : [" << t2 << "] ";


            cout << endl;
        } else if (code[i] == "^") {
            cout << "[" << code[i] << "]";
            cout << "   < basic    | "; 
            cout << "t1 : " << code[i+1]; 
        }

        i++;
    }

}
