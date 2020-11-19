#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <chrono>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
typedef string test_t;
class Assert {
    public :
        static void That (test_t actual, test_t expression) {
            if (actual != expression) {
                cout << "actual : " << endl << actual;
                cout << " expected : "<<endl << expression;

                cout << endl;
            }
        }
};
test_t Equals (test_t entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////

#define PTR(x) (((x) == '>') || ((x) == '<'))
#define BYT(x) (((x) == '+') || ((x) == '-'))
#define IO(x) (((x) == '.')  || ((x) == ','))
#define LOOP(x) (((x) == '[')|| ((x) == ']'))
vector<string> c_syntax{
  {"if (*p) do {\n"},    // FRWD [
  {"*p += "},            // ADD  +
  {"p -= "},             // DEC  <
  {"Error!"},            // CENTER
  {"p += "},             // INC  >
  {"*p -= "},            // SUB  -
  {"} while (*p);\n"},   // BACK ]
  {"*p = getchar();\n"}, // IN   ,
  {"putchar(*p);\n"}     // OUT  .
};
map<char, int> commands {{'[',-3},{'+',-2},{'<',-1},{' ',0},{'>',1},{'-',2},{']',3},{',',4},{'.',5}};

string reduce (string source) {

    string buffer;

    for (auto code : source)
        if (commands[code])
            if (buffer.size() < 1)
                buffer.push_back(code);
            else {
                int now = commands [buffer.back()], next = commands[code];

                if (now + next == 0 && now != 3)
                    buffer.pop_back();
                else
                    buffer.push_back(code);
            }

    return buffer;
}
bool validParentheses (string arr) {

    int pile = 0;
    for (auto it : arr) {
        if (pile < 0) return false;
        if (it == '[') pile++;
        if (it == ']') pile--;
  }

    return ((pile == 0) ? true : false);
}
string brainfuck_to_c (string source) {

    if (validParentheses (source) == false)
        return "Error!";

    string srcode = reduce (source); // sanitized
    ostringstream os;
    string out;
    int index = 0, stack = 0, code , next, tab, loop = 0;

    while (code = srcode[index]) {
        tab = loop, next = srcode[index + 1];

        if (BYT (code) || PTR (code)) {
            if (next != code) {
                while (tab-->0)
                    os << "  ";

                os << c_syntax[commands[code]+3] << stack + 1 << ";\n"; // out += c_syntax[commands[code]+3] + std::to_string(stack + 1) + ";\n";
                stack = 0;
            }
            else
                stack++;
        }
        if (IO (code) || LOOP (code)) {
            if (code == ']') tab--, loop--;
            if (code == '[') loop++;

            while (tab-->0)
                os << "  ";

            os << c_syntax[commands[code]+3];
        }

        index++;
    }

    return os.str();
}

int main () {

    auto start = std::chrono::high_resolution_clock::now();

    Test();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Process took " << elapsed.count()  << " ms" << std::endl;

}
void dotest (string code, string expected) {
    Assert::That (brainfuck_to_c(code), expected);
}
void Test () {

  dotest("++++", "*p += 4;\n");
  dotest("----", "*p -= 4;\n");

  dotest(">>>>", "p += 4;\n");
  dotest("<<<<", "p -= 4;\n");

  dotest(".", "putchar(*p);\n");
  dotest(",", "*p = getchar();\n");

  dotest("[[[]]", "Error!");

  dotest("[][]", "");

  dotest("[.]", "if (*p) do {\n  putchar(*p);\n} while (*p);\n");
  dotest ("[[[]]", "Error!");
  dotest ("[][]", "");
  dotest ("[.]", "if (*p) do {\n  putchar(*p);\n} while (*p);\n");
  //dotest ("[>>]","if (\*p) do {\n} while (\*p);\n");
  //dotest (""[>>[<<]]","if (\*p) do {\np += 2;\nif (\*p) do {\np -= 2;\n} while (\*p);\n} while (\*p);\n");
  /*
  */

}
