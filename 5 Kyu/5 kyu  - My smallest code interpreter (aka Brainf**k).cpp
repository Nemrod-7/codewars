#include <iostream>
#include <string>
#include <vector>

#define BUFFSIZE 1024
#define OVERFLOW 255

std::string brainLuck(std::string code, std::string input);


int main () {

    std::string tw = "codewars";
    std::cout << brainLuck(",+[-.,+]",tw);

    return 0;
}

unsigned loop (const std::string code, unsigned pos) {
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
std::string brainLuck (std::string code, std::string input) {

    std::string output, tape;
    auto pos = input.begin(), ptr = tape.begin();
    int index = 0;

    while (code[index]) {
        switch (code[index]) {
            case '>': ptr++; break;
            case '<': ptr--; break;
            case '+': (*ptr)++; break;
            case '-': (*ptr)--; break;
            case '.': output += *ptr; break;
            case ',': *ptr = pos != input.end() ? *pos++ : *ptr = OVERFLOW; break;
            case '[': if (*ptr == NULL) index = loop (code,index); break;
            case ']': if (*ptr != NULL) index = loop (code,index); break;
        }
        index++;
    }

    return output;
}
