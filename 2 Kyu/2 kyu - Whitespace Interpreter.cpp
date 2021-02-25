#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include <bitset>
#include <map>

#include <algorithm>
#include <stack>

using namespace std;

const char space = 32, tab = 9, line = 10;

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
        if (code[it] != line)
            num += code[it] == tab ? '1' : '0';
        else
            break;
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

  if (code[it] == space)  {
      it++;
      stack.push_back (number (code, it));
  } else {
      string sub = get_op (code, it);
      int size = stack.size() - 1;

      if (size < 0) throw exception();

      if (sub == "\n ")  stack.push_back (stack.back()); // Duplicate the top value on the stack.
      else if (sub == "\n\t") swap (stack[size],stack[size - 1]); // stackwap the top two value on the stack.
      else if (sub == "\n\n") stack.pop_back(); // Discard the top value on the stack.
      else {
          int num =  number (code, it), pos = size - num;
          if (sub == "\t ") {
              if (num < 0 || num > size) throw exception();

              stack.push_back (stack[pos]);
          } // (number): Duplicate the nth value from the top of the stack and push onto the stack.
          if (sub == "\t\n") {
              if (num < 0 || num > size) num = size;

              auto top = stack.end() - 1, start = top - num;
              stack.erase(start, top);
          } // (number): Discard the top n values below the top of the stack from the stack. (For n<**0** or **n**>=stack.length, remove everything but the top value.)
       }
    }
}
void arith_op (const string &code, size_t &it, vector<int> &stack) {

    string sub = get_op (code, it);
    int a = get_val (stack), b = get_val (stack);

    if (sub == "  ")  stack.push_back (a + b);
    if (sub == " \t") stack.push_back (b - a);
    if (sub == " \n") stack.push_back (a * b);

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

  if (mode == space) heap[get_val(stack)] = a; // then store a at heap address b.
  if (mode == tab) {
      if (!heap[a]) throw exception();
      stack.push_back (heap[a]);
  } // push the value at heap address a to the stack.
}

map<string,size_t> mk_base (const string &code) {

    size_t it = 0;
    string sub;
    map<string,size_t> labels;

    while (it < code.size()) {
        switch (code[it]) {
            case space : {
              it++;
              if (code[it] == space) it = code.find ('\n', it) + 1;
              else {
                  sub = get_op (code, it);
                  if (sub == "\t " || sub == "\t\n") it = code.find ('\n', it) + 1;
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
    size_t it = 0;

    while (it < code.size()) {

        switch (code[it]) {

            case space : stack_op (code, it, stack); break;
            case tab : {

                op = get_op (code, it);

                if (op == "\t ") arith_op (code, it, stack);
                if (op == "\t\t") {
                    heap_op (code[it], heap, stack);
                    it++;
                }
                if (op == "\t\n") { // I/O access

                    op = get_op (code, it);

                    if (op == "  ")  os += get_val(stack);
                    if (op == " \t") os += to_string (get_val (stack));
                    if (op == "\t ") {
                        char c;
                        if (iss >> c)
                            heap[get_val(stack)] = c;
                        else
                            throw exception();
                    }
                    if (op == "\t\t") {
                        int c;
                        if (iss >> c)
                            heap[get_val(stack)] = c;
                        else
                            throw exception();
                    }
                }
                break;
            }
            case line : { // Flow control
                it++;
                op = get_op (code, it);

                if (op == "\t\n") it = get_val(subs);
                else if (op == "\n\n") return os;
                else {
                    label = get_label (code, it);
                    if (label.size() && base[label] == 0) throw exception();

                    if (op == " \t") { // Call subroutine.
                        subs.push_back (it);
                        it = base[label];
                    }
                    if (op == " \n") { // Jump unconditionally to label.
                        it = base[label];
                    }
                    if (op == "\t ") { // Jump to the label if value == zero.
                        val = get_val (stack);
                        if (val == 0) it = base[label];
                    }
                    if (op == "\t\t") { // Jump to the label if value < zero.
                        val = get_val (stack);
                        if (val < 0) it = base[label];
                    }
                }
                break;
            }
        }
    }

    throw exception();
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
