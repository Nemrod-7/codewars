#include <string>

std::string remove_parentheses(const std::string &str) {
    int pile = 0;
    std::string os;

    for (auto &c : str) {
        if (c == '(') pile++;
        if (pile == 0) os += c;
        if (c == ')') pile--;
    }
    return os;
}
