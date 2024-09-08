#include <iostream>
#include <vector>
#include <regex>

using namespace std;

struct node {
    string sym;
    node *t1, *t2;

    node (const string &label = "", node *t1 = nullptr, node *t2 = nullptr) : sym (label), t1 (t1), t2 (t2) {}
};
void show (const std::vector<std::string> &vs) {

    for (int i = 0; i < vs.size(); i++) {
        std::cout << "[" << vs[i] << "]";
    }

    cout << endl;
}
void showtree(const node *node, bool isLeft = false, const string &prefix = "") {
    if (node != nullptr) {
        cout << prefix;
        cout << (isLeft ? "├─" : "└─" );
        cout << "[" << node->sym << "]" << endl;

        showtree(node->t1, true, prefix + (isLeft ? "│  " : "   "));
        showtree(node->t2, false, prefix + (isLeft ? "│  " : "   "));
    }
}

vector<string> tokenize(const string &input) {
    const regex tokn ("((?<!foo-)?[0-9]+(\\.[0-9]+)?)|x|[-+*^()]|(sin|cos|tan|cot|log)");
    sregex_token_iterator iter (input.begin (), input.end (), tokn);
    return vector<string> (iter, sregex_token_iterator ());
}

int order (const string &src) {
    if (src == "+" || src == "-") return 1;
    if (src == "*" || src == "/") return 2;
    if (src == "^") return 3;
    return 0;
}
template<class T> T getstack (vector<T> &stack) {
    T val = stack.back();
    stack.pop_back();
    return val;
}

string parenthesis(vector<string>::iterator &it) {

    it += 1;
    int pile = 1;
    string sub ;

    while (true) {
        pile += (*it == "(") - (*it == ")");
        if (pile == 0) break;
        sub += *it++;
    }

    return sub;
}
node *parse (const string &input) {

    const regex oper ("^[-+*^]$");
    const regex trig ("^sin|cos|tan|cot|log$");
    const regex number ("^-?\\d+(.\\d+)?|\\(-?\\d+(.\\d+)?,-?\\d+(.\\d+)?\\)$");
    vector<string> code = tokenize(input), stack;
    vector<string>::iterator it = code.begin();
    vector<node*> tree;

    while (it < code.end()) {
        string cell = *it;

        if (cell == "x") {
            tree.push_back( new node(cell));
        } else if (cell == "(") {
            tree.push_back(parse(parenthesis(it)));
        } else if (regex_match(cell, number)) {
            tree.push_back( new node(cell));
        } else if (regex_match(cell, oper)) {

            while(!stack.empty() && order(stack.back()) > order(cell)) {
                node *next = new node(getstack(stack));
                next->t2 = getstack(tree);
                next->t1 = getstack(tree);
                tree.push_back(next);
            }

            stack.push_back(cell);
        } else if (regex_match(cell, trig)) {
            it++;
            node *next = new node(cell);
            next->t1 = parse(parenthesis(it));
            tree.push_back(next);
        } else {
            cout << "invalid identifier\n";
        }

        it++;
    }


    while(!stack.empty()) {

        node *next = new node(getstack(stack));
        next->t2 = getstack(tree);
        next->t1 = getstack(tree);
        //showtree(next);
        tree.push_back(next);
    }

    //cout << "size : " << tree.size() << "\n";
    return tree.back();
}

int main () {

    vector<string> token = {"75+","- 84","","-85.52", "(-253.054,258.785)"};
    string expr = "(tan(-2 * x)+1/(cot(x+3)-1)";

show(tokenize(expr));


    //showtree(parse("4 * log(x) + x^2 / 2^x"));
    //showtree(parse("(tan(2 * x) + 1) / (cot(x * 3) - 1)"));

    //showtree(parse("-2.508 -85 * x^3"));
    //showtree(parse("2 * x^3"));
    //showtree(parse("sin(cos(x^x^2))"));
    //showtree(parse("4 * log(x) + x^2 / 2^x"));
    //showtree(parse("(tan(2 * x) + 1) / (cot(x * 3) - 1)"));

    cout << "\nend\n";
}
