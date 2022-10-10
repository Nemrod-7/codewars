#include <iostream>
#include <vector>
#include <regex>
#include <map>
#include <algorithm>

using namespace std;

map<string,int> order {{"+",1},{"-",1},{"*",2},{"/",2},{"%",2}};

vector<string> tokenize (const string &expr) {
    regex re ("\\s*(=>|[-+*/%=\(\)]|[A-Za-z_][A-Za-z0-9_]*|[0-9]*\.?[0-9]+)\\s*");
    //regex re ("[0-9]?([0-9]*[.])?[0-9]+|[\\-\\+\\\\\(\\)\\/\\*]");
    sregex_token_iterator it (expr.begin(), expr.end(), re);
    return vector<string> (it, sregex_token_iterator());
}

bool isvar (const vector<string> &expr, int index) {

    if (expr[index] != "-") return false;
    if (index == 0) return true;
    if (order[expr[index - 1]]) return true;
    return false;
}
int main () {

    vector<string> expr = {"var","=","3","+","(","y","=","x",")"};

    expr = tokenize ("2*-3 - 4  -5");

    for (auto &it : expr) {
        cout << "[" << it << "]";
    }

    cout << "end";

}
