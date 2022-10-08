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

vector<string> getargs (vector<string>::iterator &it, const vector<string> &expr) {

    const string name = *it;
    const int nargs = nvar(name);
    vector<string> args;

    while (it < expr.end() && args.size() < nargs) {
        it++;
        string arg = *it;

        if (fbase.find(arg) != fbase.end()) {
            vector <string> nxt = getargs (it, expr);

            for (auto &it2 : nxt)
                arg += " " + it2;
        }

        args.push_back(arg);
    }

    return args;
}
int main () {

    vector<string> expr = {"var","=","3","+","(","y","=","x",")"};
    // i.input("avg 7 2 4"
    expr = tokenize ("add avg echo 2  echo 3 echo 5");
    expr = tokenize ("add echo 1 echo 2");
    expr = tokenize ("avg 4 2 + avg 10 30");


    regex re("-?\\d+(\\.\\d)+?");
    vector<string>::iterator it = expr.begin();

    string name = *it;
    int nargs = nvar(name);

    vector<string> args = getargs(it, expr);


    for (auto &it : args) {
        cout << "[" << it << "]";
    }
    /*

    */
}
