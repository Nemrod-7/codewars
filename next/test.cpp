#include <iostream>
#include <vector>
#include <map>
#include <regex>
#include <algorithm>

using namespace std;

map<string,string> fbase = {{"inc","x => x + 1"},{"echo", "x => x"},{"add", "x y => x + y"}, {"avg","x y => (x + y)/2"}};

vector<string> tokenize (const string &expr) {
    regex re ("=>|(_|-)?[0-9]+(\\.[0-9]+)?|[-+*/%()=\\[\\]]|_?[a-zA-Z]+_?");
    sregex_token_iterator it (expr.begin(), expr.end(), re);
    return vector<string> (it, sregex_token_iterator());
}

int nvar (const string &func) {
    vector<string> tok = tokenize (fbase[func]);
    return find (tok.begin(), tok.end(), "=>") - tok.begin();
}

vector<string> getargs (int &index, const vector<string> &expr) {

    vector<string> args;

    int ref = index;
    int nva = nvar(expr[index]);

    for (; index < expr.size(); index++) {
        string arg = expr[index];

        if (fbase.find (expr[index]) != fbase.end()) {
            int nva2 = nvar(expr[index]);

            for (int j = index + 1; j < nva2 + index + 1; j++) {
                arg += " " + expr[j];
            }
            index += nva2;
        }

        args.push_back(arg);
    }
    return args;
}
int main () {

    vector<string> expr = {"var","=","3","+","(","y","=","x",")"};
    // i.input("avg 7 2 4"
    expr = tokenize ("avg 1 2 5");
    //expr = tokenize ("add avg echo 2  echo 3 echo 5");

    regex re("-?\\d+(\\.\\d)+?");
    int index = 1;
    vector<string> args = getargs (index, expr);


    for (auto &it : args) {

        cout << "[" << it << "]";
    }

    /*
    */
}
