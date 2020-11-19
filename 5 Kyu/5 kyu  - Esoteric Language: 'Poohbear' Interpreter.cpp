#include <iostream>
#include <sstream>
#include <cmath>

#define BUFFSIZE 1024
#define BUFFER 30000
#define CODE sourcecode[index]

const char *poohbear (const char* sourcecode);

int main () {

    std::string oo;
    //std::ostringstream os;
    oo += 1 + '0';
    oo += "i";
  //  std::cout << oo;
    std::cout << poohbear ("N");
    //std::cout << poohbear ("LQTcQAP>pQBBTAI-PA-PPL+P<BVPAL+T+P>PL+PBLPBP<DLLLT+P");

    return 0;
}

unsigned loop (const char *sourcecode, int pos) {
    int pile = 0, index = pos;
    const bool fwrd = CODE == 'W' ? true : false;

    while (CODE) {
        if (CODE == 'W') pile++;
        if (CODE == 'E') pile--;
        if (pile == 0) return index;

        fwrd == true ? index++ : index--;
    }
}
const char *poohbear (const char* sourcecode) {

    unsigned index = 0;
    std::string os;
    unsigned char tape[BUFFER] = {0} , *cell = tape, tmp;

    os.clear();
    while (CODE) {
        switch (CODE) {
            case '+' : (*cell)++; break;
            case '-' : (*cell)--; break;
            case '>' : cell++; break;
            case '<' : if (cell != tape) cell--; break;
            case 'c' : tmp = *cell ; break;
            case 'p' : *cell = tmp; break;
            case 'W' : if (*cell == 0) index = loop (sourcecode,index); break;
            case 'E' : if (*cell != 0) index = loop (sourcecode,index); break;
            case 'P' : os += *cell; break;
            case 'N' : os += std::to_string(*cell); break;
            case 'T' : *cell *= 2; break;
            case 'Q' : *cell = *cell * *cell; break;
            case 'U' : *cell = sqrt (*cell); break;
            case 'L' : *cell += 2; break;
            case 'I' : *cell -= 2; break;
            case 'V' : *cell /= 2; break;
            case 'A' : *cell += tmp; break;
            case 'B' : *cell -= tmp; break;
            case 'Y' : *cell *= tmp; break;
            case 'D' : *cell /= tmp; break;
            default : break;
        }
        index++;
    }

    return os.c_str();
}
