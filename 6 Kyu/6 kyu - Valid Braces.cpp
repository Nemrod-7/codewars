#include <iostream>
#include <string>
#include <vector>

bool valid_braces1 (std::string braces) {

    std::vector<char> brc;

    for (auto &cur : braces) {

        if (cur == '(' || cur == '[' || cur == '{') {
            brc.push_back (cur);
        } else {
            if (!brc.size()) return false;
            char last = brc.back();

            if ((cur == ')' && last == '(') || (cur == ']' && last == '[') || (cur == '}' && last == '{')) {
                brc.pop_back();
            } else {
                return false;
            }
        }

    }

    return brc.size() == 0;
}
bool valid_braces (std::string braces) {

    std::vector<char> brc;

    for (auto &ch : braces) {
        switch (ch) {
            case '[' : brc.push_back(']'); break;
            case '{' : brc.push_back('}'); break;
            case '(' : brc.push_back(')'); break;
            default : if (ch == brc.back()) brc.pop_back();
                      else return false;
                      break;
        }
    }

    return brc.empty();
}
int main () {

    std::cout << valid_braces("(})");

}
