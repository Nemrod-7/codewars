#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class Display {
    public :
        static void vect (vector<string> V) {
            for (auto &it : V) {
                cout << "["<<it<<"]";
            }
            cout << endl;
        }
};

string differentiate2 (const string &poly) {

    if (poly.find('x') == string::npos) return "0";

    string derivative;
    int dig = 1, exp = 1;

    if (poly[0] == 'x') {
        sscanf (poly.c_str(), "x^d", &exp);
    } else if (poly[0] == '-' && poly[1] == 'x') {
        sscanf (poly.c_str(), "-x^d", &exp);
    } else {
        sscanf (poly.c_str(), "dx^d", &dig, &exp);
    }

    dig *= exp, exp -= 1;

    if (exp) {
        if (dig == -1) {
          derivative += "-";
        } else if (dig < -1 || dig > 1) {
          derivative += to_string (dig);
        }
        derivative += "x";
    } else {
        derivative = to_string (dig);
    }

    return derivative;
}

/*

<constant>  ::= [0-9]+ ( "." [0-9]+ )?
<variable>  ::= "x"
<func_name> ::= "sin" | "cos" | "tan" | "cot" | "log"

<expression> ::= <term> ( ( "+" | "-" ) <term> )*
<term>       ::= <factor> ( ( "*" | "/" ) <factor> )*
<factor>     ::= <basic> ( "^" <basic> )*
<func_call>  ::= <func_name> "(" <expression> ")"
<basic>      ::= <constant> | <variable> | <func_call> | ( "(" <expression> ")" )


const auto f = differentiate("2 * x^3");
std::get<0>(f)({ 2, 2 }) == (-32, 32)
  Because:
  f(x) = 2 * x^3
  f(2 + 2i) = 2 * (2 + 2i)^3 = -32 + 32i


std::get<1>(f)({ 2, 2 }) == (0, 48)
  Because:
  f'(x) = (2 * x^3)' = 6 * x^2
  f'(2 + 2i) = 6 * (2 + 2i)^2 = 48i


std::get<2>(f)({ 2, 2 }) == (24, 24)
  Because:
  f''(x) = (6 * x^2)' = 12 * x
  f''(2 + 2i) = 12 * (2 + 2i)^2 = 24 + 24i

*/
string differentiate (const string &poly) {

    string derivative;

    return derivative;
}
int main () {

  /*
    "3x^2"  => "6x"
    "-5x^3" => "-15x^2"
    "6x^-2" => "-12x^-3"
    "5x"    => "5"
    "-x"    => "-1"
    "42"    => "0"

    */
    cout << differentiate ("x");

    //Display::vect(expr);
}
