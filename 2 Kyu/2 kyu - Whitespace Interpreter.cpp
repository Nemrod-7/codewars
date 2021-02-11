#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <bitset>
#include <stack>

using namespace std;

const char space = 32, tab = 9, line = 10;

class Debug {
    public :
        static void display (const string &src) {
            for (auto &c : src)
                switch (c) {
                    case space : cout << "[space]"; break;
                    case tab : cout << "[tab]"; break;
                    case line : cout << "[lf]"; break;
                    default : break;
                }
            cout << endl;
        }
        static string convert (string s) { // To help with debugging

          transform (s.begin(), s.end(), s.begin(), [] (char c) {
              return (c == ' ') ? 's' : ((c == '\n') ? 'n' : 't');
          });

          return s;
        }
};
class interpreter {
    private :
        int it = 0;
        string code, op;
        stack<int> S;
};
/*
IMP [tab][space] - Arithmetic
    [space][space]: Pop a and b, then push b+a.
    [space][tab]: Pop a and b, then push b-a.
    [space][line-feed]: Pop a and b, then push b*a.
    [tab][space]: Pop a and b, then push b/a*. If a is zero, throw an error.
    *Note that the result is defined as the floor of the quotient.
    [tab][tab]: Pop a and b, then push b%a*. If a is zero, throw an error.
    *Note that the result is defined as the remainder after division and sign (+/-) of the divisor (a).

IMP [tab][tab] - Heap Access
    [space]: Pop a and b, then store a at heap address b.
    [tab]: Pop a and then push the value at heap address a onto the stack.


IMP [line-feed] - Flow Control
    [space][space] (label): Mark a location in the program with label n.
    [space][tab] (label): Call a subroutine with the location specified by label n.
    [space][line-feed] (label): Jump unconditionally to the position specified by label n.
    [tab][space] (label): Pop a value off the stack and jump to the label specified by n if the value is zero.
    [tab][tab] (label): Pop a value off the stack and jump to the label specified by n if the value is less than zero.
    [tab][line-feed]: Exit a subroutine and return control to the location from which the subroutine was called.
    [line-feed][line-feed]: Exit the program.

Parsing Numbers :
    Numbers begin with a [sign] symbol : [tab] -> negative, [space] -> positive.
    Numbers end with a [terminal] symbol: [line-feed].
    Between the sign symbol and the terminal symbol are binary digits [space] -> binary-0, or [tab] -> binary-1.
    A number expression [sign][terminal] will be treated as zero.
    The expression of just [terminal] should throw an error.

Parsing Labels :
    Labels begin with any number of [tab] and [space] characters.
    Labels end with a terminal symbol: [line-feed].
    Unlike with numbers, the expression of just [terminal] is valid.
    Labels must be unique.
    A label may be declared either before or after a command that refers to it.

Input/Output : As stated earlier, there commands may read data from input or write to output.

Parsing Input :
Whitespace will accept input either characters or integers. Due to the lack of an input stream mechanism, the input will be passed as a string to the interpreter function.
Reading a character involves simply taking a character from the input stream.
Reading an integer involves parsing a decimal or hexadecimal number from the current position of the input stream, up to and terminated by a line-feed character.
The original implementation being in Haskell has stricter requirements for parsing an integer.
The Javascript and Coffeescript implementations will accept any number that can be parsed by the parseInt function as a single parameter.
The Python implementations will accept any number that can be parsed by the int function as a single parameter.
The Java implementations will use an InputStream instance for input. For InputStream use readLine if the program requests a number and read if the program expects a character.
An error should be thrown if the input ends before parsing is complete. (This is a non-issue for the Haskell implementation, as it expects user input)

Writing Output :
    For a number, append the output string with the number's string value.
    For a character, simply append the output string with the character.
    The Java implementations will support an optional OutputStream for output. If an OutputStream is provided, it should be flushed before and after code execution and filled as code is executed. The output string should be returned in any case.

Commands :
    Notation: n specifies the parameter, [number] or [label].
    Errors should be thrown for invalid numbers, labels, and heap addresses,
    or if there are not enough items on the stack to complete an operation (unless otherwise specified).
    In addition, an error should be thrown for unclean termination.


*/

int btoi (string str) {

    int sign = str[0] == space ? 1 : -1, i = 1;
    //Debug::display (str);
    string num;
    while (str[i] != line) {
        num += str[i] == tab ? '1' : '0';
        i++;
    }

    return bitset<8> (num).to_ulong() * sign;
}

void stack_op (const string &code, int &it, stack<int> &S) {
  //it++;
  switch (code[it++]) {
      case space : {
          //it++;
          int nxt = code.find (line, it) + 1;
          string num = code.substr (it, nxt - it);
          S.push (btoi (num));
          it = nxt;
          break;
      }
      case tab : { // stack manip sub
          /*
          it++;
          if (code[it] == space) { // (number): Duplicate the nth value from the top of the stack and push onto the stack.

          }
          if (code[it] == line) { // (number): Discard the top n values below the top of the stack from the stack. (For n<**0** or **n**>=stack.length, remove everything but the top value.)

          }
          */
      }
      case line : {
          /*
          it++;
          if (code[it] == space) { // [line-feed][space]: Duplicate the top value on the stack.

          }
          if (code[it] == tab) { // [line-feed][tab]: Swap the top two value on the stack.

          }
          if (code[it] == line) { // [line-feed][line-feed]: Discard the top value on the stack.

          }
          */
      }
  }

}
void arith_op (const string &code, int &it) {

}
void heap_op (const string &code, int &it) {

}
char io_op (const string &code, int &it, stack<int> &S) {
  //it++;
  string op = code.substr (it + 1, 2), os, is;
  int val;
  it += 3;
  //Debug::display (op);
  if (op == "  ") { // [space][space]: Pop a value off the stack and output it as a character.
      val = S.top();
      S.pop();
      //os += val + 'A';
      return val + 'A';
  }
  if (op == " \t") { // [space][tab]: Pop a value off the stack and output it as a number.
      val = S.top();
      S.pop();
      //os += val + '0';
      return (val + '0');
  }
  if (op == "\t ") { // [tab][space]: Read a character from input, a, Pop a value off the stack, b, then store the ASCII value of a at heap address b.

  }
  if (op == "\t\t") { // [tab][space]: Read a character from input, a, Pop a value off the stack, b, then store the ASCII value of a at heap address b.

  }

}
void flow_op (const string &code, int &it) {
    it++;
    string op = code.substr (it, 2);
    //Debug::display (op);
    if (op == "\n\n") { // [line-feed][line-feed]: Exit the program.
        it = code.size() - 1;
    }
  /*
  [space][space] (label): Mark a location in the program with label n.
  [space][tab] (label): Call a subroutine with the location specified by label n.
  [space][line-feed] (label): Jump unconditionally to the position specified by label n.
  [tab][space] (label): Pop a value off the stack and jump to the label specified by n if the value is zero.
  [tab][tab] (label): Pop a value off the stack and jump to the label specified by n if the value is less than zero.
  [tab][line-feed]: Exit a subroutine and return control to the location from which the subroutine was called.
  */
}

string whitespace (const string &code, const string &inp = string()) {
    map<int,int> heap;
    stack<int> S;
    string os, op;
    int it = 0;

    while (it < code.size()) {
        switch (code[it]) {
            case space : it++;
                //op = code.substr (it + 1, 2);
                stack_op (code, it, S); break;
            case tab   : it++;
                //op = code.substr (it + 1, 2);
                if (code[it] == space) arith_op (code, it);
                if (code[it] == tab) heap_op (code, it);
                if (code[it] == line) os += io_op (code, it, S);
                break;
            case line  :
                //Debug::display (code.substr(it));
                flow_op (code, it); break;
        }
        //it++;
    }

    cout << os;
    return os;
}
string get_op (const string &code, int &it) {
  string sub  = code.substr (it, 2);
  it += 2;
  return sub;
}
string whitespace2 (const string &code) {

    string sub, os;
    stack<int> S;
    int val, it = 0;

    while (it < code.size()) {
        switch (code[it]) {
            case space : {
                it++;
                if (code[it] == space)  {
                    int nxt = code.find (line, it);
                    string num = code.substr (it + 1, nxt - it);
                    S.push (btoi (num));
                    it = nxt;
                } else {
                    sub = code.substr (it, 2);
                    it += 2;
                    /*
                    if (sub == "\t ") { // SEARCH

                    }
                    if (sub == "\t\n") { // DISCARDn

                    }
                    if (sub == "\n ") { // return COPYTOP;

                    }
                    if (sub == "\n\t") { // return SWAP;
                    
                    }
                    if (sub == "\n\n") return 5; // return DELTOP;    // LFLF = "\n\n"
                    */
                }

                break;
            }
            case tab : {
                sub = get_op (code, it);
                //sub = code.substr (it, 2);
                //it += 2;
                if (sub == "\t ") { // Arithmetic                // TS = "\t "
                    //it += 2;
                    sub = code.substr (it, 2);
                    /*
                    if (sub == "  ")  return 6; // return ADD;
                    if (sub == " \t") return 7; // return SUBS;
                    if (sub == " \n") return 8; // return MULT;
                    if (sub == "\t ") return 9; // return DIV;   // TS = "\t "
                    if (sub == "\t\t") return 10; // return MOD;
                    */
                }
                if (sub == "\t\t") { // heap access
                    //it += 2;
                    /*
                    if (code[it] == space) return 11; // return SWAPHEAP;
                    if (code[it] == tab) return 12;   // return PUSHEAP;
                    */
                }
                if (sub == "\t\n") { // I/O access
                    sub = get_op (code, it);
                    //sub = code.substr (it, 2);
                    //it += 2;
                    if (sub == "  ") {
                        os += S.top(); //cout << static_cast<char>(S.top());
                        S.pop();
                    }
                    if (sub == " \t")  { // OUTI
                        val = S.top();
                        os += to_string(val);
                        S.pop();
                    }
                    if (sub == "\t ") {

                    }
                    if (sub == "\t\t") {


                    }
                    //Debug::display (sub);
                }
                break;
            }
            case line : {
                it++;
                sub = code.substr (it, 2);
                /*
                if (sub == "  ")  return 17; // return MARK;
                if (sub == " \t") return 18; // return CALL;
                if (sub == " \n") return 19; // return JUMP;
                if (sub == "\t ") return 20; // return JUMP1;      // TS = "\t "
                if (sub == "\t\t") return 21;// return JUMP2;
                if (sub == "\t\n") return 22;// return EXITSUB;
                */
                if (sub == "\n\n") { // return EXIT;     // LFLF = "\n\n"
                    //Debug::display (sub);
                    cout << os << endl;
                    return os;
                }
                //cout << "[exit]";

            }
            /*
            default : {
                break;
            }
            */
        };
    }

}



int operation (const string &code, int it) {

    string sub;

    switch (code[it]) {
        case space : {
            it++;
            sub = code.substr (it, 2);
            if (code[it] == space)  return 0; // return PUSH;
            it += 2;
            if (sub == "\t ")  return 1; // return SEARCH;     // TS = "\t "
            if (sub == "\t\n") return 2; // return DISCARDn;
            if (sub == "\n ")  return 3; // return COPYTOP;
            if (sub == "\n\t") return 4; // return SWAP;
            if (sub == "\n\n") return 5; // return DELTOP;    // LFLF = "\n\n"
            break;
        }
        case tab : {
            sub = code.substr (it, 2);
            it += 2;
            if (sub == "\t ") { // Arithmetic                // TS = "\t "
                //it += 2;
                sub = code.substr (it, 2);
                if (sub == "  ")  return 6; // return ADD;
                if (sub == " \t") return 7; // return SUBS;
                if (sub == " \n") return 8; // return MULT;
                if (sub == "\t ") return 9; // return DIV;   // TS = "\t "
                if (sub == "\t\t") return 10; // return MOD;
            }

            if (sub == "\t\t") { // heap access
                //it += 2;
                if (code[it] == space) return 11; // return SWAPHEAP;
                if (code[it] == tab) return 12;   // return PUSHEAP;
            }
            if (sub == "\t\n") { // I/O access
                //it += 2;
                sub = code.substr (it, 2);
                if (sub == "  ")  return 13; // OUTC
                if (sub == " \t")  return 14; // OUTI
                if (sub == "\t ")  return 15; // READC       // TS = "\t "
                if (sub == "\t\t")  return 16; // READI
            }
            break;
        }
        case line : {
            it++;
            sub = code.substr (it, 2);
            if (sub == "  ")  return 17; // return MARK;
            if (sub == " \t") return 18; // return CALL;
            if (sub == " \n") return 19; // return JUMP;
            if (sub == "\t ") return 20; // return JUMP1;      // TS = "\t "
            if (sub == "\t\t") return 21;// return JUMP2;
            if (sub == "\t\n") return 22;// return EXITSUB;
            if (sub == "\n\n") return 23;// return EXIT;     // LFLF = "\n\n"

        }
        default : {

            break;
        }
    };
}

int main () {

    //operation (output, 0);
    string outputA = "   \t    \t \n\t\n  \n\n\n";

    //Debug::display(outputA);

    whitespace2 (outputA);
    /*
    -> [space][space][space][tab][space][space][space][space][space][tab][lf][tab][lf][space][space] [lf][lf][lf]

      [space]
        [space]
            [space][tab][space][space][space][space][space][tab][lf]
                [tab][lf]
                    [space][space]

                [lf]
                    [lf][lf]
    */

}

void Test () {
    /*
    std::string output1 = "   \t\n\t\n \t\n\n\n", output2 = "   \t \n\t\n \t\n\n\n", output3 = "   \t\t\n\t\n \t\n\n\n", output0 = "    \n\t\n \t\n\n\n";
    Assert::That(whitespace(output1), Equals("1"));
    Assert::That(whitespace(output2), Equals("2"));
    Assert::That(whitespace(output3), Equals("3"));
    Assert::That(whitespace(output0), Equals("0"));

    std::string outputN1 = "  \t\t\n\t\n \t\n\n\n", outputN2 = "  \t\t \n\t\n \t\n\n\n", outputN3 = "  \t\t\t\n\t\n \t\n\n\n";
    Assert::That(whitespace(outputN1), Equals("-1"));
    Assert::That(whitespace(outputN2), Equals("-2"));
    Assert::That(whitespace(outputN3), Equals("-3"));


    expect_error("Expecting exception for unclean termination", [] () { whitespace(""); });

    std::string outputA = "   \t     \t\n\t\n  \n\n\n", outputB = "   \t    \t \n\t\n  \n\n\n", outputC = "   \t    \t\t\n\t\n  \n\n\n";
    Assert::That(whitespace(outputA), Equals("A"));
    Assert::That(whitespace(outputB), Equals("B"));
    Assert::That(whitespace(outputC), Equals("C"));

    */

}

template<typename Func> void expect_error(const std::string &msg, Func f) {
  bool error = false;

  try { f(); }
  catch (...) { error = true; }
  if (!error)
    std::cout << msg << std::endl;
  //Assert::That(error, Equals(true));
}
