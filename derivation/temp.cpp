#include <iostream>
#include <vector>
#include <string>

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
std::string operate(std::string a, std::string op, std::string b) {

    if (isnum(a) && isnum(b)) {
        int z1 = stoi(a), z2 = stoi(b), result;

        switch (op[0]) {
            case '+' : result = (z1 + z2) ; break;
            case '-' : result = (z1 - z2) ; break;
            case '*' : result = (z1 * z2) ; break;
            case '/' : result = (z1 / z2) ; break;
                       //         case '^' : result = (pow(z1, z2)) ; break;
        }
        //     //std::cout << "[" << z1 << "] " << op << " [" << z2 << "]";
        //     //std::cout << " => " << result << "\n";
        return std::to_string(result);
    }

    if (op == "+") {
        if (a == b) { op = "*", a = "2"; }
        if (a == "0") return b;
        if (b == "0") return a;
    } else if (op == "-") {
        if (a == b) return "0";
        if (b == "0") return a;
    } else if (op == "*") {
        if (a == b) { op = "^", b = "2"; } // if (a == b) { op = "^", a = "2"; }
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

int main () {

//  pow : x^a   => a.x^(a - 1)
//  exp : a^x   => a^x . ln (a)
    std::string t1 = "2", op = "^", t2 = "x";

    if (t2 == "x") {
        std::string arg1 = operate(t1,"^",t2);
        std::string arg2 = "log(" + t1 + ")";
        std::string arg3 = operate(arg1,"*",arg2);

        std::cout << arg3;

    //    std::cout << "[" << arg2 << "] " << op << " [" << exp << "]";
    }

    if (t1 == "x") {
        std::string exp = operate(t2,"-","1");
        std::string arg1 = operate(t2,"*",t1);
        std::string arg2 = operate(arg1,"^",exp);

    }
    




}
