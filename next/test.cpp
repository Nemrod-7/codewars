#include <iostream>

#include <map>
#include <regex>
#include <algorithm>

using namespace std;

map<string,string> fbase = {{"inc","x => x + 1"},{"echo", "x => x"},{"add", "x y => x + y"}};

vector<string> tokenize (const string &expr) {
    regex token ("=>|(_|-)?[0-9]+(.[0-9]+|_)?|[-+*/%()=\\[\\]]|_?[a-zA-Z]+_?");
    sregex_token_iterator it (expr.begin(), expr.end(), token);
    return vector<string> (it, sregex_token_iterator());
}


int main () {

    vector<string> expr = {"var","=","3","+","(","y","=","x",")"};

    expr = tokenize ("echo a echo b");

    
    /*
    regex variab ("_?[a-zA-Z]+_?|_[0-9]+|[0-9]+_");
    string id = expr[0], os;
    vector<string> fn = tokenize (fbase[id]), vars;
    auto fn_op = find (fn.begin(), fn.end(), "=>");

    for (int i = 1; i != expr.size(); i++) {
        vars.push_back(expr[i]);
    }

    for (auto fn_ex = fn_op + 1; fn_ex != fn.end(); fn_ex++) {
        string token = *fn_ex;

        if (regex_match (token, variab)) {
            auto pos = find (fn.begin(), fn_op, token) - fn.begin();
            token = vars[pos];
        }
        os += token + " ";
    }
    */
}
