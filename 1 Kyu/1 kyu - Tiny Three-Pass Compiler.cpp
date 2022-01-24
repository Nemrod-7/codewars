#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stack>
//#include <regex>
//#include <algorithm>
#include <functional>
#include <chrono>

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
    //AST (const string &label, AST *arg1, AST *arg2) : op (label), a (arg1), b (arg2) {}
};
/////////////////////////////////Assert/////////////////////////////////////////
class Assert {
    public :
        template<class T> static void That (const T& a, const T& b) {
            if (a != b) {
                cout << "actual : " << a << " expected : " << b;
                cout << endl;
            }
        }
};
template<class T> T Equals (const T& entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////
class Timer {
    //using time = chrono::steady_clock;
    private :
        chrono::steady_clock::time_point alpha, end;
        chrono::duration<double> elapsed;
        uint64_t index;

        void update () {
            end = chrono::steady_clock::now ();
            elapsed = end - alpha;
        }
    public :
        Timer () {
            alpha = chrono::steady_clock::now ();
            index = 0;
        }
        void start () { alpha = chrono::steady_clock::now ();}
        void stop () { update();}
        void get_duration () {

            cout << "\nDuration "
                      <<fixed<< elapsed.count()
                      << " ms" << endl;
        }
        bool running (double total) {
            update();
            index++;
            if (elapsed.count() < total) return true;
                cout << "index :: " << index << endl;
            return false;
        }
};
////////////////////////////////////tools///////////////////////////////////////
#define Bin(op,a,b) (new AST (#op, (a), (b)))
#define Arg(op,n) (new AST (#op, (n)))

bool equals (const AST &lhs, const AST &rhs) {
  return lhs.op == rhs.op && lhs.n == rhs.n && (lhs.a && rhs.a ? equals (*lhs.a, *rhs.a) : true) && (lhs.b && rhs.b ? equals (*lhs.b, *rhs.b) : true);
}
bool equals (const AST *lhs, const AST *rhs) { return equals (*lhs, *rhs); }
class Display {
    public :
        static void node (const AST *now) {
            if (oper[now->op]) {
                cout << "Bin(" << now->op << ", ";
            } else {
                cout << "Arg(" << now->op << "," << now->n;
            }
        }
        static void tree (const AST *now) {
            if (now != nullptr) {
                node (now);
                tree (now->a);
                tree (now->b);
                cout << ")";
            }
        }
};
int simulate (const vector <string> &assembly, const vector <int> &argv) {
  int r0 = 0, r1 = 0;
  stack <int> istack;
  for (auto &ins : assembly) {
    string code = ins.substr (0, 2);
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

////////////////////////////////////////////////////////////////////////////////
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
        int probe (AST* node) {           // search for max level
            if (node == nullptr) return -1;
            return max (probe (node->a), probe (node->b)) + 1;
          }
        void reduce (AST *curr) {         // reduce constant expression if possible

            if (oper[curr->op]) {

                AST *a = curr->a, *b = curr->b;

                if (a->op == "imm" && b->op == "imm") {
                    curr->n = oper[curr->op] (a->n, b->n);
                    curr->op = "imm";
                    curr->a = nullptr, curr->b = nullptr;
                    free (a), free (b);
                }
            }
            /*
            Display::node (curr);
            */
        }
        void reach (AST *node, int depth, int dest) {

            if (node != nullptr) {
                if (depth == dest) reduce (node);

                reach (node->a, depth + 1, dest);
                reach (node->b, depth + 1, dest);
            }
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
                    //cout << "imm" << " " << stoi (tile) << endl;
                } else if (args[tile]) {
                    //cout << "arg" << " " << args[tile] - 1 << endl;
                    tree.push ((new AST ("arg", args[tile] - 1)));
                } else if (oper[tile]) {

                    AST *temp = new AST (tile);
                    /*
                    while (!ops.empty ()) {

                    }
                    */
                    ops.push (temp);
                } else {
                      //cout << "[" << tile << "]";
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
            //static regex re ("[-+*/()[\\]]|[A-Za-z]+|\\d+");
            //sregex_token_iterator it (program.begin (), program.end (), re);
            //return vector<string> (it, sregex_token_iterator ());
            string::iterator it = program.begin();
            vector<string> tok;

            while (*it) {
                if (*it != ' ') tok.push_back ({*it});
                it++;
            }
            return tok;
        }

        AST *pass1 (string program) {     // Returns an un-optimized AST
            auto tokens = tokenize (program);
            vector<string>::iterator it = tokens.begin();
            int i = 1;
            /*
            // Each node is of type 'AST' and has the following fields:
            // 'string op', 'AST* a', 'AST* b', and 'int n'

            AST ("+", a, b)       // add subtree a to subtree b
            AST ("-", a, b)       // subtract subtree b from subtree a
            AST ("*", a, b)       // multiply subtree a by subtree b
            AST ("/", a, b)       // divide subtree a from subtree b
            AST ("arg", n)        // reference to n-th argument, n integer
            AST ("imm", n)        // immediate value n, n integer

            [ x ] x + 2*5  ==>  new AST ("+", new AST ("arg", 0), new AST ("*", new AST ("imm", 2), new AST ("imm", 5)))

            -----------------------
            args  : [ x ]
            expr  : x + 2*5
            -----------------------

                        ("+", a, b)
                        /          \
                  ("*", a, b)     ("arg", 0)
                  /        \
              ("imm", 2)  ("imm", 5)

            */

            for (it++ ; *it != "]"; it++) {
                  args[*it] = i++;
            }

            AST *ASTree = mktree ({it + 1, tokens.end()});

            //Display::tree (ASTree);

            return ASTree;
        }
        AST *pass2 (AST *ast) {           // Returns an AST with constant expressions reduced
            /*
            new AST ("+", new AST ("arg", 0), new AST ("*", new AST ("imm", 2), new AST ("imm", 5)))

            =>  new AST ("+", new AST ("arg", 0), new AST ("imm", 10))
            */

            AST *head = ast;
            int depth = probe (ast), index = depth;

            while (index-->0) {
                reach (head, 0, index);
            }

            //Display::tree (head);
            return head;
        };
        vector<string> pass3 (AST *ast) { // Returns assembly instructions
        vector<string> Asm;
        /*
        "IM n"     // load the constant value n into R0
        "AR n"     // load the n-th input argument into R0
        "SW"       // swap R0 and R1
        "PU"       // push R0 onto the stack
        "PO"       // pop the top value off of the stack into R0
        "AD"       // add R1 to R0 and put the result in R0
        "SU"       // subtract R1 from R0 and put the result in R0
        "MU"       // multiply R0 by R1 and put the result in R0
        "DI"       // divide R0 by R1 and put the result in R0

        ex : new AST ("+", new AST ("arg", 0), new AST ("imm", 10)) => [ "IM 10", "SW", "AR 0", "AD" ]

        */
        return Asm;
    }
};

int main () {
    Timer clock;


    string prog = "[x] x + 2*5";  // AST *ast1 = Bin(+,Arg(arg,0),Bin(*,Arg(imm,2),Arg(imm,5)));
    /*
    string prog = "[x] x";      // AST *ast1 = Arg(arg,0);
    string prog = "[] 1 - 2";   // AST *ast1 = Bin(-,Arg(imm,1),Arg(imm,2));
    string prog = "[] 1";       // AST *ast1 = Arg(imm,1);
    string prog = "[x y z] z";  // AST *ast1 = Arg(arg,2);
    string prog = "[ x y ] ( x + y ) / 2";
    */


    Compiler now;
    AST *ASTree = now.pass1 (prog);

    ASTree = now.pass2 (ASTree);

    Display::tree (ASTree);

    clock.stop();
    clock.get_duration();
}

void Test () {
    /*

    { // AST_creation
        AST *ast = Bin(+,Arg(imm,1),Arg(imm,2));
        Assert::That (false, Equals (ast == 0));
    }

    Compiler *compiler = new Compiler ();
    Assert::That (false, Equals (compiler == 0));

    */

    /*
     {
        string prog = "[ x y z ] ( 2*3*x + 5*y - 3*z ) / (1 + 3 + 2*2)";
        vector <string> tokens1 ({"[", "x", "y", "z", "]", "(", "2", "*", "3", "*", "x", "+", "5", "*", "y", "-", "3", "*", "z", ")", "/", "(", "1", "+", "3", "+", "2", "*", "2", ")"});

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
    */
};
