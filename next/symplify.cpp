#include <iostream>
//#include <regex>
#include <vector>
#include <algorithm>

using namespace std;
/*
vector<string> tokenize (string src) {
    const regex re ("[\\+\\-]?[0-9]*[a-z]+");
    sregex_token_iterator it (src.begin(), src.end(), re);
    return {it, std::sregex_token_iterator()};
}
*/

int getsign (const std::string &src) {
    if (src[0] == '-') return -1;
    return 1;
}
int getmult (const string &src) {
    int num = 1;
    sscanf (src.c_str(), "%i", &num);
    return num;
}

string getvar (const string &src) {
    string var;
    for (int i = 0; i < src.size(); i++) {
        if (isalpha(src[i])) var += src[i];
    }
    return var;
}


pair<int,string> getcell(const string &src) {

    int sig = 1;
    int num = 0;
    string var;

    for (auto &it : src) {
        if (it == '-') sig *= -1;
        if (isdigit(it)) num = num * 10 + it - '0';
        if (isalpha(it)) var += it;
    }

    sort(var.begin(), var.end());
    if (num == 0) num = 1;
    return {sig * num, var};
}

int main () {

    string src = "3x-yx+2xy-x"; // 2x + xy
    //"-a+5ab+3a-c-2a";
    //vector<string> token = tokenize(src); 
    vector<string> token = {"3x","-yx","+2xy", "-x"};
    vector<string> res;

    for (int i = 0; i < token.size(); i++) {
        auto [num, var] = getcell(token[i]);

        for (int j = i + 1; j < token.size(); j++) {
            auto [nxn, nxv] = getcell(token[j]);

            if (nxv == var) {
                num += nxn;
                token.erase(token.begin() + j);
            }
        }
        cout << num << var << '\n';
    }

}
