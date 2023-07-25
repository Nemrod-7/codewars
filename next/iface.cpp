#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

struct winsize win;
struct termios term;

class display {
  static inline size_t bsize;
  static inline std::string header, footer;
  static inline const std::string reset = "\033[0m", bright = "\033[1m", fainted = "\033[2m", italic = "\033[3m", underline = "\033[4m", blink = "\033[5m", reverse = "\033[7m";

public:
  static void init() {
    ioctl(0, TIOCGWINSZ, &win);

    bsize = win.ws_row - 10;
    header += "┌", footer += "└";

    for (int i = 0; i < win.ws_col - 2; i++) {
      header += "─", footer += "─";
    }
    header += "┐\n", footer += "┘\n";
  }
  static void menu (size_t x, size_t y, bool edit) {
    const std::vector<std::string> menu = {"File","Help","Base","Mode","clear","Time","Rain"};
    const std::vector<std::vector<std::string>> subm {
      {"Save","Load","Exit"}, {"general", "derivation rules"}, {"values", "lambdas"}, {"interpret","derivate","brainfuck"}, {}, {}, {}
    };
    int pad = 3;
    std::cout << "\033c";

    for (size_t j = 0; j < menu.size(); j++) {         // display menu bar
      if (j == x) std::cout << reverse;
      std::cout << " " << menu[j] << " ";
      std::cout << reset;
    }
    std::cout << fainted << reverse;
    std::cout << (edit == false ? " navigation" : "  edition  ");
    std::cout << reset << "\n";
    if (y != 0) {                                   // display submenu
      int space = x;
      for (size_t i = 0; i < x; i++) {
        space += menu[i].size() + 1;
      }
      for (size_t j = 0; j < subm[x].size(); j++) {
        std::cout << std::string (space, ' ');
        if (j + 1 == y) std::cout << reverse;
        std::cout << subm[x][j] << reset << "\n";
      }
      pad -= subm[x].size();
    }
    std::cout << std::string (pad,'\n');
  }
  static void bar (const std::string &input, int x = 0) { // display input bar
    int pos = std::max (0, std::min (x, (int) input.size()));

    std::cout << header;
    std::cout << "│ᐅ ";
    for (int i = 0; i < input.size(); i++) {
        if (i == pos) std::cout << reverse;
        std::cout << input[i] << reset;
    }
    std::cout << std::string(win.ws_col - input.size() - 4, ' ') << "│\n";
    // std::cout << "│ᐅ " << std::left << std::setw(win.ws_col - 4) << input << "│\n";
    std::cout << footer;
  }
  static void box (const std::vector<std::string> &msgbox) { // display text box
    const size_t st = std::max(0, int (msgbox.size() - bsize));
    const size_t nd = st + bsize;

    std::cout << header;
    for (size_t i = st; i < nd; i++) {
      std::cout << "│";
      std::cout << std::left << std::setw(win.ws_col - 2);
      if (i < msgbox.size()) {
        std::cout << msgbox[i] ;
      } else {
        std::cout << " ";
      }
      std::cout << "│\n";
    }
    std::cout << footer;
  }
  static std::string tape (unsigned char tape[], int index) {
      int end = win.ws_col / 4 -1;
      std::stringstream os;

      os << "\n";
      for (int i = 0; i < end; i++) {
          os  << std::setw (4) << i;
      }
      os << '\n';
      os << "┌";
      for (int i = 0; i < end - 1; i++) {
          os << "───┬";
      }
      os << "───┐\n│";

      for (int i = 0; i < end; i++) {
          os << std::setw(3) << static_cast<int> (tape[i]);
          os << "│";
      }
      os << "\n└";
      for (int i = 0; i < end - 1; i++) {
          os << "───┴";
      }
      os << "───┘\n";
      for (int i = 0; i < end; i++) {
          os  << std::setw (4) << (i == index ? "^" : " ");
      }

      return os.str() + "\n";
  }
};

unsigned loop (const std::string &code, unsigned pos) {
    int pile = 0, index = pos;
    const bool fwrd = code[index] == '[' ? true : false;

    while (code[index]) {
        pile += (code[index] == '[') - (code[index] == ']');
        if (pile == 0) return index;

        fwrd == true ? index++ : index--;
    }

    return index;
}
std::string BFrun (const std::string &code, std::string input) {
  if (code.size() == 0) return "";

    int bal = 0;
    for (auto &it : code) {
        bal += (it == '[') - (it == ']');
    }
    if (bal != 0) throw std::logic_error ("bad format code");

    unsigned char tape[10000] = {}, *ptr = tape;
    int index = 0;
    auto ins = input.begin();
    std::string os;

    while (code[index]) {
      int pos = ptr - tape;
      if (pos < 0 || pos > 10000) {
          throw std::runtime_error ("Stack smashing detected");
      }
      switch (code[index]) {
        case '>': ptr++; break;
        case '<': ptr--; break;
        case '+': (*ptr)++; break;
        case '-': (*ptr)--; break;
        case '.': os += *ptr; break;
        case ',': *ptr = ins != input.end() ? *ins++ : 0; break;
        case '[': if (*ptr == 0) index = loop (code,index); break;
        case ']': if (*ptr != 0) index = loop (code,index); break;
      }
      index++;
    }

    os += "\n";
    os += display::tape (tape, ptr - tape);
    return os ;
}

class interface {
      enum keys {enter = 10, escape = 27, space = 32, up = 65, down = 66, right = 67, left = 68, del = 126, back = 127};
      enum mode_m {nul, interpreter, derivation, brainfuck};

      bool running;
      std::string input, expression;
      std::string result;
      int x = 0, y = 0;

      private :
          int getkey() {
                int key = getchar();

                switch (key) {
                    case escape:
                        key = getchar();
                        if (key == 91) { key = getchar(); }
                        break;
                    default : break;
                }

                return key;
          }
      public :
          interface () {
              setlocale (LC_ALL, "en_US.UTF-8");

              tcgetattr(STDIN_FILENO, &term);
              term.c_lflag &= ~ICANON;
              tcsetattr(STDIN_FILENO, TCSANOW, &term);
              display::init();
              x = 0, y = 0;
              running = true;
          }
          void brain() {
              const std::string reset = "\033[0m", bright = "\033[1m", fainted = "\033[2m", italic = "\033[3m", underline = "\033[4m", blink = "\033[5m", reverse = "\033[7m";

              std::vector<std::string> msg;
              bool edit  = false;
              int pos = 0;

              while (running) {
                  display::menu(x,y);
                  display::bar(input, pos);
                  display::box(msg);

                  int key = getkey();
                  std::string line;
                  std::stringstream iss;

                  switch (key) {
                      case 96: edit ^= 1; break;

                      case escape: break;
                      case up: y--; break;
                      case down: y++; break;
                      case right: (edit == false) ? x++ : pos++; break;
                      case left : (edit == false) ? x-- : pos--; break;
                      case enter: if (y == 0) expression = input, input.clear(); break;
                      case back:
                          if (pos > 0) {
                              if (pos == input.size()) {
                                input.pop_back();
                              } else if (pos < input.size()){
                                input.erase(input.begin() + pos - 1);
                              }
                              pos--;
                          }
                          break;
                      case del:
                          if (x < input.size()) {
                              input.erase(input.begin() + pos);
                              input.erase(input.begin() + pos - 1);
                              pos--;
                          }
                          break;
                      default :
                          if (pos == input.size()) {
                              input += key;
                              pos++;
                          } else if (pos < input.size()) {
                              input.insert(input.begin() + pos, key);
                              pos++;
                          }
                          break;
                  }

                  pos = std::max (0, std::min (pos, (int)input.size()));
                  x = std::max (0, std::min (x, 6));
                  y = std::max (0, std::min (y, 3));

                  if (key == enter) {
                      if (y == 0) {
                          try {
                              result = BFrun (expression, "string");
                              iss << "Result: " << result ;
                          } catch (std::exception &x) {
                              iss << "Error : " << x.what() << "\n";
                          }
                          input = expression;
                      } else if (y == 3) {
                          running = false;
                      }
                  }
              }
          }
};

int main () {

    interface curr;
    curr.brain();


    std::string input = "the sky above the port was the color of a television tuned to a dead channel";

    // std::cout << std::setw(10) <<

    std::cout << "\nEXIT\n";
}
