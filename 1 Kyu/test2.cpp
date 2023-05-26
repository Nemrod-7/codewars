#include <iostream>
#include <vector>
#include <regex>
#include <map>

using namespace std;
map<string, pair<string,string>> fbase; // functions base : <func name, <args, function>>


void display (vector<string> expr) {
    for (auto it : expr) {
        cout << "[" << it << "]";
    }
    cout << endl;
}

vector<string> tokenize (const string &expr) {
    regex re ("=>|_?[0-9]+(\\.[0-9]+)?|[-+*/%()=\\[\\]]|_?[a-zA-Z]+_?");
    //regex re ("=>|(_|-)?[0-9]+(\\.[0-9]+)?|[-+*/%()=\\[\\]]|_?[a-zA-Z]+_?");
    sregex_token_iterator it (expr.begin(), expr.end(), re);
    return vector<string> (it, sregex_token_iterator());
}
vector<string> getargs (vector<string>::iterator &it, const vector<string> &expr) {

    const string name = *it;
    const int nargs = tokenize(fbase[name].first).size();
    vector<string> args;
    cout << name << " => ";

    while (it + 1 < expr.end() && args.size() < nargs) {
        it++;
        string cell = *it;

        if (cell == "(") {
            while (*it != ")") {
              cell += *++it;
            }
        } else if (fbase.find(cell) != fbase.end()) {
            vector<string> nxt = getargs (it, expr);

            for (auto &it2 : nxt) {
                cell += " " + it2;
            }
        }

        cout << "["<< cell << "]";
        /*

        */
        args.push_back(cell);
    }

    return args;
}

int main () {

    fbase["sq"] = {"x","x * x"};
    fbase["add"] = {"x y","x + y"};
    fbase["sub"] = {"x y","x - y"};
    fbase["mul"] = {"x y","x * y"};
    fbase["div"] = {"x y","x / y"};
    fbase["inc"] = {"x","x + 1"};
    fbase["echo"] = {"x","x"};

    // Assert::That(interpret("avg 4 2 + avg 10 30"), Equals(23.0));

    string input = "add (4+5) 67";
    vector<string> expr = tokenize (input);
    vector<string>::iterator it = expr.begin();

    vector<string> args = getargs (it, expr);
    // cout << args.size();
    // display(args);

    cout << "end";
}
