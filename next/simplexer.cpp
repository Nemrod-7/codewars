#include <iostream>
#include <vector>
#include <regex>

using namespace std;

vector<string> tokenize (const string &expr) {
    regex re ("[0-9]+|_?[a-zA-Z0-9]+|[-*+/%()]|[ \t\n]|(\".*\")");
    sregex_token_iterator it (expr.begin(), expr.end(), re);
    return vector<string> (it, sregex_token_iterator());
}

int main () {

    string txt = "123false + id123-\"this is a string\"";
    auto token = tokenize(txt);
    const regex number ("(-?[0-9]+(.[0-9]+)?)"), quote ("\".+?\"|\'.?\'"), oper ("[-+*/%()]"), wspace ("[ \t\n]");


    for (auto &tok : token) {

        if (tok == "true" || tok == "false") {
              cout << "bool: [" << tok << "]";
        } else
        
        if (regex_match(tok, number)) {
            cout << "digit: [" << tok << "]";
        } else if (regex_match(tok, oper)) {
            cout << "oper : [" << tok << "]";
        } else if (regex_match(tok, wspace)) {
            cout << "white: [" << tok << "]";
        }

        cout << '\n';
    }


}
