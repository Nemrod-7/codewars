#include <iostream>
#include <iomanip>
#include <string>

#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

#include <thread>
#include <random>
#include <chrono>

using namespace std;

struct termios t;
struct winsize w;

void input_enter_off() {
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
void input_enter_on() {
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
void initdim () {
  ioctl(0, TIOCGWINSZ, &w);
}

std::string u8 (const wchar_t wc) {
    char buffer[8] = {0};
    bool shifts = std::wctomb(nullptr, 0); // reset the conversion state
    const int ret = std::wctomb (buffer, wc);
    if (ret < 0) throw std::invalid_argument("inconvertible wide characters in the current locale");
    buffer[ret] = '\0';  // make 'buffer' contain a C-style string
    return std::string (buffer);
}

const std::string divers =
      "Constants :\n" "       e      pi\n\n" "Functions :\n" "     log      ln     rad     sqr\n" "     cos    acos     deg     abs\n"
      "     sin    asin    sqrt        \n" "     tan    atan                \n\n" "Operators :\n" "+ - * % / ^ = =>\n\n"
      "lambdas :\n" "fn 'name' 'parameters' => 'function' \n" "ex : fn avg x y => (x + y) / 2\n\n";

const std::string rules =
      "rules of derivation: \n\n" "cst : cst   => 0\n" "lin : x     => 1\n" "add : a + b => a' + b'\n" "min : a - b => a' - b'\n"
      "mul : a . b => a.b' + a'.b\n" "div : a / b => (a'. b − b'. a) / b²\n" "pow : x^a   => a.x^(a - 1)\n" "exp : e^x   => e^x\n"
      "exp : a^x   => a^x . ln a\n" "ln  : ln(x) => 1 / x\n" "sin : sin x => cos x\n" "cos : cos x => -sin x\n" "tan : tan x => 1 / cos²x\n";

void apppend (std::vector<std::string> &history, const std::string &txt) {
    std::istringstream iss (rules);
    std::string buffer;

    while (getline (iss, buffer)) {
        history.push_back(buffer);
    }
}

int main() {

    setlocale (LC_ALL, "en_US.UTF-8");
    initdim();
    input_enter_off();

    const std::vector<std::string> menu = {"File","Help","Base","Mode","clear","Time","Rain"};
    int width = w.ws_col, height = w.ws_row;

    std::string head, bott;

    head += "┌", bott += "└";
    for (int i = 0; i < width - 2; i++) {
        head += "─";
        bott += "─";
    }
    head += "┐\n", bott += "┘\n";


    std::string input = "x + 1 = 3";
    std::cout << "\033c";

    for (int j = 0; j < menu.size(); j++) {         // display menu bar
        std::cout << " " << menu[j] << " ";
    }

    std::cout << std::string (4, '\n');
    std::cout << head;
    std::cout << "│ᐅ " << std::left << std::setw(width-4) << input << "│\n";
    std::cout << bott;
    const int hsize = 5;



    std::vector<std::string> history;
    int fst = history.size() - hsize;

    std::cout << head;
    for (int i = 0; i < hsize; i++) {
        std::cout << "│";
        std::cout << std::setw(width - 2);
        if (i + fst >= 0) {
            std::cout << history[i] ;
        } else {
            std::cout << " ";
        }
        std::cout << "│\n";
    }
    std::cout << bott;
    // std::cout << bott;

    std::cout << "\nexit\n";
    return 0;
}
