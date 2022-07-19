#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <string>
#include <thread>

using namespace std;

/*
keeps variables
a += b     : "[->+>+<<]>"         -> ida = 1, idb = 2
a -= b     : ">[->+<<->]<[->+<]>" ->ida = 1, idb = 2
c = a + b  : "[->>+>>+<<<<]>[->>+>+<<<]>" ->ida = 2, idb = 3, idc = 4
c = a - b  : "[->>+>>+<<<<]>[->>+>-<<<]"
c = a * b  :
c = a / b  :
c =
*/
string showout (const string &in) {
  return "-OUTPUT-------------------------\n" + in + "\n--------------------------------\n";
}
string showtape (unsigned char tape[], int index) {

    stringstream os;
    os << "\n-MEMORY-------------------------\n";
    for (int i = 0; i < 16; i++) {
      int tile = static_cast<int> (tape[i]);
      os << setw (3);

      if (tile) {
          os << tile;
      } else
          os << " ";
      os << '|';
    }
    os << "\n--------------------------------\n";

    for (int i = 0; i < 16; i++) {
        os << setw (4) << (i == index ? "^" : " ");
    }

    return os.str();
}
string showcode (const string &code, int index) {

    stringstream os;
    os << "\n-CODE---------------------------\n";

    for (int i = 0; i < code.size(); i++) {
        char tile = code[i];
        if (tile == '[') os << "\n";
        if (i == index)
            os << "\033[0;31m" << tile << "\033[0m";
        else {
            os << tile;
        }
        if (tile == ']') os << "\n";
    }

    os << "\n--------------------------------\n";

    return os.str();
}
bool isop (char c) { return c == '>' || c == '<' || c == '+' || c == '-' || c == '.' || c == ',' || c == '['|| c ==']';}
void show (unsigned char tape[], int pos, const string &code, int index, const string &os) {
    if (isop (code[index])) {
        cout << "\033c";
        this_thread::sleep_for(50ms);

        cout << showout (os);
        cout << showtape (tape, pos);
        //cout << showcode (code, index);
    }
}
bool is_valid (const string &code) {

  int bal = 0;

  for (auto &it : code) {
      if (it == '[') bal++;
      if (it == ']') bal--;
  }

  return bal == 0;
}

unsigned loop (const string &code, unsigned pos) {
    int pile = 0, index = pos;
    const bool fwrd = code[index] == '[' ? true : false;

    while (code[index]) {
        if (code[index] == '[') pile++;
        if (code[index] == ']') pile--;
        if (pile == 0) return index;

        fwrd == true ? index++ : index--;
    }

    return index;
}
string runBF (const string &code, string input) {

    if (!is_valid (code)) return "";

    unsigned char tape[10000] = {}, *ptr = tape;
    int index = 0;
    auto ins = input.begin();
    string os;

    while (code[index]) {
        switch (code[index]) {
            case '>': ptr++; break;
            case '<': ptr--; break;
            case '+': (*ptr)++; break;
            case '-': (*ptr)--; break;
            case '.': os += *ptr; break;
            case ',': *ptr = ins != input.end() ? *ins++ : 0; break;
            case '[': if (*ptr == 0) index = loop (code,index); break;
            case ']': if (*ptr != 0) index = loop (code,index); break;
        }

        index++;
    }
    show (tape, ptr - tape, code, index - 1, os);
    //cout << showtape (tape, ptr - tape);
    return os;
}

int main () {

  string tw = "1\u0000"; // ,+[-.,+]

  string code =

  /*
  */

  "sum from 0 to N"
  ">++++++[>++++++++<-] "    // mk ascii character 48
  ",[>[-<->>+<],]"           // get digit - '0' from input
  "<<[[->++++++++++<]>[>]<<]"// if array > 10 convert array to base 10 number
  ">[-<+>]<"
"[>[-]<[>>+<+<-]"           // if second cell is null copy first cell in second and third and decrease it to 0
">[<+>-]<-]>>"              // move second cell in first cell decrement input && go to begin of while loop

">[-]++++++++++<"
"[->-[>+>>]>[+[-<+>]>+>>[-]]<<<<<]" // divmod by 10
">[-]>[>]"                  // go to array end
"++++++[>++++++++<-]"        // mk ascii character 48
"<[>>[-<+<+>>]<<.<]"        // print char



;



  cout << "::" << runBF (code, tw);

  /*
  cout << "input : ";

  while (cin >> code) {
      if (code == "exit") break;
      runBF (code, tw);
      cout << "input : ";
  }

  */
  //cout << " => " << tmp;

  //string fib = "1, 1, 2, 3, 5, 8, 13, 21, ";

  cout << "end";

  return 0;
}
