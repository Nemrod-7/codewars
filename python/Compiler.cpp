#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stack>
#include <regex>
#include <functional>
#include <chrono>

#include "../../templates/Assert.hpp"

using namespace std;
using func = function<double(double,double)>;

map<string,int> order {{"+", 1},{"-",1},{"*",2},{"/",2}};
map<string, func> oper {{"+", plus<double>()},{"-", minus<double>()},
              {"*", multiplies<double>()}, {"/", divides<double>()}};

struct AST {
    string op;
    int n;
    AST *a, *b;

    AST (const string &label = "", const int &num = 0) : op (label), n (num), a (nullptr), b (nullptr) {}
    AST (const string &label, AST *arg1, AST *arg2) : op (label), n (0), a (arg1), b (arg2) {}
};

/////////////////////////////////tools//////////////////////////////////////////
#define Bin(op,a,b) (new AST (#op, (a), (b)))
#define Arg(op,n) (new AST (#op, (n)))

bool equals (const AST &lhs, const AST &rhs) {
  return lhs.op == rhs.op && lhs.n == rhs.n && (lhs.a && rhs.a ? equals (*lhs.a, *rhs.a) : true) && (lhs.b && rhs.b ? equals (*lhs.b, *rhs.b) : true);
}
bool equals (const AST *lhs, const AST *rhs) { return equals (*lhs, *rhs); }

template<class T> static void vect (const vector<T> &now) {
    for (auto &it : now) cout << it << " ";
    cout << endl;
}

void shownode (const AST *now) {
    if (oper[now->op]) {
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
/*
*/
////////////////////////////////////////////////////////////////////////////////
class Compiler {
    private :
        map<string, int> args;

        bool isnum (const string &ex) {
            regex number ("(-?[0-9]+(.[0-9]+)?)");
            return regex_match (ex, number);
        }
        AST *getstk (stack<AST *> &S) {   // get value from stack && delete stack
            AST *now = S.top();
            S.pop();
            return now;
        }
        stack<AST *> postorder (AST *root) {      // post order traversal

            AST *temp = root;
            stack<AST *> s1, s2;
            s1.push (temp);

            while (!s1.empty()) {
                temp = getstk (s1);
                s2.push (temp);

                if (temp->a) s1.push(temp->a);
                if (temp->b) s1.push(temp->b);
            }

            return s2;
        }
        AST *mktree (const vector<string> &expr) { // make AS tree
            if (!expr.size()) throw runtime_error ("Empty expression");

            bool running = true;
            vector<string>::const_iterator it = expr.begin();
            stack<AST *> tree, ops;

            while (running) {
                string tile = *it;

                if (tile == "(") {
                    it++;
                    auto nxt = find (it, expr.end(), ")");

                    AST *sub = mktree ({it, nxt});
                    tree.push (sub);

                    it = nxt;
                } else if (isnum (tile)) {
                    tree.push (new AST ("imm", stoi (tile)));
                } else if (args[tile]) {
                    tree.push ((new AST ("arg", args[tile] - 1)));
                } else if (oper[tile]) {
                    AST *temp = new AST (tile);

                    while (!ops.empty () && order[ops.top()->op] >= order[tile]) {
                        AST *op = getstk (ops);
                        op->b = getstk (tree), op->a = getstk (tree);
                        tree.push (op);
                    }

                    ops.push (temp);
                } else {

                    throw::logic_error ("invalid identifier");
                }

                it++;
                if (it - expr.begin() >= expr.size()) running = false;
            }

            while (!ops.empty ()) {
                AST *op = getstk (ops);
                op->b = getstk (tree), op->a = getstk (tree);
                tree.push (op);
            }

            return tree.size() ? tree.top() : nullptr;
          }

    public :
        vector<string> compile (string program) {
            return pass3 (pass2 (pass1 (program)));
        }
        vector<string> tokenize (string program) {

            static regex re ("[-+*/()\\[\\]]|[A-Za-z]+|\\d+");
            sregex_token_iterator it (program.begin (), program.end (), re);
            return vector<string> (it, sregex_token_iterator ());
        }

        AST *pass1 (string program) {     // Returns an un-optimized AST
            auto tokens = tokenize (program);
            vector<string>::iterator it = tokens.begin();

            int i = 1;
            for (it++ ; *it != "]"; it++)
                  args[*it] = i++;

            AST *ASTree = mktree ({it + 1, tokens.end()});

            return ASTree;
        }
        AST *pass2 (AST *ast) {           // Returns an AST with constant expressions reduced

            if (ast->op == "imm" || ast->op == "arg") return ast;
            ast->a = pass2 (ast->a), ast->b = pass2 (ast->b);

            if (ast->a->op == "imm" && ast->b->op == "imm") {
                ast->n = oper[ast->op] (ast->a->n, ast->b->n);
                ast->op = "imm";
                ast->a = nullptr, ast->b = nullptr;
            }

            return ast;
        };
        vector<string> pass3 (AST *ast) { // Returns assembly instructions

            vector<string> Asm, pref {"PO", "SW", "PO"};
            stack<AST *> tree = postorder(ast);

            while (!tree.empty()) {
                AST *node = getstk (tree);

                if (node->op == "imm") { 
                    Asm.push_back ("IM " + to_string (node->n));
                } else if (node->op == "arg") { 
                    Asm.push_back ("AR " + to_string (node->n));
                } else {
                    Asm.insert (Asm.end(), pref.begin(), pref.end());

                    if (node->op == "+") { 
                        Asm.push_back("AD");
                    } else if (node->op == "-") { 
                        Asm.push_back("SU");
                    } else if (node->op == "*") { 
                        Asm.push_back("MU");
                    } else if (node->op == "/") { 
                        Asm.push_back("DI");
                    }
                }

                Asm.push_back ("PU");
            }

            return Asm ;
          }
};

int simulate (const vector<string> &assembly, const vector<int> &argv) {
    int r0 = 0, r1 = 0;
    stack<int> istack;

    for (auto &ins : assembly) {
        //cout << r0 << " ";
        string code = ins.substr (0, 2);
        //cout << ins << " ";
             if (code == "IM") { r0 = stoi (ins.substr (3)); }
        else if (code == "AR") { r0 = argv.at (stoi (ins.substr (3))); }
        else if (code == "SW") { swap (r0, r1); }
        else if (code == "PU") { istack.push (r0); }
        else if (code == "PO") { r0 = istack.top (); istack.pop (); }
        else if (code == "AD") { r0 += r1; }
        else if (code == "SU") { r0 -= r1; }
        else if (code == "MU") { r0 *= r1; }
        else if (code == "DI") { r0 /= r1; }
    }
    return r0;
}

int main () {
    Timer clock;


    AST *ast1 = Bin(-,Arg(imm,1),Arg(imm,2));
    string prog = "[ x y z ] 2*3*x + 5*y - 3*z ";

    Compiler compiler;
    AST *ast = compiler.pass1 (prog);

    showtree (ast);
    //Assert::That (true, Equals (equals (*ast, *ast1)));

    //Test ();

    clock.stop();
    clock.get_duration();
}
/*
void Test () {

      { // AST_creation
        AST *ast = Bin(+,Arg(imm,1),Arg(imm,2));
        Assert::That (false, Equals (ast == 0));

        Compiler *compiler = new Compiler ();
        Assert::That (false, Equals (compiler == 0));
      }


     {
        string prog = "[ x y z ] ( 2*3*x + 5*y - 3*z ) / (1 + 3 + 2*2)";
        vector<string> tokens1 ({"[", "x", "y", "z", "]", "(", "2", "*", "3", "*", "x", "+", "5", "*", "y", "-", "3", "*", "z", ")", "/", "(", "1", "+", "3", "+", "2", "*", "2", ")"});

        Compiler compiler;
        auto tokens = compiler.tokenize (prog);
        Assert::That (tokens, EqualsContainer (tokens1));
    }

    {
        string prog = "[] 1";
        AST *ast1 = Arg(imm,1);

        Compiler compiler;
        AST *ast = compiler.pass1 (prog);
        Assert::That (true, Equals (equals (*ast, *ast1)));
    }

    {
        string prog = "[] 1 - 2";
        AST *ast1 = Bin(-,Arg(imm,1),Arg(imm,2));

        Compiler compiler;
        AST *ast = compiler.pass1 (prog);
        Assert::That (true, Equals (equals (*ast, *ast1)));
    }

    {
        string prog = "[x] x";
        AST *ast1 = Arg(arg,0);

        Compiler compiler;
        AST *ast = compiler.pass1 (prog);
        Assert::That (true, Equals (equals (*ast, *ast1)));
    }

    {
        string prog = "[x y z] z";
        AST *ast1 = Arg(arg,2);

        Compiler compiler;
        AST *ast = compiler.pass1 (prog);
        Assert::That (true, Equals (equals (*ast, *ast1)));
    }

    {
        string prog = "[x] x+2*5";
        AST *ast1 = Bin(+,Arg(arg,0),Bin(*,Arg(imm,2),Arg(imm,5)));

        Compiler compiler;
        AST *ast = compiler.pass1 (prog);
        Assert::That (true, Equals (equals (*ast, *ast1)));
    }

    {
        string prog = "[ x y z ] ( 2*3*x + 5*y - 3*z ) / (1 + 3 + 2*2)";
        AST *ast1 = Bin(/,Bin(-,Bin(+,Bin(*,Bin(*,Arg(imm,2),Arg(imm,3)),Arg(arg,0)),Bin(*,Arg(imm,5),Arg(arg,1))),Bin(*,Arg(imm,3),Arg(arg,2))),Bin(+,Bin(+,Arg(imm,1),Arg(imm,3)),Bin(*,Arg(imm,2),Arg(imm,2))));

        Compiler compiler;
        AST *ast = compiler.pass1 (prog);
        Assert::That (true, Equals (equals (*ast, *ast1)));
    }

    {
        string prog = "[x] x+2*5";
        AST *ast1 = Bin(+,Arg(arg,0),Arg(imm,10));

        Compiler compiler;
        AST *ast = compiler.pass2 (compiler.pass1 (prog));
        Assert::That (true, Equals (equals (*ast, *ast1)));
    }

    {
        Compiler compiler;
        AST *ast = compiler.pass2 (Bin(-,Arg(imm,10),Arg(imm,2)));
        Assert::That (true, Equals (equals (ast, Arg(imm,8))));
    }

    {
        Compiler compiler;
        AST *ast = compiler.pass2 (Bin(/,Arg(imm,10),Arg(imm,2)));
        Assert::That (true, Equals (equals (ast, Arg(imm,5))));
    }

    {
        string prog = "[ x y z ] ( 2*3*x + 5*y - 3*z ) / (1 + 3 + 2*2)";
        AST *ast1 = Bin(/,Bin(-,Bin(+,Bin(*,Arg(imm,6),Arg(arg,0)),Bin(*,Arg(imm,5),Arg(arg,1))),Bin(*,Arg(imm,3),Arg(arg,2))),Arg(imm,8));

        Compiler compiler;
        AST *ast = compiler.pass2 (compiler.pass1 (prog));
        Assert::That (true, Equals (equals (*ast, *ast1)));
    }

    {
        string prog = "[ x y z ] ( 2*3*x + 5*y - 3*z ) / (1 + 3 + 2*2)";

        Compiler compiler;
        auto c = compiler.compile (prog);
        Assert::That (simulate (c, {5, 4, 1}), Equals (5));
    }

    {
        string prog = "[ x y z ] ( 2*3*x + 5*y - 3*z ) / (1 + 3 + 2*2)";
        Compiler compiler;
        auto c = compiler.compile (prog);
        Assert::That (simulate (c, {4, 0, 0}), Equals (3));
        Assert::That (simulate (c, {4, 8, 0}), Equals (8));
        Assert::That (simulate (c, {4, 8, 16}), Equals (2));
    }

    {
        string prog = "[ x y z ] x - y - z + 10 / 5 / 2 - 7 / 1 / 7";
        Compiler compiler;
        auto c = compiler.compile (prog);
        Assert::That (simulate (c, {5, 4, 1}), Equals (0));
    }
};
*/
///////////////////////////////////////////////////////////////////////////////
