#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct AST {
    string op;
    int n;
    AST *a, *b;

    AST (const string &label = "", const int &num = 0) : op (label), n (num), a (nullptr), b (nullptr) {}
    AST (const string &label, AST *arg1, AST *arg2) : op (label), n (0), a (arg1), b (arg2) {}
};

#define Bin(op,a,b) (new AST (#op, (a), (b)))
#define Arg(op,n) (new AST (#op, (n)))


int oper(string id) {
    if (id == "+" || id == "-") return 1; 
    if (id == "*" || id == "/") return 2;
    return 0;
}

template<class T> static void vect (const vector<T> &now) {
    for (auto &it : now) cout << it << " ";
    cout << endl;
}

void shownode (const AST *now) {
    if (oper(now->op)) {
        cout << "Bin(" << now->op << ", ";
    } else {
        cout << "Arg(" << now->op << "," << now->n;
    }
}
void showtree (const AST *now) {
    if (now != nullptr) {
        shownode (now);
        showtree (now->a);
        showtree (now->b);
        cout << ")";
    }
}

AST *getstk (vector<AST *> &S) {   // get value from vector && delete vector
    AST *now = S.back();
    S.pop_back();
    return now;
}

vector<AST *> postorder (AST *root) {      // post order traversal

    AST *temp = root;
    vector<AST *> s1, s2;
    s1.push_back (temp);

    while (!s1.empty()) {
        temp = getstk (s1);
        s2.push_back (temp);

        if (temp->a) s1.push_back(temp->a);
        if (temp->b) s1.push_back(temp->b);
    }

    return s2;
}

int main() {

    AST *tree = 
        Bin(/,
            Bin(-,
                Bin(+,
                    Bin(*, Arg(imm,6), Arg(arg,0)),
                    Bin(*, Arg(imm,5), Arg(arg,1))),
                Bin(*, Arg(imm,3), Arg(arg,2))),

            Arg(imm, 8));

    vector<AST *>post = postorder(tree);
    
    cout << post.size() << '\n';

    for (auto node : post)

        shownode(node);
        cout<< "\n";


}
