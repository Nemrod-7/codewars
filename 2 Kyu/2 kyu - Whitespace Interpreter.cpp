#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <vector>
#include <bitset>
#include <map>

#include <algorithm>
#include <stack>

#include <iomanip>

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
void Test (), Test2 ();
////////////////////////////////////////////////////////////////////////////////
const char space = 32, tab = 9, line = 10;

class Debug {
    public :
        static string unbleach(std::string s) {
          std::transform(s.begin(), s.end(), s.begin(), [] (char c) { return (c == ' ') ? 's' : ((c == '\n') ? 'n' : 't'); });
          return s;
        }
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
          oss << display (src);
        }
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

int number2 (const string &code, size_t &it) {

  if (code[it] == line) throw exception();
  int sign = code[it] == space ? 1 : -1;
  string num;
  it++;

  do {
    if (code[it] != line ) num += code[it] == tab ? '1' : '0';
    else break;
  } while (it++);

  it++;
  return bitset<8> (num).to_ulong() * sign;
}
int number (const string &code, size_t &it) {
    if (code[it] == line) {
      throw exception();
    }
    int sign = code[it] == space ? 1 : -1;
    string num;

    while (it++) {
        if (code[it] != line)
            num += code[it] == tab ? '1' : '0';
        else break;
    }

    it++;
    return bitset<8> (num).to_ulong() * sign;
}
string get_label (const string &code, size_t &it) {
    string os;

    do {
        os += code[it];
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
  string sub = code.substr (it, 2);
  it += 2;
  return sub;
}

void stack_op (const string &code, size_t &it, vector<int> &stack) {
  it++;

  if (code[it] == space)  { it++;
      stack.push_back (number (code, it));
      //cout << "number : " << stack.back() << endl;
  } else {
      string sub = get_op (code, it);
      int size = stack.size() - 1;

      if (size < 0) throw exception();

      if (sub == "\t ") {
          int num =  number (code, it);
          int pos = size - num;
          if (num < 0 || num >= (int) stack.size()) throw exception();
          //cout << num << ' ' << size;
          stack.push_back (stack[pos]);
      } // (number): Duplicate the nth value from the top of the stack and push onto the stack.
      if (sub == "\t\n") {
          int num = number (code, it);

          if (num < 0 || num >= (int) stack.size())
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
    if (sub == "  ")  { stack.push_back (a + b); } // Pop a and b, then push b+a.
    if (sub == " \t") { stack.push_back (b - a); } // Pop a and b, then push b-a.
    if (sub == " \n") { stack.push_back (a * b); } // Pop a and b, then push b*a.

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
      heap[get_val(stack)] = a;
  } // Pop a and b, then store a at heap address b.
  if (mode == tab) {
      //cout << heap.size ();
      if (!heap[a]) throw exception();
      stack.push_back (heap[a]);
  } // Pop a and push the value at heap address a to the stack.
}

map<string,size_t> mk_base (const string &code) {

    size_t it = 0;
    string sub;
    map<string,size_t> labels;

    while (it < code.size()) {
        switch (code[it]) {
            case space : {
              it++;
              if (code[it] == space)  {
                  it = code.find ('\n', it) + 1;
              } else {
                  sub = get_op (code, it);
                  if (sub == "\t " || sub == "\t\n") it = code.find ('\n', it) + 1;                  //if (sub == "\t\n") it = code.find ('\n', it) + 1;
              }
              break;
            }
            case tab : {
                sub = get_op (code, it);

                if (sub == "\t " || sub == "\t\n")  it += 2;
                if (sub == "\t\t") it++;
                break;
            }
            case line : {
                it++;
                sub = get_op (code, it);

                if (sub != "\t\n" && sub != "\n\n") {
                    string label = get_label (code, it);

                    if (sub == "  ") {
                        if (labels[label]) throw exception();
                        labels[label] = it;
                    }

                }
                break;
            }
        }
    }

    return labels;
}

string whitespace (const string &src, const string &inp = string()) {

    string code = clean (src), os;
    string op, label;
    stringstream iss (inp);
    vector<int> stack, subs;
    map<int,int> heap;
    map<string, size_t> base = mk_base(code);
    int val;
    size_t index = 4, it = 0;

    while (index-->0) { //it < code.size()

        switch (code[it]) {

            case space : { stack_op (code, it, stack);
              //cout << "stack : " << it << endl;
             break; }
            case tab : {

                op = get_op (code, it);

                if (op == "\t ") { arith_op (code, it, stack); }
                if (op == "\t\t") {
                    heap_op (code[it], heap, stack);
                    it++;
                }
                if (op == "\t\n") { // I/O access

                    op = get_op (code, it);

                    if (op == "  ") { os += get_val(stack); }
                    if (op == " \t")  { os += to_string (get_val (stack)); }
                    if (op == "\t ") {
                        char c;
                        if (iss >> c)
                            heap[get_val(stack)] = c;
                        else
                            throw exception();
                        //cout << a << ' ';
                    }
                    if (op == "\t\t") {
                        int c;
                        if (iss >> c)
                            heap[get_val(stack)] = c;
                        else
                            throw exception();

                    }
                    // Debug::display (op);
                }
                break;
            }
            case line : { // Flow control
                it++;
                op = get_op (code, it);

                if (op == "\t\n") {
                    it = get_val(subs);
                    //cout << "exitsub : " << it << endl;
                }

                if (op == "\n\n") { return os; }

                label = get_label (code, it);
                //if (base[label] == 0) throw exception();

                if (op == " \t") {
                    subs.push_back (it);
                    //cout << " call ";
                    it = base[label];
                    //cout << it << endl;
                } // Call subroutine.
                if (op == " \n") {
                    it = base[label];
                } // Jump unconditionally to label.
                if (op == "\t ") {
                    val = get_val (stack);
                    if (val == 0) it = base[label];
                } // Jump to the label if value == zero.
                if (op == "\t\t") {
                    val = get_val (stack);
                    if (val < 0) it = base[label];
                    //cout << "jump2 : " << it << endl;
                }// Jump to the label if value < zero.

                break;
            }
            default : {
                 cout << "default : " << it << endl;
            }
        }
        cout << it << ' ';
    }
    //if (!src.size())
    //throw exception();
}

int main () {



  //string code = "   \t\n   \t \n   \t\t\n\t\n \t\n \n\n\t\n \t\t\n \t\n  \n\n  \n\n\n\n"; //=> ok Expecting exception for repeated labels
  string call4 = "   \t\n\n \t \n   \t \n\n \t \n   \t\t\n\n \t \n\n\n\n\n   \n\t\n \t\n\t\n";
  whitespace (call4);

  //Test2();
  //Debug::to_file (call4);


  /*
  //map<string, size_t> base = mk_base(code);
  for (auto &it : base)
      cout << Debug::display (it.first) << ' ' << it.second << endl;
      */

  //Test();
  //cout << "Debug::display(jump5.substr(42 - 5));";

}

template<typename Func> void expect_error (const std::string &msg, Func f) {
  bool error = false;

  try { f(); }
  catch (...) { error = true; }
  if (!error)
    cout << msg << endl;
}
void Test2 () {
  cout << " jumps test";
  string call2 = "   \t \n\n \t \n   \t\t\n\n \t \n   \t\n\n \t \n\n\n\n\n   \n\t\n \t\n\n\n"; // => ok Testing_subroutine_functionality
  whitespace (call2);

  expect_error("Expecting exception for repeated labels", [] () { whitespace ("   \t\n   \t \n   \t\t\n\t\n \t\n \n\n\t\n \t\t\n \t\n  \n\n  \n\n\n\n"); });
  expect_error("Expecting exception for unknown label", [] () { whitespace ("   \t\n\t\n \t   \t\n\n \t \n   \t \n\n \t \n   \t\t\n\n \t \n\n\n\n\n  \n\t\n \t\n\t\n"); });

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

string jump = "   \t\n   \t\t\n   \n   \t \n   \n   \t\n\n  \n\t\n \t\n\t \n\n\n\n";// Expected: equal to "123"
Assert::That(whitespace(jump), Equals("123"));

{
    string intst = "   \t\n\t\n\t\t   \t \n\t\n\t\t   \t\t\n\t\n\t\t   \t\t\n\t\t\t   \t \n\t\t\t   \t\n\t\t\t\t\n \t\t\n \t\t\n \t\n\n\n";
    string input = "1\n2\n3\n";
    Assert::That(whitespace(intst, input), Equals("123"));
}

expect_error("Expecting exception for unclean termination", [] () { whitespace("   \t\n\t\n\t\t   \t \n\t\n\t\t   \t\t\n\t\n\t\t   \t\t\n\t\t\t   \t \n\t\t\t   \t\n\t\t\t\t\n \t\t\n \t\t\n \t\n\n\n","1\n2\n"); });
expect_error("Expecting exception for out of bounds index", [] () { whitespace ("   \t\n   \t \n   \t\t\n \t \t\t\n\t\n \t\n\n\n"); });
expect_error("Expecting exception for out of bounds index", [] () { whitespace ("   \t\n   \t \n   \t\t\n \t  \t\t\n\t\n \t\n\n\n"); });

string jump1 = "   \t\n   \t\t\n   \n   \t \n   \n   \t\n\n  \n\t\n \t\n\t \n\n\n\n";
string jump2 = "  \t\t\t\n\n  \n \n   \t\t\n\t  \n\t\n \t   \t\n\t    \n \n\t\t\n\n\n\n";
string jump3 = "   \t\n   \t \n   \t\t\n\t\n \t\n \n\n\t\n \t\t\n \t\n  \n\n\n\n";
string jump4 = "   \t\n   \t \n   \t\t\n\t\n \t\n \n\n\n  \t\n\t\n \t\t\n \t\n\n\n\n  \n\n \n\t\n";
string jump5 = "  \t\t\n   \n   \t\n   \t \n   \t\t\n\n  \n\t\n \t \n \n\t\t \n\n \n\n\n   \n\n\n\n";
string jump6 = "   \n   \t\n\t\n \t\n\t  \n   \t\n\t\n \t\n\n\n";
  /*
  Testing_input_functionality
  Log
  Testing_conditional_and_unconditional_jump_edge_cases
  Log
  "   \t\n\t\n \t\n\t  \n   \t\n\n   \n\n\n\n";
    */

}
//////////////////////////////////Arkive///////////////////////////////////////
/*
size_t search_pos (const string &code, string label) {
//const string mark = "\n  ";
size_t size = label.size() + 3;
return code.find ("\n  " + label) + size;
}
enum instruct {
    PUSH, DUPLICATE, DISCARD, COPY, SWAP, POP,
    ADD, SUB, MUL, DIV, MOD, HEAP1, HEAP2,
    OUTC, OUTI, READC, READN,
    MARK, CALL, JUMP, JUMP1, JUMP2, ESUB, EXIT
};
void operation (const string src) {
  string code = clean (src), os;
  size_t it = 0;

  string op, label;
  int ins, num;
  int index = 0;
  map<string, size_t> locat;
  vector<vector<string>> program;

  while (it < code.size()) {

    switch (code[it]) {
      case space : {
        it++;

        if (code[it] == space)  { it++;

          ins = PUSH;
          label = to_string(number (code, it));

        } else {

          op = get_op (code, it);

          if (op == "\t ") {
            ins = DUPLICATE;
            label = to_string(number (code, it));
          }
          if (op == "\t\n") {
            ins = DISCARD;
            label = to_string(number (code, it));
          }
          if (op == "\n ") ins = COPY;
          if (op == "\n\t") ins = SWAP;
          if (op == "\n\n") ins = POP;
        }

        break;
      }
      case tab : {

        op = get_op (code, it);

        if (op == "\t ") {
          op = get_op (code, it);

          if (op == "  ")  ins = ADD;
          if (op == " \t") ins = SUB;
          if (op == " \n") ins = MUL;
          if (op == "\t ") ins = DIV;
          if (op == "\t\t") ins = MOD;
        }

        if (op == "\t\t") {
          cout << " heap " << endl;
          it++;
        }
        if (op == "\t\n") { // I/O access

          op = get_op (code, it);

          if (op == "  ")  ins = OUTC;
          if (op == " \t") ins = OUTI;
          if (op == "\t ") ins = READC;
          if (op == "\t\t") ins = READN;
        }
        break;
      }
      case line : { // Flow control

        it++;
        op = get_op (code, it);

        if (op == "\t\n") ins = ESUB;
        if (op == "\n\n") ins = EXIT;
        //label = get_label (code, it);
        if (op == "  ")  {
          ins = MARK;
          label = get_label (code, it);

        }
        if (op == " \t") {
          ins = CALL;
          label = get_label (code, it);
        }
        if (op == " \n") {
          ins = JUMP;
          label = get_label (code, it);
        }
        if (op == "\t ") {
          ins = JUMP1;
          label = get_label (code, it);
        }

        if (op == "\t\t") {
          ins = JUMP2;
          label = get_label (code, it);
        }
        break;
      }
      default : {
        //cout << "default : " << endl;
      }
    }
    cout << index++ << ' ' << ins << ' ' << label << endl;
  }
}
*/
