#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <bitset>
#include <map>

#include <stack>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
class Assert {
    public :
        template<class T> static void That (const T& a, const T& b) {
            static int testn;
            if (a != b) {
                cout << "test " << testn << " -> ";
                cout << "actual : " << a << " expected : " << b;
                cout << endl;
            }
            testn++;
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
            cout << endl;
        }
        static void to_file (string &src) {
          fstream oss ("whites", ios::out);
          oss << Debug::display (src);
        }
};
class interpreter {
    private :
        int it = 0;
        string code, op;
        stack<int> S;
};
//////////////////////////////////////////////////////////////////
string clean (const string &src) {
    string code;
    for (auto &it : src) {
        if (it == space || it == tab || it == line)
            code += it;
    }
    return code;
}

int number (const string &code, size_t &it) {
    int sign = code[it] == space ? 1 : -1;
    string num;

    while (it++) {
        if (code[it] == line) break;
        num += code[it] == tab ? '1' : '0';
    }

    it++;
    return bitset<8> (num).to_ulong() * sign;
}

string get_label (const string &code, size_t &it) {
    string os;

    do {
        switch (code[it]) {
            case space : os += 's'; break;
            case tab : os += 't'; break;
            case line : os += 'l'; break;
            default : break;
        }
    } while (code[it++] != line);

    return os;
}
int get_val (vector<int> &stack) {
    if (!stack.size()) throw exception();
    int val = stack.back();
    stack.pop_back();
    return val;
}
string get_op (const string &code, size_t &it) {
  string sub  = code.substr (it, 2);
  it += 2;
  return sub;
}

void stack_op (const string &code, size_t &it, vector<int> &stack) {
  it++;
  //cout << "stack ";
  if (code[it] == space)  { it++;
      stack.push_back (number (code, it));

  } else {
      string sub = get_op (code, it);
      int size = stack.size() - 1;
      //cout << size << ' ';
      if (size < 0) throw exception();

      if (sub == "\t ") {
          int pos = size - number (code, it);
          stack.push_back (stack[pos]);
      } // (number): Duplicate the nth value from the top of the stack and push onto the stack.
      if (sub == "\t\n") {
          int num = number (code, it);

          if (num < 0 || num >= stack.size())
              num = stack.size() - 1;

          auto top = stack.end() - 1, start = top - num;
          stack.erase(start, top);
          //cout << pos << ' ';
      } // (number): Discard the top n values below the top of the stack from the stack. (For n<**0** or **n**>=stack.length, remove everything but the top value.)
      if (sub == "\n ") { stack.push_back (stack.back()); } // Duplicate the top value on the stack.
      if (sub == "\n\t") { swap (stack[size],stack[size - 1]); } // stackwap the top two value on the stack.
      if (sub == "\n\n") { stack.pop_back(); } // Discard the top value on the stack.

  }
}
void arith_op (const string &code, size_t &it, vector<int> &stack) {

    string sub = get_op (code, it);

    //if (stack.size() < 2) throw exception();
    int a = get_val (stack), b = get_val (stack);
    //cout << a << ' ' << b << endl;
    //cout << a << ' ' << b;
    if (sub == "  ")  { stack.push_back (a + b); } // Pop a and b, then push b+a.
    if (sub == " \t") { stack.push_back (b - a); } // Pop a and b, then push b-a.
    if (sub == " \n") { stack.push_back (a * b); } // Pop a and b, then push b*a.

    //if (a == 0) throw exception();
    if (sub == "\t ") {
        if (a == 0) throw exception();
        stack.push_back (floor (b / (float)a));
    } // Pop a and b, then push b/a*. If a is zero, throw an error.

    if (sub == "\t\t") {
        if (a == 0) throw exception();
        stack.push_back ((b % a + a) % a);
    } // Pop a and b, then push b % a*. If a is zero, throw an error.
}

void heap_op (char mode, map<int,int> &heap, vector<int> &stack) {
  int a = get_val(stack);
  //cout << a ;
  if (mode == space) {
      if (!stack.size()) throw exception();
      heap[get_val(stack)] = a;
  } // Pop a and b, then store a at heap address b.
  if (mode == tab) {
      //cout << heap.size ();
      if (!heap.size()) throw exception();
      stack.push_back (heap[a]);
  } // Pop a and push the value at heap address a to the stack.
}

string whitespace (const string &src, const string &inp = string()) {

    string code = clean (src), input = inp, sub, os;
    string label;
    string::iterator in = input.begin();
    vector<int> stack;
    map<int,int> heap;
    map<string,size_t> locat;
    int a, b;
    size_t it = 0;

    while (it < code.size()) {
        int op = code[it];
        //cout << it << ' ';
        switch (op) {
            case space : { stack_op (code, it, stack);
              //cout << "stack : " << it << endl;
             break; }
            case tab : {
                //cout << "tab : " << it << endl;
                sub = get_op (code, it);

                if (sub == "\t ") { arith_op (code, it, stack); }
                if (sub == "\t\t") {
                    heap_op (code[it], heap, stack);
                    it++;
                }
                if (sub == "\t\n") { // I/O access

                    sub = get_op (code, it);
                    //cout << Debug::display(sub);
                    if (sub == "  ") { os += get_val(stack); }
                    if (sub == " \t")  { os += to_string (get_val (stack)); }
                    if (sub == "\t ") {
                        a = *in++;
                        b = get_val(stack);
                        heap[b] = a;
                    }
                    if (sub == "\t\t") {
                      //cout << "here";
                      a = *in++;
                      b = get_val(stack);
                      heap[b] = a;
                    }
                    // Debug::display (sub);
                }
                break;
            }
            case line : { // Flow control
                //cout << "line : " << it << endl;
                it++;
                sub = code.substr (it, 2);

                if (sub == "  ")  {
                    int location = it;
                    label = get_label (code, it);
                    //if (!locat[label])
                        locat[label] = location;
                    //cout << "mark : " << locat[label] << endl;
                  //cout << "mark : " << it << endl;
                } // [space][space] (label): Mark a location in the program with label n.
                if (sub == " \t") {
                    cout << "call : " << it << endl;
                } // space][tab] (label): Call a subroutine with the location specified by label n.
                if (sub == " \n") {
                    cout << "jump : " << it << endl;
                } // [space][line-feed] (label): Jump unconditionally to the position specified by label n.
                if (sub == "\t ") {
                    a = get_val(stack);
                    label = get_label(code,it);

                    if (a == 0) it = locat[get_label(code,it)];
                } // [tab][space] (label): Pop a value off the stack and jump to the label specified by n if the value is zero.

                if (sub == "\t\t") {
                    cout << "jump2 : " << it << endl;
                }// [tab][tab] (label): Pop a value off the stack and jump to the label specified by n if the value is less than zero.

                if (sub == "\t\n") {
                    cout << "exitsub : " << it << endl;
                }// [line-feed][line-feed]: Exit the program.

                if (sub == "\n\n") {
                    //cout << "exit : " << it << endl;
                    return os;
                } // Exit the program.

                break;
            }
            default : {
                 cout << "default : " << it << endl;
            }
        }
    }
    //if (!src.size())
    //throw exception();
}


int main () {

  string code = "   \t\n   \t\t\n   \n   \t \n   \n   \t\n\n  \n\t\n \t\n\t \n\n\n\n";// Expected: equal to "123"
  //Debug::to_file (jump);
  //string test = "   \t \n\t\n \t\n\n\n";
  //Debug::to_file (test);

  cout << whitespace(code);
// [space][space] (label): Mark a location in the program with label n.
// space][tab] (label): Call a subroutine with the location specified by label n.

// [space][line-feed] (label): Jump unconditionally to the position specified by label n.
// [tab][space] (label): Pop a value off the stack and jump to the label specified by label n if the value is zero.
// [tab][tab] (label): Pop a value off the stack and jump to the label specified by n if the value is less than zero.
// [line-feed][line-feed]: Exit the program.
// Exit the program.

  //Test();
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

  string duplicate = "   \t\t\n \n \t\n \t\t\n \t\n\n\n";  // 33
  Assert::That(whitespace(duplicate), Equals("33"));

  string duplicateN1 = "   \t\n   \t \n   \t\t\n \t  \t \n\t\n \t\n\n\n"; // 1
  Assert::That(whitespace(duplicateN1), Equals("1"));

  string duplicateN2 = "   \t\n   \t \n   \t\t\n \t  \t\n\t\n \t\n\n\n"; // 2
  Assert::That(whitespace(duplicateN2), Equals("2"));

  string duplicateN3 = "   \t\n   \t \n   \t\t\n \t   \n\t\n \t\n\n\n"; // 3
  Assert::That(whitespace(duplicateN3), Equals("3"));

  string swap = "   \t\t\n   \t \n \n\t\t\n \t\t\n \t\n\n\n"; // 32
  Assert::That(whitespace(swap), Equals("32"));

  string discard = "   \t\t\n   \t \n \n\t \n\n\t\n \t\n\n\n"; // 2
  Assert::That(whitespace(discard), Equals("2"));

  string arith = "  \t\t\n   \t  \n\t   \t\n \t\n\n\n"; // 3
  Assert::That(whitespace(arith), Equals("3"));

  string floordiv = "   \t   \n   \t\t\n\t \t \t\n \t\n\n\n"; // 2
  Assert::That(whitespace(floordiv), Equals("2"));

  string floor2 = "  \t\t   \n   \t\t\n\t \t \t\n \t\n\n\n";
  Assert::That(whitespace(floor2), Equals("-3"));

  string floor3 = "   \t \t\n  \t\t \n\t \t\t\t\n \t\n\n\n";
  Assert::That(whitespace(floor3), Equals("-1")); //Should be the remainder with the sign of the divisor

  string heap2 = "   \t\n   \t\n   \t \n\t\t \t\t\t\t\n \t\n\n\n"; // 2
  Assert::That(whitespace(heap2), Equals("2"));

  string slide = "   \t\t\n   \t \n   \t\n   \t  \n   \t\t \n   \t \t\n   \t\t\t\n \n\t \t\n \t\t\n\t\n \t\t\n \t\t\n \t\t\n \t\n\n\n"; // 5123
  Assert::That(whitespace(slide), Equals("5123"));

  // Testing_stack_edge_cases
  string edge1 = "  \t\n\t\n \t\n\n\n";
  Assert::That(whitespace(edge1), Equals("0"));

  expect_error("Expecting exception for unclean termination", [] () { whitespace(" \n \n\n\n"); });
  expect_error("Expecting exception for unclean termination", [] () { whitespace(" \n\n\n\n\n"); });

  string edge4 = "   \t\n   \t \n   \t\t\n \t  \t\t\n\t\n \t\n\n\n";
  // Testing_heap_edge_cases
  expect_error("Expecting exception undefined heap address", [] () { whitespace("   \t\n\t\t\t\t\n \t\n\n\n"); });

  expect_error("Expecting exception for empty stack", [] () { whitespace("   \t\n   \t \n\t\t \t\t\t\n\n\n"); });
  expect_error("Expecting exception for arith edge case stack", [] (){ whitespace ("\t   \t\n \t\n\n\n");}); // nth test : Testing_arithmetic_edge_cases


  /*

  string input_func = "   \t\n\t\n\t\t   \t \n\t\n\t\t   \t\t\n\t\n\t\t   \t\t\n\t\t\t   \t \n\t\t\t   \t\n\t\t\t\t\n \t\t\n \t\t\n \t\n\n\n";// 123
  string input_edge = "   \t\n\t\n\t\t   \t \n\t\n\t\t   \t\t\n\t\n\t\t   \t\t\n\t\t\t   \t \n\t\t\t   \t\n\t\t\t\t\n \t\t\n \t\t\n \t\n\n\n";//Expecting exception for end of input
  Testing_input_functionality
  Log
  "   \t\n\t\n\t\t   \t \n\t\n\t\t   \t\t\n\t\n\t\t   \t\t\n\t\t\t   \t \n\t\t\t   \t\n\t\t\t\t\n \t\t\n \t\t\n \t\n\n\n";
  Expected: equal to "123"
  Actual: "491050"
  Testing_input_edge_cases
  Log
  "   \t\n\t\n\t\t   \t \n\t\n\t\t   \t\t\n\t\n\t\t   \t\t\n\t\t\t   \t \n\t\t\t   \t\n\t\t\t\t\n \t\t\n \t\t\n \t\n\n\n";
  Expecting exception for end of input
  Expected: true
  Actual: false
  Testing_conditional_and_unconditional_jump_functionality
  Log
  "   \t\n   \t\t\n   \n   \t \n   \n   \t\n\n  \n\t\n \t\n\t \n\n\n\n";
  Expected: equal to "123"
  Actual: ""
  Testing_conditional_and_unconditional_jump_edge_cases
  Log
  "   \t\n\t\n \t\n\t  \n   \t\n\n   \n\n\n\n";
    */

}
//////////////////////////////////Arkive///////////////////////////////////////
