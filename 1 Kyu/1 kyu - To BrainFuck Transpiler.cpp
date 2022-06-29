#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <map>
#include <algorithm>

#include "/home/wintermute/code/templates/Assert.hpp"

using namespace std;
using std::string_literals::operator""s;

const char eol = '\n';

const regex varprefix ("[$|_A-Za-z]+") , digit ("(-?[0-9]+(.[0-9]+)?)"), quote ("\".+?\"|\'.?\'"), nocomment ("(?:(?!//|--|#|rem).)*");

bool isvar (const string &src) { return regex_match (src, varprefix); }
bool isnum (const string &src) { return regex_match (src, digit); }
int get_val (map<string, int>& reg, string op) { return isalpha (op.at(0)) ? reg[op] : stoi (op); }

vector<string> format3 (vector<string> line) {
  for (auto &tok : line) {
    if (!regex_match (tok, quote))
        transform (tok.begin(), tok.end(), tok.begin(), ::tolower);
  }
  return line;
}
vector<vector<string>> tokenize3 (const string &input) {
  istringstream iss (input);
  vector<string> vect;
  string line;
  smatch match;
  regex re ("[$|_A-Za-z]+|(-?[0-9]+(.[0-9]+)?)|\".+?\"|\'.?\'");
  vector<vector<string>> prog;

  while (getline (iss, line)) {

    if (regex_search (line, match, nocomment)) {
      string str = match[0].str();
      sregex_token_iterator it (str.begin(), str.end(), re);
      vector<string> token (it, sregex_token_iterator());

      format3 (token);
      if (!token.empty()) prog.push_back(format3 (token));
    }
  }
  return prog;
}

string asm_interpreter (const string &input) {

    //using code = vector<vector<string>>;
    bool running = true;
    stringstream os, bf;

    map<string, int> reg;
    vector<vector<string>> prog = tokenize3 (input);
    vector<vector<string>>::iterator it = prog.begin();
    stack<vector<vector<string>>::iterator> pos;

    while (running) {
        //if (it - prog.begin() == 3) break;
        vector<string> line = *it;
        string op = line[0];
        //Display::vect (line);

               if (op == "var") {
                   //cout << ":" << op << " :: ";
                   for (int i = 1; i < line.size(); i++) {
                        reg[line[i]] = i - 1;
                        //cout << '[' << line[i] << " -> " << i - 1 << ']';
                   }
        } else if (op == "set") {
            if (reg.find (line[1]) == reg.end() || !isnum (line[2]))
                throw::logic_error ("invalid identifier");

            reg[line[1]] = stoi (line[2]);
        } else if (op == "inc") {   // Increase a b : a += b
            reg[line[1]] += reg[line[2]]; // [-<+>]
        } else if (op == "dec") {   // decrease a b : a -= b
            reg[line[1]] -= reg[line[2]];
        } else if (op == "add") {   // add a + b to c :
            reg[line[3]] = reg[line[1]] + reg[line[2]];
        } else if (op == "sub") {   // sub a - b to c
            reg[line[3]] = reg[line[1]] - reg[line[2]];
        } else if (op == "mul") {   // mul a * b to c
            reg[line[3]] = reg[line[1]] * reg[line[2]];
        } else if (op == "div") {   // div a / b to c
            reg[line[3]] = reg[line[1]] / reg[line[2]];
        } else if (op == "mod") {   // mod a % b to c
            reg[line[3]] = reg[line[1]] % reg[line[2]];
        } else if (op == "divmod") {// divm a / b to c, a % b to d
            reg[line[3]] = reg[line[1]] / reg[line[2]];
            reg[line[4]] = reg[line[1]] % reg[line[2]];
        } else if (op == "cmp") {   // comp a & b to set c
            if (reg[line[1]] < reg[line[2]]) {
                reg[line[3]] = -1;
            } else if (reg[line[1]] == reg[line[2]]) {
                reg[line[3]] = 0;
            } else if (reg[line[1]] > reg[line[2]]) {
                reg[line[3]] = 1;
            }
        } else if (op == "a2b") {   // ascii to byte

        } else if (op == "b2a") {   // byte to ascii

        } else if (op == "lset") {  // set c into index b of list a

        } else if (op == "lget") {  // Read index b of list a into c

        } else if (op == "ifreq") { // execute block when a == b

        } else if (op == "ifneq") { // execute bloc when a != b

        } else if (op == "wneq") {  // loop block when a != b

        } else if (op == "proc") {  // begin block

        } else if (op == "end") {   // end block

        } else if (op == "call") {  // call block proc "name"

            string label = line[1], var = line[2];

            for (auto fnd = prog.begin(); fnd != prog.end(); fnd++) {
                if (fnd->at(0) == "proc" && fnd->at(1) == label) {
                    reg[fnd->at(2)] = reg[var];
                    break;
                }
            }
            //jump = true;
        } else if (op == "read") {  // read into a : BF operator ','

            if (reg.find (line[1]) == reg.end())
                throw::logic_error ("invalid identifier");

            cout << "enter variable : ";
            cin >> reg[line[1]];

        } else if (op == "msg") {   // print msg : BF operator '.'

            for (size_t i = 1; i < line.size(); ++i) {
                if (regex_match (line[i], quote)) {
                    os << line[i].substr (1, line[i].size() - 2);
                } else {
                    os << reg[line[i]];
                }
            }
        } else if (op == "rem") {   // Error Handling

        }

        /*
        */
        it++;
        if (it == prog.end()) running = false;
    }

    return os.str();
}

unsigned loop (const string &code, unsigned pos) {
    int balance = 0, index = pos;
    const bool fwrd = code[index] == '[' ? true : false;

    while (code[index]) {
        if (code[index] == '[') balance++;
        if (code[index] == ']') balance--;
        if (balance == 0) return index;

        fwrd == true ? index++ : index--;
    }

    return index;
}
string bnf_interpreter (const string &code, string &input) {

    string os;
    char tape[30000] = {}, *ptr = &tape[15000];
    auto pos = input.begin();
    int index = 0;

    while (code[index]) {
        switch (code[index]) {
            case '>': ptr++; break;      // increment the data pointer (to point to the next cell to the right)
            case '<': ptr--; break;      // decrement the data pointer (to point to the next cell to the left).
            case '+': (*ptr)++; break;   // increment (increase by one, truncate overflow: 255 + 1 = 0) the byte at the data pointer
            case '-': (*ptr)--; break;   // decrement (decrease by one, treat as unsigned byte: 0 - 1 = 255 ) the byte at the data pointer.
            case '.': os += *ptr; break; // output the byte at the data pointer.
            case ',': *ptr = pos != input.end() ? *pos++ : 0; break; // store one byte of input to the data pointer.
            case '[': if (*ptr == 0) index = loop (code,index); break; // loop until data pointer != 0 then jump to command after ]
            case ']': if (*ptr != 0) index = loop (code,index); break; // until data pointer != 0, got to command after [
        }
        index++;
    }

    return os;
}

int main () {

    Timer clock;

    string input =
		"var   X//This is a comment\n"
		"read X--This is also a comment\n"
		"msg \"Bye\" X#No doubt it is a comment\n"
		"rem &&Some comment~!@#$\":<";
		//"?","Bye?";

    cout << asm_interpreter (input);

    input =
    "var A\n"
    "set a 20\n"
    "call Wrap a\n"

    "proc Say x\n"
    "    msg \"It is \"x\n"
    "    call Wrap X\n"
    "end\n"
    "rem loop\n"
    "Proc Wrap X\n"
    "    call Say x\n"
    "eNd";

    input =
    "var q w e\n"
    "read q\n"
    "read w\n"
    "add q w e\n"
    "msg q \" \" w \" \" e";

    // to => Any valid BrainFuck code with the same functionality.
    //string brnfk = transpiler (input);

    //kcuf (code);

    clock.stop();
    clock.get_duration();

}

////////////////////////////////////////////////////////////////////////////////
string trunc (string &src) {
    size_t end = src.find ("//");
    end = min (end, src.find ("#"));
    end = min (end, src.find ("--"));
    //end = min (end, src.find ("rem"));
    return src.substr (0, end);
}
vector<string> format2 (const string &input) {

    istringstream iss (input);
    string line;
    vector<string> ins;

    while (getline (iss, line)) {

        line = trunc (line);
        transform (line.begin(), line.end(), line.begin(), ::tolower);

        if (line.size())
            ins.push_back (line);
    }

    return ins;
}

vector<string> format (const string &input) {

    istringstream iss (input);
    vector<string> vect;
    string line;
    smatch match;

    while (getline (iss, line)) {
        if (regex_search (line, match, nocomment)) {
            if (match[0] != "")
                vect.push_back(match[0]);

        }
    }

    return vect;
}
vector<string> tokenize2 (const string &line) {
    regex re ("[$|_A-Za-z]+|-?\\d+.?\\d+|\".+\"|\'.\'");
    sregex_token_iterator it (line.begin (), line.end (), re);
    return vector<string> (it, sregex_token_iterator ());
}
