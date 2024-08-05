#include <iostream>
#include <regex>

using namespace std;

void show (const std::vector<std::string> &vs) {

    for (int i = 0; i < vs.size(); i++) {
        std::cout << "[" << vs[i] << "]";
    }
}

void derivative (string src) {

    regex term ("^[-+*/^]$");
    regex numr ("^[0-9]+(\\.[0-9]+)?$");
    regex func ("^[sin|cos|tan|cot|log]$");

    regex re ("([0-9]+(\\.[0-9]+)?)|x|[-+*^]|(sin|cos|tan|cot|log)");

    sregex_token_iterator iter (src.begin (), src.end (), re);
    vector<string> token (iter, sregex_token_iterator ());
    vector<string> t1, t2;

    for (int i = 0; i < token.size(); i++) {

        if (regex_match(token[i], numr)) {
            cout << "number   : [" << token[i] << "]";
        } else if (regex_match(token[i], term)) {
            t1 = {token.begin(), token.begin() + i};
            t2 = {token.begin() + i + 1, token.end()};

            cout << "operator : " ;
            show (t1);
            cout << " <- " << token[i] << " -> ";
            show(t2);
        } else if (token[i] == "x") {
            cout << "variable : [" << token[i] << "]";
        }

        cout << endl;
    }
}

int main () {

    /*

       <variable>  ::= "x"
       <constant>  ::= [0-9]+(.[0-9]+)?
       <func_name> ::= "sin" | "cos" | "tan" | "cot" | "log"

       <factor>     ::= <basic> ( "^" <basic> )*
       <expression> ::= <term> ( "+" | "-"  <term> )*
       <term>       ::= <factor> ( "*" | "/"  <factor> )*

       <func_call>  ::= <func_name> "(" <expression> ")"
       <basic>      ::= <constant> | <variable> | <func_call> | ( "(" <expression> ")" )

       */


    derivative("2 * x^3");

}
