#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <bitset>
#include <stack>

using namespace std;
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
string Equals (const string &entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////
const char space = 32, tab = 9, line = 10;

class Debug {
    public :
        static string display (const string &src) {
            string os;
            for (auto &c : src)
                switch (c) {
                    case space : os += "[space]"; break;
                    case tab : os += "[tab]"; break;
                    case line : os += "[lf]"; break;
                    default : break;
                }

            return os;
        }
        static void mk_str (string s) { // To help with debugging
            cout << '"';
            for (auto &it : s) {
                if (it == space) cout << ' ';
                if (it == tab) cout << (char)92 << 't';
                if (it == line) cout << (char)92 << 'n';
            }
            cout << '"' << ';';

        }
};
class interpreter {
    private :
        int it = 0;
        string code, op;
        stack<int> S;
};


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
  it++;
  switch (code[it]) {
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
}
string whitespace2 (const string &code, const string &inp = string()) {
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

//////////////////////////////////////////////////////////////////
string clean (const string &src) {
    string code;
    for (auto &it : src) {
        if (it == space || it == tab || it == line)
            code += it;
    }
    return code;
}
string get_op (const string &code, int &it) {
  string sub  = code.substr (it, 2);
  it += 2;
  return sub;
}
void add (stack<int> S) {

}
string whitespace (const string &src) {

    string code = clean (src), sub, os;
    stack<int> S;
    int a, b, it = 0;

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
                    if (sub == "\t ") { // (number): Duplicate the nth value from the top of the stack and push onto the stack.

                    }
                    if (sub == "\t\n") { // (number): Discard the top n values below the top of the stack from the stack. (For n<**0** or **n**>=stack.length, remove everything but the top value.)

                    }
                    if (sub == "\n ") { // Duplicate the top value on the stack.
                        S.push(S.top());
                    }
                    if (sub == "\n\t") { // Swap the top two value on the stack.
                        a = S.top(), S.pop();
                        b = S.top(), S.pop();
                        S.push (b);
                        S.push (a);
                    }
                    /*
                    if (sub == "\n\n") { // Discard the top value on the stack.

                    }

                    */
                }

                break;
            }
            case tab : {
                sub = get_op (code, it);

                if (sub == "\t ") { // Arithmetic

                    sub = code.substr (it, 2);
                    a = S.top(), S.pop();
                    b = S.top(), S.pop();
                    
                    if (sub == "  ")  { S.push (a + b); } // Pop a and b, then push b+a.
                    if (sub == " \t") { S.push (b - a); } // Pop a and b, then push b-a.
                    if (sub == " \n") { S.push (a * b); } // Pop a and b, then push b*a.
                    if (sub == "\t ") {
                        if (a == 0) throw runtime_error();
                        S.push (b / a); 
                    } // Pop a and b, then push b/a*. If a is zero, throw an error.
                    
                    /*
                    if (sub == "\t\t") { // Pop a and b, then push b % a*. If a is zero, throw an error.

                    }
                    */
                }
                if (sub == "\t\t") { // heap access
                    //it += 2;
                    /*
                    if (code[it] == space) { // Pop a and b, then store a at heap address b.

                    }
                    if (code[it] == tab) { // Pop a and then push the value at heap address a onto the stack.

                    }
                    */
                }
                if (sub == "\t\n") { // I/O access
                    sub = get_op (code, it);
                    
                    if (sub == "  ") {
                        os += S.top(); // cout << static_cast<char>(S.top());
                        S.pop();
                    }
                    if (sub == " \t")  { // OUTI
                        a = S.top();
                        os += to_string (a);
                        S.pop();
                    }
                    if (sub == "\t ") {

                    }
                    if (sub == "\t\t") {

                        
                    }
                    // Debug::display (sub);
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
                break;
            }
            default : {
                // cout << it;
                // Debug::display (code);
                //throw::exception();
                it++;
            }
            /*
            */
        }
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

    string duplicate = "   \t\t\n \n \t\n \t\t\n \t\n\n\n";
    string duplicateN1 = "   \t\n   \t \n   \t\t\n \t  \t \n\t\n \t\n\n\n"; // 1
    string duplicateN2 = "   \t\n   \t \n   \t\t\n \t  \t\n\t\n \t\n\n\n"; // 2
    string duplicateN3 = "   \t\n   \t \n   \t\t\n \t   \n\t\n \t\n\n\n"; // 3
    string swap = "   \t\t\n   \t \n \n\t\t\n \t\t\n \t\n\n\n";

    whitespace (duplicateN1);


    /*
    fstream oss ("whites", ios::out);
    oss << Debug::display (duplicateN1);
    -> [space]
          [space]
              [space][tab][lf]
       [space]
          [lf][space]

       [tab][lf]
          [space][tab][tab][lf]
       [space][tab]

       [lf][lf][lf]

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

void Test () {
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


    outputA = "blahhhh   \targgggghhh     \t\n\t\n  \n\n\n"; // A
    Assert::That(whitespace(outputA), Equals("A"));

    outputB = " I heart \t  cats  \t \n\t\n  \n\n\n"; // B
    Assert::That(whitespace(outputB), Equals("B"));

    outputC = "   \t  welcome  \t\t\n\t\n to the\nnew\nworld\n"; // C
    Assert::That(whitespace(outputC), Equals("C"));

  string pushTwice = "   \t\t\n   \t\t\n\t\n \t\t\n \t\n\n\n"; // 33
  Assert::That(whitespace(pushTwice), Equals("33"));
  /*
  string duplicate = "   \t\t\n \n \t\n \t\t\n \t\n\n\n";  // 33
  string duplicateN1 = "   \t\n   \t \n   \t\t\n \t  \t \n\t\n \t\n\n\n"; // 1
  string duplicateN2 = "   \t\n   \t \n   \t\t\n \t  \t\n\t\n \t\n\n\n"; // 2
  string duplicateN3 = "   \t\n   \t \n   \t\t\n \t   \n\t\n \t\n\n\n"; // 3
  string swap = "   \t\t\n   \t \n \n\t\t\n \t\t\n \t\n\n\n"; // 32
  string discard = "   \t\t\n   \t \n \n\t \n\n\t\n \t\n\n\n"; // 2
  string slide = "   \t\t\n   \t \n   \t\n   \t  \n   \t\t \n   \t \t\n   \t\t\t\n \n\t \t\n \t\t\n\t\n \t\t\n \t\t\n \t\t\n \t\n\n\n"; // 5123
  string test = "  \t\n\t\n \t\n\n\n"; // 0

  string heap2 = "   \t\n   \t\n   \t \n\t\t \t\t\t\t\n \t\n\n\n"; // 2
  string arith = "  \t\t\n   \t  \n\t   \t\n \t\n\n\n"; // 3
  string floor div = "   \t   \n   \t\t\n\t \t \t\n \t\n\n\n"; // 2

  string input_func = "   \t\n\t\n\t\t   \t \n\t\n\t\t   \t\t\n\t\n\t\t   \t\t\n\t\t\t   \t \n\t\t\t   \t\n\t\t\t\t\n \t\t\n \t\t\n \t\n\n\n";// 123

  string input_edge = "   \t\n\t\n\t\t   \t \n\t\n\t\t   \t\t\n\t\n\t\t   \t\t\n\t\t\t   \t \n\t\t\t   \t\n\t\t\t\t\n \t\t\n \t\t\n \t\n\n\n";//Expecting exception for end of input

    */

}
