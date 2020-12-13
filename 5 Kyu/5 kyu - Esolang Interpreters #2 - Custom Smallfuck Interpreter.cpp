#include <iostream>

size_t loop (const std::string &code, size_t it) {
    int pile = 0;
    const bool fwd = code[it] == '[' ? true : false;

    while (code[it]) {
        if (code[it] == '[') pile++;
        if (code[it] == ']') pile--;
        if (pile == 0) return it;

        fwd == true ? it++ : it--;
    }
    return 0;
}
std::string interpreter(const std::string &code, const std::string &tape) {
    std::string os = tape;
    size_t in = 0, it = 0;

    while (in != code.size()) {
        char com = code[in];

        switch (com) {
            case '>' : it++; break;
            case '<' : it--; break;
            case '*' : os[it] = os[it] == '0' ? '1' : '0'; break;
            case '[' : if (os[it] == '0') in = loop (code, in); break;
            case ']' : if (os[it] != '0') in = loop (code, in); break;
        }

        if (it > tape.size () - 1 || it < 0) break;
        in++;
    }

    return os;
}

int main () {}
