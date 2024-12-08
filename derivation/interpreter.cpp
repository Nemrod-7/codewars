#include <iostream>

#include <vector>
#include <cmath>
#include <regex>
#include <tuple>
#include <complex>
#include <functional>

//#include "/home/Nemrod-7/include/Timer.hpp"
//#include "/home/Nemrod-7/include/Assert.hpp"
//#include "tests.hpp"

// clang++ -fPIC -I $(INCLUDE) -L $(LIB_PATH) -c eval.cpp -o eval.o
// clang++ -shared =I$(INCLUDE) -L $(LIB_PATH) -lglobal -Wl,-rpath,$(LIB_PATH) eval.o -o libeval.so
// clang++ -I ~/code/interpreter/include -L ~/code/interpreter/lib -leval eval.cpp

// simple interactive interpreter :
//
//function        ::= fn-keyword fn-name { identifier } fn-operator expression
//fn-name         ::= identifier
//fn-operator     ::= '=>'
//fn-keyword      ::= 'fn'
//
//expression      ::= factor | expression operator expression
//factor          ::= number | identifier | assignment | '(' expression ')' | function-call
//assignment      ::= identifier '=' expression
//function-call   ::= fn-name { expression }
//
//operator        ::= '+' | '-' | '*' | '/' | '%'
//
//identifier      ::= letter | '_' { identifier-char }
//identifier-char ::= '_' | letter | digit
//
//number          ::= { digit } [ '.' digit { digit } ]
//
//letter          ::= 'a' | 'b' | ... | 'y' | 'z' | 'A' | 'B' | ... | 'Y' | 'Z'
//digit           ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'

//////////////////////////////////////////eval.hpp/////////////////////////////////////////////////////////
struct symbol {
    std::complex<double> num;
    std::string str;

    symbol (std::string s) : num(0), str(s) {}
    symbol (double n) : num(n), str("") {}
    symbol (std::complex<double> n) : num(n), str("") {}
};

using inum = std::complex<double>;
using func_t = std::function<inum(inum)>;
using expression = std::vector<symbol>;

const std::regex operat ("^[-+*%/^=]$");
const std::regex identf ("_?[a-zA-Z]+_?|_[0-9]+");
const std::regex trigon ("^sin|cos|tan|cot|log|sec|csc$");
const std::regex number ("^-?\\d+(\\.\\d+)?$|^\\(-?\\d+(\\.\\d+)?,-?\\d+(\\.\\d+)?\\)$");

const expression zero = {symbol(0.0)}, one {symbol(1.0)}, two = {symbol(2.0)};

static std::map<std::string, std::pair<expression, expression>> values = {
    {"e", {{symbol(2.71828)},zero}},
    {"pi", {{symbol(3.14159265359)},zero}},
    {"tau", {{symbol(6.283185307179)},zero}}
}; // values base : <arg name, value>

static std::map<std::string, std::pair<expression,expression>> fbase; // lambda base : <func name, <args, function

class show {
    public:
        static std::string token (const symbol &tok) {
            std::stringstream os;

            if (tok.str == "") {
                if (tok.num.imag() == 0.0) {
                    os << tok.num.real();
                } else {
                    os << "(" << tok.num.real() << "+" << tok.num.imag() << "i)";
                }
            } else {
                os << tok.str;
            }

            return os.str();
        }
        static std::string expr (const expression &code) {
            std::stringstream os;

            for (int i = 0; i < code.size(); i++) {
                if (code[i].str == "(" && code[i+2].str == ")") {
                    os << token(code[i+1]);
                    i += 2;
                } else {
                    os << token(code[i]);
                }
            }

            return os.str();
        }
        static std::string vect (const expression &code) {
            std::stringstream os;

            for (auto &tok : code) {
                os << "[" << token(tok) << "]";
            }

            return os.str();
        }
        // void tree (const node &curr, bool isLeft = false, const std::string &prefix = "") {
        //     if (curr != nullptr) {
        //         std::cout << prefix;
        //         std::cout << (isLeft ? "├─" : "└─" );
        //
        //         if (curr->sym == "") {
        //             std::cout << "[" << curr->val << "]" << std::endl;
        //         } else {
        //
        //             std::cout << "[" << curr->sym << "]" << std::endl;
        //         }
        //
        //         showtree(curr->t1, true, prefix + (isLeft ? "│  " : "   "));
        //         showtree(curr->t2, false, prefix + (isLeft ? "│  " : "   "));
        //     }
        // }
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
std::complex<double> cot (const std::complex<double> &x) { return 1.0 / tan(x); }
std::complex<double> sec (const std::complex<double> &x) { return 1.0 / cos(x); }
std::complex<double> csc (const std::complex<double> &x) { return 1.0 / sin(x); }
std::complex<double> sqr (const std::complex<double> &x) { return x * x; }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool operator == (const symbol &lhs, const std::string &rhs) {
    return lhs.str == rhs;
}
bool operator == (const symbol &lhs, const symbol &rhs) {
    return lhs.num == rhs.num && lhs.str == rhs.str;
}
bool operator == (const expression &lhs, const expression &rhs) {

    if (lhs.size() != rhs.size()) return false;

    for (size_t i = 0; i < lhs.size(); i++) {
        if (lhs[i].num != rhs[i].num || lhs[i].str != rhs[i].str) return false;
    }

    return true;
}
const expression operator + (const std::string &lhs, const expression &rhs) {

    expression arm = {symbol(lhs)};

    for (auto &it : rhs) arm.push_back(it);

    return arm;
}
const expression operator + (const expression &lhs, const expression &rhs) {

    expression arm = lhs;

    for (auto &it : rhs) arm.push_back(it);

    return arm;
}
const expression operator + (const expression &lhs, const std::string &rhs) {

    expression arm;

    for (auto &it : lhs) arm.push_back(it);
    arm.push_back(symbol(rhs));

    return arm;
}
std::complex<double> operator ^ (const std::complex<double> &lhs, const std::complex<double> &rhs) {
    return pow(lhs,rhs);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
expression brace (const expression &lhs) {
    expression arm = {symbol("(")};

    for (auto &tok : lhs) arm.push_back(tok);
    arm.push_back(symbol(")"));

    return arm;
}
template<class T> T getstack(std::vector<T> &stack) {
    T top = stack.back();
    stack.pop_back();
    return top;
}

bool isnum (const expression &term) {
    return term.size() == 1 && term[0].str == "";
}

int order (const std::string &src) {
    if (src == "+" || src == "-") return 1;
    if (src == "*" || src == "/" || src == "%") return 2;
    if (src == "^") return 3;
    return 0;
}
bool precedence (const std::vector<std::string> &stack, const std::string &cell) {
    if (stack.empty()) return false;
    if (cell == "^") return order(stack.back()) > order(cell);
    return order(stack.back()) >= order(cell);
}

expression tokenize (const std::string &input) {

    const std::regex re ("=>|([0-9]+(\\.[0-9]+)?)|x|[-+*/%^()=]|(_?[a-zA-Z]+_?|sin|cos|tan|cot|log)");
    std::sregex_token_iterator iter (input.begin (), input.end (), re);
    std::vector<std::string> temp (iter, std::sregex_token_iterator ());
    expression code;

    for (size_t i = 0; i < temp.size(); i++) {
        std::string curr = temp[i];

        if (temp[i] == "-" && (i == 0 || regex_match(temp[i-1], operat))) {
            curr = "-" + temp[i + 1];
            i += 2;
        }

        if (regex_match(curr, number)) {
            code.push_back(stod(curr));
        } else {
            code.push_back(curr);
        }
    }

    return code;
}
expression getsub (expression::iterator &it, expression::iterator &nd) {
    int pile = 1;
    expression sub;

    for (it = it + 1; pile != 0 && it != nd ; it++) {
        symbol curr = *it;
        pile += (curr.str == "(") - (curr.str == ")");
        if (pile == 0) break;
        sub.push_back(curr);
    }

    return sub;
}
std::vector<expression> getargs (expression::iterator &it, const expression::iterator &end) {

    const std::string name = (*it).str;
    const size_t nargs = (fbase[name].first).size();
    std::vector<expression> args;

    while (it + 1 < end && args.size() < nargs) {
        it++;
        symbol cell = *it;
        expression sub = {cell};

        /*
           if (cell.str == "(") {
        //while ((*it).str != ")") {
        //  sub += *++it;
        //}
        } else
        */
        if (fbase.find(cell.str) != fbase.end()) {
            std::vector<expression> nxt = getargs (it, end);

            for (auto &it2 : nxt) {
                sub = sub + it2;
            }
        }

        args.push_back(sub);
    }


    return args;
}

expression add (const expression &a, const expression &b) {

    if (a == zero) return b;
    if (b == zero) return a;
    if (isnum(a) && isnum(b)) return { symbol(a[0].num + b[0].num) };

    return brace(a) + "+" + brace(b);
}
expression sub (const expression &a, const expression &b) {

    if (a == b) return zero;
    if (isnum(a) && isnum(b)) return { symbol(a[0].num - b[0].num) };

    return brace(a) + "-" + brace(b);
}
expression mul (const expression &a, const expression &b) {

    if (a == zero || b == zero) return zero;
    if (a == one) return b;
    if (b == one) return a;
    if (isnum(a) && isnum(b)) return { symbol(a[0].num * b[0].num) };

    return brace(a) + "*" + brace(b);
}
expression div (const expression &a, const expression &b) {

    if (a == zero) return zero;
    if (b == one) return a;
    if (a == b) return one;
    if (isnum(a) && isnum(b)) return { symbol(a[0].num / b[0].num) };

    return brace(a) + "/" + brace(b);
}
expression exp (const expression &a, const expression &b) {

    if (a == one || b == zero) return one;
    if (b == one) return a;
    if (isnum(a) && isnum(b)) return { symbol(a[0].num ^ b[0].num) };

    return brace(a) + "^" + brace(b);
}
expression mod (const expression &a, const expression &b) {

    if (a == one || b == zero) return one;
    if (b == one) return a;
    if (isnum(a) && isnum(b)) return { symbol( (int) real(a[0].num) % (int) real(b[0].num)) };

    return brace(a) + "%" + brace(b);
}

std::pair<expression,expression> operate (std::vector<std::pair<expression, expression>> &heap, std::vector<std::string> &oper) {

    auto [f2, d2] = getstack(heap);
    auto [f1, d1] = getstack(heap);
    std::string op = getstack(oper);
    std::pair<expression,expression> res;

    if (op == "+") {
        res = {add(f1, f2), add(d1, d2)} ;
    } else if (op == "-") {
        res = {sub(f1, f2), sub(d1, d2)} ;
    } else if (op == "*") {
        res = {mul(f1, f2), add(mul(f1,d2), mul(d1,f2))} ;
    } else if (op == "/") {
        expression num =  sub(mul(d1,f2),mul(f1,d2)), den =  exp(f2, two);

        res = {div(f1,f2), div(num,den)} ;
    } else if (op == "^") {
        expression log = "log" + brace(f1);
        expression inner = add( mul( d1, div(f2,f1) ), mul(d2, log));

        res = {exp(f1,f2), mul(exp(f1,f2), inner)} ;
    } else if (op == "%") {

        res = {mod(f1,f2), {zero}} ;
    } else {
        throw::std::logic_error ("Invalid operator.");
    }

    return res;
}
std::pair<expression,expression> interpreter (expression expr, const std::complex<double> &val = {0,0}) {
    // expression -> { fonction, derivative }
    bool running = true;
    expression fx;
    expression::iterator it = expr.begin(), end = expr.end();
    std::vector<std::string> oper;
    std::vector<std::pair<expression,expression>> heap;

    while (running) {
        symbol curr = *it;

        if (expr[0].str == "fn") {
            expression::iterator it = expr.begin() + 1, end = expr.end();
            std::string name = expr[1].str;
            auto mid = find (it, end, "=>");

            for (auto fn_ex = it + 1; fn_ex != mid; fn_ex++) {
                if (find (mid, end, *fn_ex) == end)
                    throw::std::logic_error ("Unknown identifier.");

                if (find (it + 1, fn_ex, *fn_ex) != fn_ex)
                    throw::std::logic_error ("Invalid function.");
            }

            fbase[name] = {getsub (it, mid), getsub (it, end)};
            return {zero,zero};
        } else if (regex_match(curr.str, trigon)) {
            it++;
            auto [var, dx] = interpreter(getsub(it,end), val);
            expression nxt = brace(var);

            if (curr.str == "log") {
                fx = (isnum(var)) ? expression {symbol(log(var[0].num))} : curr.str + nxt;
                heap.push_back( { fx , div( dx, nxt)});
            } else if (curr.str == "sin") {
                fx = (isnum(var)) ? expression {symbol(sin(var[0].num))} : curr.str + nxt;
                heap.push_back( { fx , mul( dx, "cos" + nxt)});
            } else if (curr.str == "cos") {
                fx = (isnum(var)) ? expression {symbol(cos(var[0].num))} : curr.str + nxt;
                heap.push_back( { fx , mul( sub(zero, dx), "sin" + nxt) });
            } else if (curr.str == "tan") {
                fx = (isnum(var)) ? expression {symbol(tan(var[0].num))} : curr.str + nxt;
                heap.push_back( { fx , div( dx, exp("cos" + nxt, two)) });
                //heap.push_back( { fx , mul(dx, exp("sec" + nxt, two)) });
            } else if (curr.str == "cot") {
                fx = (isnum(var)) ? expression {symbol(cot(var[0].num))} : curr.str + nxt;
                //fx = (isnum(nxt)) ? expression {symbol(1.0 / tan(nxt[0].num))} : curr.str + nxt;
                heap.push_back( { fx , div( sub(zero, dx), exp("sin" + nxt, two) ) });
                //heap.push_back( { fx , div( sub(zero, dx), exp("csc" + nxt, two) ) });
            }

            else if (curr.str == "sec") {
                fx = (isnum(var)) ? expression {symbol(sec(nxt[0].num))} : curr.str + nxt;
                heap.push_back( { fx , mul(dx, mul("sec" + nxt, "tan" + nxt)) });
            } else if (curr.str == "csc") {
                fx = (isnum(var)) ? expression {symbol(sec(nxt[0].num))} : curr.str + nxt;
                heap.push_back( { fx , mul(dx, mul("cot" + nxt, "csc" + nxt)) });
            } else {
                throw::std::logic_error ("Invalid trigonometric function.");
            }

        } else if (regex_match(curr.str, identf)) {
            if (find (it, end, "=") != end) {
                if (fbase.find (curr.str) != fbase.end())
                    throw::std::logic_error ("Invalid initializer.");

                values[curr.str] = interpreter ({it + 2, end});
                return values[curr.str];
            } else if (values.find (curr.str) != values.end()) {

                heap.push_back (values[curr.str]);
            } else if (fbase.find (curr.str) != fbase.end()) {
                std::vector<expression> args = getargs (it, end);
                auto &[values, lmda] = fbase[curr.str];
                expression next;

                if (args.size() != values.size())
                    throw::std::logic_error ("Invalid number of argument.");

                for (symbol &tok : lmda) {
                    if (regex_match (tok.str, identf)) {
                        int pos = find (values.begin(), values.end(), tok.str) - values.begin();
                        next = next + interpreter(args[pos]).first;
                    } else {
                        next.push_back(tok);
                    }
                }

                heap.push_back(interpreter(next));
            } else if (curr.str == "x") {
                fx = val == inum{0,0} ? expression {curr} : expression {symbol(val)};
                heap.push_back({fx, {symbol(1.0)}});
            } else {
                throw::std::logic_error ("Invalid identifier.");
            }
        } else if (curr.str == "") { // if symbol is a number
            heap.push_back({{curr}, {symbol(0.0)}});
        } else if (curr.str == "(") {
            heap.push_back(interpreter(getsub(it,end), val));
        } else if (regex_match(curr.str, operat)) {
            while (precedence(oper, curr.str)) {
                heap.push_back(operate(heap,oper));
            }
            oper.push_back(curr.str);
        } else {
            throw::std::logic_error ("Invalid symbol.");
        }

        it++;
        if (it >= end) running = false;
    }

    while (!oper.empty()) {
        heap.push_back(operate(heap,oper));
    }

    return getstack(heap);
}

std::tuple<func_t,func_t,func_t> differentiate (const std::string &input) { // for testing purpose only

    //cout << "expression : [" <<  input << "]\n" << flush;
    expression pass0 = tokenize(input);
    expression pass1 = interpreter(pass0).second;
    expression pass2 = interpreter(pass1).second;

    return {
        [pass0](inum x) { return interpreter(pass0, x).first[0].num; },
            [pass1](inum x) { return interpreter(pass1, x).first[0].num; },
            [pass2](inum x) { return interpreter(pass2, x).first[0].num; },
    };
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace txt {

    const std::vector<std::string> help = {
        "",

      "Commands :\n"
          //"exit derivate interpret help clear save show rules time pause\n"
          "general : exit clear save load\n"
          "mode    : evaluate derivate brainfuck\n"
          "info    : help rules mode values base\n"
          "modules : time rain sudoku brainfuck\n"
          "variable: speed\n"
          "\n\n",

        "rules of differentiation : \n\n"
        "Let f be a function.\n"
        "The derivative function, denoted by f′, is the function whose domain\n"
        "consists of those values of x such that the following limit exists:\n"
        "f′(x) = lim h→0 of (f(x + h) − f(x)) / h.\n\n"
        "cst : 1     => 0\n"
        "lin : x     => 1\n"
        "add : a + b => a' + b'\n"
        "sub : a - b => a' - b'\n"
        "mul : a * b => a.b' + a'.b\n"
        "div : a / b => (a'* b − b'* a) / (b * b)\n"
        "exp : x ^ y => x^y . (x'.(y/x) + y'.log(x))\n"
        "log : log x => x' / x\n"
        "sin : sin x => x'.cos x\n"
        "cos : cos x => -x'.sin x\n"
        "tan : tan x => x' / (cos(x))^2\n"
        "cot : cot x => -x' / (sin(x))^2\n"
        "sec : sec x => sec x . tan x . x'\n"
        "csc : csc x => cot x . csc x . x'\n\n" ,

        "Constants :\n" "e pi tau\n\n"
        "Functions :\n"
        "cos sin tan cot sec csc \n"
        "log rad deg sqr         \n"
        //"     cos    acos     deg    abs\n"
        //"     sin    asin    sqrt    rad\n"
        //"     tan    atan     sqr    deg\n\n"
        "Operators :\n"
        "+ - * % / ^ = =>\n\n"
        "lambdas :\n"
        "fn 'name' 'parameters' => 'function'\n"
        "ex : fn avg x y => (x + y) / 2      \n\n",

        "brainfuck rules: \n\n"
            "> : move pointer forward\n"
            "< : move pointer backward\n"
            "+ : increment cell under pointer\n"
            "- : decrement cell under pointer\n"
            ". : output cell under pointer\n"
            ", : input celll under pointer\n"
            "[ : loop until cell equal zero\n"
            "] : move backward if cell is not empty\n",
  };

    const std::string sav =
        "\nsave expression to the base\nformat :\n\n"
        "fn [name](param) => <expression> :: save as lambda.\n"
        "   [name]        =  <expression> :: save as value.\n\n"
        "ex: fn radian x => x * 360.0 * pi\n\n";

    const std::vector<std::string> menu = {"File", "Place","Help","Base","Mode","clear","Time","Module"}; // main menu

    std::vector<std::vector<std::string>> sub { // sub menus
        {"Save","Load","Exit"},{}, {"commands$$", "interpreter"  ,"derivation", "brainfuck$"}, {"values$", "lambdas"}, {"evaluate","derivate$","brainfuck", "sudoku$$$"}, {"input$$", "console"}, {}, {"Rain$$$$", "Password"}
    };

    const std::vector<std::string> mode {"no mode selected", "evaluate", "derivate", "brainfuck", "sudoku"};
    const std::vector<std::string> speed {"disable", "instant", "fast", "medium", "slow", "sluggish"};
    const std::vector<std::string> difficulty = {"beginner", "easy", "medium", "hard", "hell"};
    const std::vector<std::string> sudosub = {"solve", "new", "reset", "level"}; // main menu
}

class interface {
    private:

    public:
        enum mode_t {no_mode, evaluate, derivate, brainfck, sudoku};

};
/*
int main () {

    // Timer clock;
    // clock.start();

    fbase["echo"] = {tokenize("x"), tokenize("x")};
    fbase["inc"] = {tokenize("x"), tokenize("x + 1")};
    fbase["sqr"] = {tokenize("x"), tokenize("x * x")};
    fbase["add"] = {tokenize("x y"), tokenize("x + y")};
    fbase["sub"] = {tokenize("x y"), tokenize("x - y")};
    fbase["mul"] = {tokenize("x y"), tokenize("x * y")};
    fbase["div"] = {tokenize("x y"), tokenize("x / y")};
    fbase["avg"] = {tokenize("x y"), tokenize("(x + y) / 2")};
    fbase["rad"] = {tokenize("x y"), tokenize("x * pi / 180.0")};
    fbase["deg"] = {tokenize("x y"), tokenize("x * 180.0 / pi")};

    enum mode_t { no_mode, evaluate, derivate, brainfck, sudoku };

    std::string input;
    std::stringstream iss;
    //std::cout << interpret("avg 4 2") << "\n";
    int mode = no_mode;
    bool running = true;

    while (running) {
        std::cout << "enter input > ";
        std::getline(std::cin, input);

        if (input == "exit" || input == "quit") {
            running = false;
        } else if (input == "evaluate") {
            mode = evaluate;
        } else if (input == "derivate") {
            mode = derivate;
        } else if (input == "brainfuck") {
            mode = brainfck;
        }

        else if (input == "help") {
            std::cout << txt::help[0];
        } else if (input == "rules") {
            std::cout << txt::help[1];
        } else if (input == "mode") {
            std::cout << txt::mode[mode] << "\n";
        } else if (input == "values") {
            for (auto &[name, value] : values) {
                std::cout << name << " => " << show::expr(value.first) << "\n";
            }
        } else if (input == "base") {
            for (auto &[name,func] : fbase) {
                std::cout << name << " => " << show::expr(func.first) << " | " << show::expr(func.second) << "\n";
            }
        }

        else {

            try {
                expression code = tokenize(input);
                auto [fx,dx] = interpreter(code);

                switch (mode) {
                    case evaluate : std::cout << "fx : " << show::expr(fx) << "\n"; break;
                    case derivate : std::cout << "dx : " << show::expr(dx) << "\n"; break;
                    default: std::cout << "no mode selected\n"; break;
                }
            } catch (const std::exception &x) {
                std::cout << "error : " << x.what() << "\n";
            }
        }
    }

    // clock.stop();
    // clock.get_duration();
}
*/
