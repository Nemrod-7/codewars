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

    unsigned char tape[10000] = {}, *ptr = tape;
    std::string output;
    auto ins = input.begin();
    int index = 0;

    while (code[index]) {
        switch (code[index]) {
            case '>': ptr++; break;
            case '<': ptr--; break;
            case '+': (*ptr)++; break;
            case '-': (*ptr)--; break;
            case '.': output += *ptr; break;
            case ',': *ptr = ins != input.end() ? *ins++ : 0; break;
            case '[': if (*ptr == 0) index = loop (code,index); break;
            case ']': if (*ptr != 0) index = loop (code,index); break;
        }
        index++;
    }
    for (int i = 0; i < 50; i++) {
      std::cout << (int) tape[i] << ' ';
    }
    std::cout << '\n';

    return output;
}

int main () {

  std::string tw = "23"; // ,+[-.,+]

  std::string code =

  //">+++[-<+++++++++++>]<-" // generate space code : 32
  //">-[[->+>+<<]>[-<+>]+>-]<-<-<"
  "+"

  ;

  std::cout << brainfuck(code,tw);

  return 0;
}
