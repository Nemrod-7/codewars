#include <vector>

using namespace std;

string getnum (string::iterator &it) {
    string num;
    if (isalpha (*it)) return "1";
    if (*it == '+') it++;

    while (isdigit (*it) || *it == '-')
        num += *it++;

    return num .back() != '-' ? num : num + "1";
}
vector<string> decomp (string src) {
    string::iterator it = src.begin() + 1;
    vector<string> expr (3);
    expr[0] = getnum (it);
    expr[1] = *it++;
    expr[2] = getnum (it);
    return expr;
}
string form (int num) {
    string os;
    if (num < -1 || num > 1) {
        os += to_string (num);
    } else if (num == -1) {
        os += "-";
    }

    return os;
}
string quadratic_builder (const string &src) {

    string fst = src.substr (0, src.find (")") + 1), scd = src.substr (src.find (")") + 1);
    vector<string> ex1 = decomp (fst), ex2 = decomp (scd);
    
    int num = stoi (ex1[0]) * stoi (ex2[0]);
    string equ = form (num) + ex1[1] + "^2";

    num = stoi (ex1[0]) * stoi (ex2[2]) + stoi (ex1[2]) * stoi (ex2[0]);

    if (num != 0) {
        if (num > 0) equ += "+";
        equ += form (num) + ex1[1];
    }

    num = (stoi (ex1[2]) * stoi (ex2[2]));

    if (num != 0) {
        if (num > 0) equ += "+";
        equ += to_string (num);
    }

    return equ;
}
