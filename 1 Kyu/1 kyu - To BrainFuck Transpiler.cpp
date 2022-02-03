#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;
using std::string_literals::operator""s;

const char eol = '\n';
const int overflow = 255;

bool isvar (string &src) {
    string::iterator it = src.begin();
    if (src.front() == '_' || src.front() == '$') it++;

    while (it != src.end()) {
        if (!isalpha (*it)) return false;
        it++;
    }
    return true;
}
bool isnum (string &src) {
    string::iterator it = src.begin();
    if (src.front() == '-') it++;

    while (it != src.end()) {
        if (!isdigit (*it) && *it != '.') return false;
        it++;
    }
    return true;
}

string sanitize (string &src) {
    size_t end = src.find ("//");
    end = min (end, src.find ("#"));
    end = min (end, src.find ("--"));
    //end = min (end, src.find ("rem"));
    return src.substr (0, end);
}
vector<string> format (const string &input) {

    istringstream iss (input);
    string line;
    vector<string> ins;

    while (getline (iss, line)) {

        line = sanitize (line);
        transform(line.begin(), line.end(), line.begin(), ::tolower);
        if (line.size())
            ins.push_back (line);
    }

    return ins;
}
vector<string> tokenize (const string &input) {
    vector<string> code;
    auto it = input.begin();
    string ins;

    while (it - input.begin() < input.size()) {
        if (*it == ' ') {
            it++;

        } else {

            while (*it && *it != ' ') ins += *it++;

            code.push_back (ins);
            ins.clear();
        }
    }

    return code;
}

std::string transpiler (const std::string &input) /* throws std::string */ {

    bool running = true;
    int r0, r1;
    string os;
    vector<string> code = format (input);
    vector<string>::iterator it = code.begin();
    map<string, int> vars;

    while (running) {
        vector<string> line = tokenize (*it);
        string op = line.front();

               if (op == "var") {
                vars[line[1]] = 0;
                 //cout << op << " " << line[1] << endl;
        } else if (op == "set") {
            if (vars.find (line[1]) == vars.end())
                throw::logic_error ("invalid identifier");

        } else if (op == "inc") {

        } else if (op == "dec") {

        } else if (op == "add") {

        } else if (op == "sub") {

        } else if (op == "mul") {

        } else if (op == "div") {

        } else if (op == "mod") {

        } else if (op == "divmod") {

        } else if (op == "cmp") {

        } else if (op == "a2b") {

        } else if (op == "b2a") {

        } else if (op == "lset") {

        } else if (op == "lget") {

        } else if (op == "ifreq") {

        } else if (op == "ifneq") {

        } else if (op == "wneq") {

        } else if (op == "proc") {

        } else if (op == "end") {

        } else if (op == "call") {

        } else if (op == "read") {
            if (vars.find (line[1]) == vars.end())
                throw::logic_error ("invalid identifier");
            r0 = vars[line[1]];
        } else if (op == "msg") {

        } else if (op == "rem") {

        }

        it++;
        if (it == code.end()) running = false;
    }
    /*
    */
    return os;
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
string brainfuck (const string &code, string &input) {

    string os, tape (1024, 0);
    auto pos = input.begin(), ptr = tape.begin();
    int index = 0;

    while (code[index]) {
        switch (code[index]) {
            case '>': ptr++; break;
            case '<': ptr--; break;
            case '+': (*ptr)++; break;
            case '-': (*ptr)--; break;
            case '.': os += *ptr; break; // putchar (*ptr)
            case ',': *ptr = pos != input.end() ? *pos++ : *ptr = overflow; break; // *ptr = getchar()
            case '[': if (*ptr == 0) index = loop (code,index); break; // while (*ptr) {
            case ']': if (*ptr != 0) index = loop (code,index); break; // }
        }
        index++;
    }

    return os;
}

int main () {

    string code =
		"var   X//This is a comment\n"
		"read X--This is also a comment\n"
		"msg \"Bye\" X#No doubt it is a comment\n"
		"rem &&Some comment~!@#$\":<";
		//"?","Bye?";
    string brnfk = transpiler (code);


    //kcuf (code);


    //cout << "end";
}
