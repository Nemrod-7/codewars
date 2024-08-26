#include <iostream>
#include <vector>
#include <complex>
#include <cmath>

using namespace std;
const double epsilon = 1e-8;

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

    if (zx.imag() == 0) {
        oss << zx.real();
    } else {
        oss << zx;
    }
    //zx.imag() == 0 ? oss << zx.real() : oss << zx;
    return oss.str();
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
bool is_operator (const string &input) {
    return input == "+" || input == "-" || input == "*" || input == "^" || input == "/";
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

string add(string a, string b) {

    if (isnum(a) && isnum(b)) return ctos(round(stoc(a) + stoc(b)));
    if (a == b) return "2*" + b;
    if (a == "0") return b;
    if (b == "0") return a;

    return a + "+" + b;
}
string sub(string a, string b) {

    if (isnum(a) && isnum(b)) return ctos(round(stoc(a) - stoc(b)));
    if (a == b) return "0";
    if (b == "0") return a;

    return a + "-" + b;
}
string mul(string a, string b) {

    if (isnum(a) && isnum(b)) return ctos(round(stoc(a) / stoc(b)));
    if (a == b) return a + "^2";
    if (a == "0" || b == "0") return "0";
    if (a == "1") return b;
    if (b == "1") return a;

    return a + "*" + b;
}
string div(string a, string b) {

    if (isnum(a) && isnum(b)) return ctos(round(stoc(a) / stoc(b)));
    if (a == b) return "1";
    if (a == "0") return "0";
    if (b == "1") return a;

    return a + "/(" + b + ")";
}
string power(string a, string b) {

    if (isnum(a) && isnum(b)) return ctos(round(pow(stoc(a), stoc(b))));
    if (a == "1" || b == "1") return a;
    if (b == "0") return "1";
    if (a == "0") return "0";
    return a + "^" + b;
}


int main () {


    complex<double> z0 = {6,0}, z1 = {2,0}, z3 = {2,2};
    complex<double> ze = {1,1};

    string input = "-4*x-32-(34 + 2)--4";

    auto code = tokenize(input);

     for (int i = 0; i < code.size(); i++) {
         cout << "[" << code[i] << "]";
     }
    // cout << endl;


    /*
    cout << 3.0 / (3.0 / 2.0) << "\n";
    cout << (3.0 / 3.0) / 2.0 << "\n";

    Let f be a function.
    The derivative function, denoted by f′, is the function whose domain consists of those values of x such that the following limit exists:

    f′(x) = lim h→0 of (f(x + h) − f(x)) / h.


    x^4 => 4.x^(4-1)




    */
}
