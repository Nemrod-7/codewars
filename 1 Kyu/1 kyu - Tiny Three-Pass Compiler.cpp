#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include <regex>
#include <algorithm>

using namespace std;

struct AST {
    string op;
    AST *a, *b;
    int n;

    AST (const string &label = "", const int num = 0) : op (label), n (num) {}
    AST (const string &label, AST *arg1, AST *arg2) : op (label), a (arg1), b (arg2) {}
    /*
    */
};
struct Compiler {

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

    AST *pass1 (string program) { // Returns an un-optimized AST
        auto tokens = tokenize (program);


        return 0;
    }
    AST *pass2 (AST *ast) { // Returns an AST with constant expressions reduced
        AST *next = new AST;


        return next;
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
        */
        return Asm;
    }
};

int main () {

    string prog = "[ x y z ] ( 2*3*x + 5*y - 3*z ) / (1 + 3 + 2*2)";

    //AST *op1 = new AST ("imm", 1), *op2 = new AST ("imm", 2);
    //AST *ast = new AST ("+", new AST ("imm", 1), new AST ("imm", 2) );

    Compiler now;
    vector<string> test = now.tokenize (prog);

    for (auto &it : test) {
      cout << it;
    //    cout << "[" << it << "]";
    }
    /*
    */
}

#define Bin(op,a,b) (new AST (#op, (a), (b)))
#define Arg(op,n) (new AST (#op, (n)))

bool equals (const AST &lhs, const AST &rhs) {
  return lhs.op == rhs.op && lhs.n == rhs.n && (lhs.a && rhs.a ? equals (*lhs.a, *rhs.a) : true) && (lhs.b && rhs.b ? equals (*lhs.b, *rhs.b) : true);
}
bool equals (const AST *lhs, const AST *rhs) { return equals (*lhs, *rhs); }

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
