#include <iostream>
#include <vector>
#include <regex>

using namespace std;

void show (const std::vector<std::string> &vs) {

    for (int i = 0; i < vs.size(); i++) {
        std::cout << "[" << vs[i] << "]";
    }

    cout << endl;
}

vector<string> tokenize (const string &input) {
    regex re ("[0-9]+|[-+*/^()]");  
    sregex_token_iterator iter (input.begin (), input.end (), re);
    return vector<string> (iter, sregex_token_iterator ());
}

vector<string> clean (const vector<string> &token) {

    const regex oper ("^[-+*/^]$");
    vector<string> code;
    
    for (int i = 0; i < token.size(); i++) {
        if (token[i] == "-" && (i == 0 || regex_match(token[i-1], oper))) {
            code.push_back("-" + token[i + 1]);
            i += 2;
        } 

        code.push_back(token[i]);
    }

    return code;
}


int main () {

    string expr = "6-5*-6/(56)-78";  
    regex oper ("^[-+*/^]$");
    //expr.erase(std::remove(expr.begin(), expr.end(), ' '), expr.end());
    
    auto code = tokenize(expr);

    //for (int i = 0; i < code.size(); i++) {
    //    if (code[i] == "-" && (i == 0 || regex_match(code[i-1], oper))) {
    //        code.erase(code.begin() + i);
    //        code[i] = "-" + code[i];
    //    } 
    //    cout << "[" << code[i] << "]";
    //}

    show(clean(code));

    cout << "\nend\n";
}
