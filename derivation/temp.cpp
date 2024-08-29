#include <iostream>

using namespace std;

struct node {
    string id;

    node (const string &src = "") : id(src) {}

};


bool equal (const node *a, const char *b) {

    return a->id == b;
}

int operator + (node *a, node *b) {

    return 0;
}
int main () {

    node *test = new node("sin");


    if (equal(test, "cos")) {

    }


    cout << "end";

    delete test;
}
