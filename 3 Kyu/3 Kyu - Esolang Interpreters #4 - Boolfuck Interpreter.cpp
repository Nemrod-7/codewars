#include <iostream>
#include <cstdlib>

unsigned loop (const char *code, unsigned index) {
    int pile = 0;
    const bool fwd = code[index] == '[' ? true : false;

    while (code[index]) {
       pile += code[index] == '[' - code[index] == ']';
       if (pile == 0) return index;

       fwd == true ? index++ : index--;
    }
}
char *boolfuck (const char *code, const char *input) {

  char *output = new char [1024], *out = output;
  unsigned index = 0, bit = 0, bin = 0;
  bool tape[30000] = {0}, *data = &tape[30000 / 2] ;

  while (code[index]) {
      switch (code[index]) {
          case '+' : *data ^= 1; break;
          case ',' : *data = input[bin / 8] >> (bin % 8) &1;
                     bin++;
                     break;

          case ';' : output[bit / 8] |= *data << (bit % 8);
                     bit++;
                     break;

          case '<' : data--; break;
          case '>' : data++; break;
          case '[' : if (*data == 0) index = loop (code,index); break;
          case ']' : if (*data != 0) index = loop (code,index); break;
          default : break;
      }
      index++;
    }

    return output;
}

int main () {

  const char *code = ";;;+;+;;+;+;+;+;+;+;;+;;+;;;+;;+;+;;+;;;+;;+;+;;+;+;;;;+;+;;+;;;+;;+;+;+;;;;;;;+;+;;+;;;+;+;;;+;+;;;;+;+;;+;;+;+;;+;;;+;;;+;;+;+;;+;;;+;+;;+;;+;+;+;;;;+;+;;;+;+;+;";

  std::cout << boolfuck(code,"Hello, world!\n");

  int data = 9;

  for (int bit = 0; bit < 8; bit++) {
      int ex = data >> bit &1;
  }


}
