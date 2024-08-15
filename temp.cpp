#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

bool isnum (const string &src) {

    for (auto &it : src) {
        if (!isdigit(it)) return false;
    }

    return true;
}
vector<string> tokenize(const string &input) {
    vector<string> token;

    for (auto &it : input) {
        token.push_back(string(1,it));
    }

    return token;
}
string operate (string a, string op, string b) {

    switch (op[0]) {
        case '+' : return to_string( stoi(a) + stoi(b)) ; break;
        case '-' : return to_string( stoi(a) - stoi(b)) ; break;
        case '*' : return to_string( stoi(a) * stoi(b)) ; break;
        case '/' : return to_string( stoi(a) / stoi(b)) ; break;
        case '^' : return to_string((int) pow(stoi(a), stoi(b))) ; break;
    }

    return "0";
}


string differentiate(const string &input) {

    if (input == "x") return "1"; 
    if (isnum(input)) return "0";

    vector<string> code = tokenize(input);

    if (code.size() == 3) {
        string a = code[0], op = code[1], b = code[2];

        if (isnum(a) && isnum(b)) {
            return operate(a,op,b);
        }

        /*if (op == "+") {*/
        /*    if (a == b) { op = "*", a = "2"; }*/
        /*    if (a == "0") return b;*/
        /*    if (b == "0") return a;*/
        /*} else if (op == "-") {*/
        /*    if (a == b) return "0";*/
        /*    if (b == "0") return a;*/
        /*} else if (op == "*") {*/
        /*    if (a == b) { op = "^", a = "2"; }*/
        /*    if (a == "0" || b == "0") return "0";*/
        /*    if (a == "1") return b;*/
        /*    if (b == "1") return a;*/
        /*} else if (op == "/") {*/
        /*    if (a == b) return "1";*/
        /*    if (a == "0") return "0";*/
        /*    if (b == "1") return a;*/
        /*} else if (op == "^") {*/
        /*    if (a == "1" || b == "1") return a;*/
        /*    if (b == "0") return "1";*/
        /*    if (a == "0") return "0";*/
        /*}*/
    }

        vector<string> t1, t2;
    
        for (int i = 0; i < code.size(); i++) {
             if (code[i] == "*") {
                t1 = {code.begin(), code.begin() + i};
                t2 = {code.begin() + i + 1, code.end()};

             }

             cout << endl;
        }




        return ""; 
}

int main () {

    vector<string> code = {"2","*","x","^","3"};
    // (2) * (x^3) => a.b' + a'.b
    // (x) ^ (3)   => a.x^(a - 1) 

    vector<string> t1,t2;
    t1 = {"2"}, t2 = {"x","^","3"} ;


    string actual = differentiate("x+3");
    cout << "result : " << actual << "\n";


}
