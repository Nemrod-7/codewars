#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <complex>
#include <tuple>
#include <cmath>
#include "tests.hpp"


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
//
//  rules of derivation :
//  con : cst   => 0
//  lin : x     => 1
//  add : a + b => a' + b'
//  min : a - b => a' - b'
//  mul : a * b => a.b' + a'.b
//  div : a / b => (a'* b − b'* a) / (b * b)
//  pow : x^a   => a.x^(a - 1)
//  exp : e^x   => e^x
//  exp : a^x   => a^x . ln (a)
//  ln  : ln(x) => 1 / x
//  sin : sin x => cos x
//  cos : cos x => -sin x
//  tan : tan x => 1 / (cos²(x))

void show (const std::vector<std::string> &vs) {

    for (int i = 0; i < vs.size(); i++) {
        std::cout << "[" << vs[i] << "]";
    }
    std::cout << std::endl;
}

using value_t = std::complex<double>;
using func_t = std::function<value_t(value_t)>;

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
bool isfunc (const std::string &input) {
    return input == "sin" || input == "cos" || input == "tan" || input == "log" || input == "cot";
}
bool is_operator(const std::string &input) {
    return input == "+" || input == "-" || input == "*" || input == "^" || input == "/";
}
int order (const std::string &src) {
    if (src == "+" || src == "-") return 1;
    if (src == "*" || src == "/") return 2;
    if (src == "^") return 3;
    return 0;

}
std::string parenthesis(const std::string &code) {
    int i = 1, pile = 1;
    std::string buffer;

    while (true) {
        if (code[i] == '(') pile++;
        if (code[i] == ')') pile--;
        if (pile == 0) return buffer;
        buffer += code[i++];
    }

    return "";
}

std::vector<std::string> tokenize (const std::string &input) {

    int i = 0;
    std::vector<std::string> code;

    while (i < input.size()) {

        if (isdigit(input[i])) {
            std::string buffer;

            while (isdigit(input[i]) || input[i] == '.') buffer += input[i++];
            code.push_back(buffer);
        } else if (isspace(input[i])) {
            while (isspace(input[i])) i++;
        } else if (input[i] == '+' || input[i] == '-') {
            code.push_back(std::string(1,input[i++]));
        } else if (input[i] == '*' || input[i] == '/') {
            code.push_back(std::string(1,input[i++]));
        } else if (input[i] == '^') {
            code.push_back(std::string(1,input[i++]));
        } else if (isalpha(input[i])) {
            std::string buffer;

            while (isalpha(input[i])) buffer += input[i++];
            code.push_back(buffer);
        } else if (input[i] == '(') {
            std::string cell = parenthesis(input.substr(i));
            i += cell.size() + 2;

            code.push_back(cell);
        }

        else {
            code.push_back(std::string(1,input[i++]));
        }
    }

    return code;
}
double dround (double num) { return floor(1e5 * num) / 1e5; }
value_t cround (const value_t &zx) { return { dround(zx.real()), dround(zx.imag()) };}
value_t stoc(const std::string &input) {
    std::istringstream iss(input);
    value_t zx;
    iss >> zx;
    return zx;
}
std::string ctos(const value_t &zx) {
    std::ostringstream oss;


    if (zx.imag() == 0) {
        oss << zx.real();
    } else {
        oss << zx;
    }
    //zx.imag() == 0 ? oss << zx.real() : oss << zx;
    return oss.str();
}

std::string getstack(std::vector<std::string> &stack) {
    std::string val = stack.back();
    stack.pop_back();
    return val;
}
std::string join(const std::vector<std::string> &input) {
    std::string os;
    for (auto &it : input) os += it;
    return os;
}

std::string operate(std::string a, std::string op, std::string b) {

    if (isnum(a) && isnum(b)) {
        value_t z1 = stoc(a), z2 = stoc(b);
        value_t result;

        switch (op[0]) {
            case '+' : result = cround(z1 + z2) ; break;
            case '-' : result = cround(z1 - z2) ; break;
            case '*' : result = cround(z1 * z2) ; break;
            case '/' : result = cround(z1 / z2) ; break;
            case '^' : result = cround(pow(z1, z2)) ; break;
        }
        //        std::cout << "[" << z1 << "] " << code[i]<< " [" << z2 << "]";
        //        std::cout << " => " << result << "\n";
        return ctos(result);
    }

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
std::string evaluate(const std::string &expression, const std::string &number = "") {

    std::vector<std::string> code = tokenize(expression);

    int index = 0;
    std::vector<std::string> vars;
    std::vector<std::string> oper;
    std::vector<std::string> func;

    while (index < code.size()) {

        if (code[index] == "x") {
            std::string var = number == "" ? code[index] : number;
            vars.push_back(var);
        } else if (isnum(code[index])) {
            //std::cout << "number : " << code[index] << endl;
            vars.push_back(code[index]);
        } else if (isfunc(code[index])) {
            std::string next = code[index + 1];
            std::string result;

            if (number == "") {
                result = code[index] + " " + next;
            } else {
                if (code[index] == "cos") {
                    result = ctos (cos(stoc(number)));
                } else if (code[index] == "sin") {
                    result = ctos(sin(stoc(number)));
                } else if (code[index] == "tan") {
                    result = ctos(tan(stoc(number)));
                } else if (code[index] == "log") {
                    result = ctos(log(stoc(number)));
                } else if (code[index] == "cot") {
                    //cot(x) = cos(x)/sin(x)
                    //cot(x) = 1 / tan(x)
                    value_t x = stoc(number);
                    value_t res = cos(x) / sin(x);
                    result = ctos(res);
                }
                //std::cout << code[index] << " " << number << " = " << result << "\n";
                result = code[index] + " " + next;
            }

            vars.push_back(result);
            index++;
        } else if (order(code[index])) {

            while (!oper.empty() && order(oper.back()) > order(code[index])) {
                std::string v2 = getstack(vars), v1 = getstack(vars), op = getstack(oper);
                std::string result = operate(v1,op,v2);

                vars.push_back(result);
            }

            oper.push_back(code[index]);
        }

        index++;
    }

    while (!oper.empty()) {
        std::string v2 = getstack(vars), v1 = getstack(vars), op = getstack(oper);
        std::string result = operate(v1,op,v2);

        vars.push_back(result);
    }

    return vars.back();
}
std::string diff(std::string t1, std::string term, std::string t2) {
    std::string result;

    std::string derivate(const std::string &);

    if (term == "+") { // add : a + b => a' + b'
        std::string d1 = derivate(t1), d2 = derivate(t2);

        return (operate(d1, term, d2));
    } else if (term == "-") { // min : a - b => a' - b'
        std::string d1 = derivate(t1), d2 = derivate(t2);

        return (operate(d1, term, d2));
    } else if (term == "*") { // mul : a * b => a.b' + a'.b
        std::string d1 = operate(t1, "*", derivate(t2));
        std::string d2 = operate(derivate(t1), "*", t2);

        return (operate(d1, "+", d2));
    } else if (term == "/") { // div : a / b => (a'* b − b'* a) / (b * b)
        std::string d1 = operate(derivate(t1), "*", t2);
        std::string d2 = operate(t1, "*", derivate(t2));
        // std::cout << "[" << d1 << "] " << "-" << " [" << d2 << "]\n";

        std::string d3 = operate(d1, "-", d2);
        std::string d4 = operate(t2, "*", t2);

        return (operate (d3, "/", d1));
    } else if (term == "^") { // pow : x^a   => a.x^(a - 1)

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
        //std::cout << "[" << t1 << "] [" << t2 << "] => ";
        //std::cout << operate(t1,"*",t2) << "|" << operate(t2,"*",t1);
        //std::cout << "\n";
    }



    return result;
}
std::string derivate(const std::string &input) {

    if (input == "x") return "1"; // constant
    if (isnum(input)) return "0"; // variable

    int i = 0;
    std::vector<std::string> code = tokenize(input);
    // show(code);

    while (i < code.size()) {

        if (is_operator(code[i])) {
            int j = i - 1, k = i;

            while (j--> 0) 
                if (is_operator(code[j])) { break; };

            while (k++ < code.size()) 
                if (is_operator(code[k])) { break; } 

            std::string t1 = join({code.begin() + j + 1, code.begin() + i});
            std::string t2 = join({code.begin() + i + 1, code.begin() + k });

            // std::cout << "[" << t1 << "] " << op << " [" << t2 << "]";
            if (code[i]== "+") { // add : a + b => a' + b'
                std::string d1 = derivate(t1), d2 = derivate(t2);

                return (operate(d1, code[i], d2));
            } else if (code[i]== "-") { // min : a - b => a' - b'
                std::string d1 = derivate(t1), d2 = derivate(t2);

                return (operate(d1, code[i], d2));
            } else if (code[i]== "*") { // mul : a * b => a.b' + a'.b
                std::string d1 = operate(t1, "*", derivate(t2));
                std::string d2 = operate(derivate(t1), "*", t2);

                return (operate(d1, "+", d2));
            } else if (code[i]== "/") { // div : a / b => (a'* b − b'* a) / (b * b)
                std::string d1 = operate(derivate(t1), "*", t2);
                std::string d2 = operate(t1, "*", derivate(t2));
                // std::cout << "[" << d1 << "] " << "-" << " [" << d2 << "]\n";

                std::string d3 = operate(d1, "-", d2);
                std::string d4 = operate(t2, "*", t2);

                return (operate (d3, "/", d1));
            } else if (code[i]== "^") { // pow : x^a   => a.x^(a - 1)

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
                //std::cout << "[" << t1 << "] [" << t2 << "] => ";
                //std::cout << operate(t1,"*",t2) << "|" << operate(t2,"*",t1);
                //std::cout << "\n";
            }

        } else {
            std::string arg = code[i + 1];

            if (code[i] == "cos") {
                std::string dx = "-sin " + arg;

                return evaluate(dx);
            } else if (code[i] == "sin") {
                std::string dx = "cos " + arg;

                return evaluate(dx);
            } else if (code[i] == "tan") { //  tan : tan x => 1 / (cos²(x))
                std::string dx1 = operate("cos","^","2");
                std::string dx2 = operate(dx1, "*", arg);
                std::string dx3 = operate("1","/", dx2);

                return evaluate(dx3);
            } else if (code[i] == "log") {
                std::string dx = operate("1","/","x");
                return evaluate(dx);
            } else if (code[i] == "cot") {

            }
        }

        i++;
    }

    return "";
}

std::tuple<func_t, func_t, func_t> differentiate(const std::string& eq) {

    std::string pass0 = eq;
    std::string pass1 = derivate(pass0);
    std::string pass2 = derivate(pass1);

    std::cout << "\nresult : \n";
    std::cout << "pass0  : " << pass0 << "\n";
    std::cout << "pass1  : " << pass1 << "\n";
    std::cout << "pass2  : " << pass2 << "\n";

    return {
        { [pass0](value_t x) { return stoc ( evaluate(pass0, ctos(x))); } },
            { [pass1](value_t x) { return stoc ( evaluate(pass1, ctos(x))); } },
            { [pass2](value_t x) { return stoc ( evaluate(pass2, ctos(x))); } },
    };
}

int main () {


    // const auto [f, df_dx, d2f_dx2] = differentiate("4 * log(x) + x^2 / 2^x");
    // const auto [f, df_dx, d2f_dx2] = differentiate("sin(cos(x^x^2))");
    // const auto [f, df_dx, d2f_dx2] = differentiate("(tan(2 * x) + 1) / (cot(x * 3) - 1)");

    std::string eq = "x^2";
    // auto code = tokenize(eq);
    // std::string pass0 = derivate("3*x^2");
    // std::vector<std::string> vars {"x","2"}, oper = {"^"};

    // std::string d1 = derivate(eq);
    // std::cout << "=> [" << d1 << "]\n";





    // tests();
    //const auto [f, df_dx, d2f_dx2] = differentiate("sin(cos(x^x^2))");

    //    {
    //
    //        Assert::That(f({ 1, 1 }),       EqualsAdaptive(value_t{ 0.839472, -0.0115338 }), ExtraMessage("The function failed! f(x) = sin(cos(x^x^2)), x = (1, 1)"));
    //        Assert::That(df_dx({ 1, 1 }),   EqualsAdaptive(value_t{ 0.0752251, -0.0149614 }), ExtraMessage("The first derivative failed! f(x) = sin(cos(x^x^2)), x = (1, 1)"));
    //        Assert::That(d2f_dx2({ 1, 1 }), EqualsAdaptive(value_t{ 0.12722, 0.402059 }), ExtraMessage("The second derivative failed! f(x) = sin(cos(x^x^2)), x = (1, 1)"));
    //    }


}
