#include <iostream>
#include <complex>
#include <cmath>

using namespace std;
const double epsilon = 1e-8;

double round(double x) {
    return floor(x * 1e8) / 1e8; 
}
std::complex<double> round(std::complex<double> x) {
    return { round(x.real()),round(x.imag()) };
}

std::complex<double> power(std::complex<double> x, std::complex<double> y) {
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

    return a + "/" + b;
}
string power(string a, string b) {

    if (isnum(a) && isnum(b)) return ctos(round(std::pow(stoc(a), stoc(b))));
    if (a == "1" || b == "1") return a;
    if (b == "0") return "1";
    if (a == "0") return "0";
    return a + "^" + b;
}


int main () {


    std::complex<double> z0 = {6,0}, z1 = {2,0}, z3 = {2,2};
    std::complex<double> ze = {2,0};


    std::cout << z0 * pow(z3, ze);
    std::cout << z0 * power(z3, ze);

    cout << "\n";

    cout << 0 - epsilon;
}
