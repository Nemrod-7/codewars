#include <iostream>
#include <string>

const int overflow = 255;

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
std::string brainfuck (std::string code, std::string input) {

    std::string output, tape (1024, 0);
    auto ins = input.begin(), ptr = tape.begin();
    int index = 0;

    while (code[index]) {
        switch (code[index]) {
            case '>': ptr++; break;
            case '<': ptr--; break;
            case '+': (*ptr)++; break;
            case '-': (*ptr)--; break;
            case '.': output += *ptr; break;
            case ',': *ptr = ins != input.end() ? *ins++ : *ptr = overflow; break;
            case '[': if (*ptr == 0) index = loop (code,index); break;
            case ']': if (*ptr != 0) index = loop (code,index); break;
        }
        index++;
    }

    return output;
}

int main () {

  std::string tw = "codewars";

  std::cout << brainfuck(",+[-.,+]",tw);

  return 0;
}
