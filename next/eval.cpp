
#include <vector>
#include <string>
#include <map>
#include <stack>
#include <regex>
#include <functional>

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
    AST (const string &label, AST *arg1, AST *arg2) : op (label), a (arg1), b (arg2) {}
};

class Compiler {
    private :
        map<string, int> args;

        bool isnum (const string &ex) {

            if (oper[ex]) return false;

            for (auto &it : ex) {
                if (isalpha (it)) return false;
                if (it == '(' || it == ')') return false;
            }
            return true;
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
            // Turn a program string into a vector of tokens.  Each token
            // is either '[', ']', '(', ')', '+', '-', '*', '/', a variable
            // name or a number (as a string)
            static regex re ("[-+*/()[\\]]|[A-Za-z]+|\\d+");
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
            /*
            stack<AST *> s1 = postorder (ast);
            while (!s1.empty()) {
                reduce (getstk (s1));
            }
            */

            return ast;
        };
        vector<string> pass3 (AST *ast) { // Returns assembly instructions

            vector<string> Asm, pref {"PO", "SW", "PO"};
            stack<AST *> tree = postorder(ast);

            while (!tree.empty()) {
                AST *node = getstk (tree);

                if (node->op == "imm") { Asm.push_back ("IM " + to_string (node->n));
                } else if (node->op == "arg") { Asm.push_back ("AR " + to_string (node->n));
                } else {
                    Asm.insert (Asm.end(), pref.begin(), pref.end());

                    if (node->op == "+") { Asm.push_back("AD");
                    } else if (node->op == "-") { Asm.push_back("SU");
                    } else if (node->op == "*") { Asm.push_back("MU");
                    } else if (node->op == "/") { Asm.push_back("DI");
                    }
                }

                Asm.push_back ("PU");
            }

            return Asm ;
          }
};

int main () {

}
