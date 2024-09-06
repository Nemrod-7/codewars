#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct node {
    string sym;
    node *t1, *t2;

    node (const string &label = "", node *t1 = nullptr, node *t2 = nullptr) : sym (label), t1 (t1), t2 (t2) {}

};

string evaluate (node *curr) {

    if (curr != nullptr) {
        string symbol = curr->sym;
        string a = evaluate(curr->t1), b = evaluate(curr->t2);
        //cout << "[" << a << "](" << symbol << ")[" << b << "]\n";
        return a + symbol + b;
    }

    return "";
}

bool is_term(const string &sym) { return sym == "+" || sym == "-"; }
bool is_fact(const string &sym) { return sym == "*" || sym == "/"; }
bool is_number (const string &input) {

    if (input.size() == 0) return false;
    int i = 0, end = input.size();

    if (input.front() == '(' && input.back() == ')') {
        i += 1, end -= 1;
    }

    for (; i < end; i++) {
        if (input[i] == '-' && isdigit(input[i+1])) continue;
        if (input[i] != '.' && input[i] != ',' && !isdigit(input[i])) {
            return false;
        }
    }

    return true;
}
bool is_operator (const string &input) {
    return is_fact(input) || is_term(input) || input == "^";
}

node *div(node *a, node *b) {

    //if (a->sym == b->sym) return new node ("1");
    if (a->sym == "0") return new node ("0");
    if (b->sym == "1") return a;
    if (is_number(a->sym) && is_number(b->sym)) return new node(to_string(stoi(a->sym) / stoi(b->sym)));

    return new node ("/",a,b);
}
node *add(node *a, node *b) {

    // if (a->sym == b->sym && !is_operator(a->sym)) return new node ("*",new node("2"), a);
    if (a->sym == "0") return b;
    if (b->sym == "0") return a;
    if (is_number(a->sym) && is_number(b->sym)) return new node(to_string(stoi(a->sym) + stoi(b->sym)));

    return new node ("+",a,b);
}
node *sub(node *a, node *b) {

    //if (a->sym == b->sym) return new node("0");
    if (b->sym == "0") return a;
    if (is_number(a->sym) && is_number(b->sym)) return new node(to_string(stoi(a->sym) - stoi(b->sym)));

    return new node ("-",a,b);
}
node *mul(node *a, node *b) {

    // if (a->sym == b->sym) return new node ("*",a, b);
    if (a->sym == "0" || b->sym == "0") return new node ("0");
    if (a->sym == "1") return b;
    if (b->sym == "1") return a;
    if (is_number(a->sym) && is_number(b->sym)) return new node(to_string(stoi(a->sym) * stoi(b->sym)));

    return new node ("*",a,b);
}
node *exp(node *a, node *b) {

    if (a->sym == "1" || b->sym == "1") return a;
    if (b->sym == "0") return new node("1");
    if (a->sym == "0") return new node("0");
    if (is_number(a->sym) && is_number(b->sym)) return new node(to_string(pow(stoi(a->sym), stoi(b->sym))));

    return new node("^",a,b);
}

pair<vector<string>,vector<string>> getarg(node *curr) {

    if (curr->sym == "x") {
        return {{"x"},{"x"}};
    } else if (is_number(curr->sym)) {
        return {{curr->sym},{curr->sym}};
    } else if (curr->sym == "*") {
        pair<vector<string>,vector<string>> v1 = getarg(curr->t1), v2 = getarg(curr->t2);

        v1.first.insert( v1.first.end(), v2.first.begin(), v2.first.end() );
        v1.second.insert( v1.second.end(), v2.second.begin(), v2.second.end() );

        return {v1};
    } else if (curr->sym == "^") {
        return {getarg(curr->t1).first,{}};
    } else if (curr->sym == "/") {
        return {getarg(curr->t1).first,getarg(curr->t2).second};
    }

    return {};
}

int main () {


    node *tree = new node("*", new node("2"),
            new node("*",
                new node("/", new node("3"), new node("x")),
                new node("^", new node("x"), new node("3"))
                )) ;



    cout << "\nend\n";
}
