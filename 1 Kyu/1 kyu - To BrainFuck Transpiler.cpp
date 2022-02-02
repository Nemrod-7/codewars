#include <iostream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;
using std::string_literals::operator""s;

const char eol = '\n';

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
    end = min (end, src.find ("rem"));
    return src.substr (0, end);
}
vector<string> format (const string &input) {

    istringstream iss (input);
    string line;
    vector<string> ins;

    while (getline (iss, line))
        ins.push_back (sanitize (line));

    return ins;
}
vector<string> tokenize (const string &input) {
    vector<string> code;
    string ins;

    for (auto &it : input) {
        if (it == ' ') {
            code.push_back(ins);
            ins.clear();
        } else {
            ins += it;
        }
    }

    return code;
}
std::string kcuf (const std::string &input) /* throws std::string */ {

    bool running = true;
    vector<string> code = format (input);
    vector<string>::iterator it = code.begin();
    string op;

    while (running) {
        vector<string> line = tokenize (*it);
        op = line[0];
        //cout << line.front() << endl;

        it++;
        if (it == code.end()) running = false;
    }
    /*
    */
    return "";
}
//map<string,bool> comm {{"--",true}, {"#", true}, {"//", true}};
int main () {

    string code =
		"var X//This is a comment\n"
		"read X--This is also a comment\n"
		"msg \"Bye\" X#No doubt it is a comment\n"
		"rem &&Some comment~!@#$\":<";
		//"?","Bye?";

    kcuf (code);


}
