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

string mkup (int width) {
    string os;
    os += "┌";
    for (int j = 0; j < width; j++) {
        os +="─";
    }
    os +="┐";


    return os;
}
string mkbot (int width) {
    string os;
      os +=  "└";
      for (int j = 0; j < width; j++) {
          os += "─";
      }
      os += "┘";
    return os;
}

void getime () {
    const auto now = std::chrono::system_clock::now();
    const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
    std::cout << "The clock is currently at " << std::ctime(&t_c);
}


void interface () {
  enum escape {enter = 10, esc = 27};

  const string reset = "\033[0m", bright = "\033[1m", fainted = "\033[2m", italic = "\033[3m", underline = "\033[4m", blink = "\033[5m", reverse = "\033[7m";
  const vector<string> menu = {"exit","help","clear","time","mode"};
  const vector<string> mode = {"derivate","interpret"};
  const int nopt = menu.size() - 1;

  int width = 5, height = 5;

  bool running = true, modec = false;
  int x = 0, y = 0;

  while (running) {
      int opt = 4;
      int key = getc (stdin);

      switch (key) { // getkey
          case enter : opt = x; break; // enter
          case 97  : x--; break;
          case 100 : x++; break;
          case 115 : y++; break;
          case 119 : y--; break;
          // case esc : running = false; break;
          default : cout << " " << key << "\n"; break;
      }

      x = min (max (x, 0), nopt);
      cout << "\033c";
      // menu bar
      for (int j = 0; j < menu.size(); j++) {
          if (j == x) cout << reverse;
          cout << " " << menu[j] << " ";
          cout << reset;
      }
      cout << "\n";

      switch (opt) {
          case 0 : running = false; break;
          case 1 : cout << "help : Iface Usage...\n"; break;
          case 2 : cout << "\033c"; break;
          case 3 : getime(); break;
          case 4 : cout << "mode : \n";
              modec = true;
              break;
          default : break;
      }

      if (modec == true) {
        y = min (max (y, 0), 1);
        cout << y << "\n";
        cout << "┌──────────┐\n";

        for (int j = 0; j < 2; j++) {
            cout << "│";
            if (j == y) cout << reverse;

            cout << setw (10);
            cout << mode[j];
            cout << reset;
            cout << "│\n";
        }

        cout << "└──────────┘\n";
      }

  }

}
int main() {

    setlocale (LC_ALL, "en_US.UTF-8");
    initdim();
    input_enter_off();

    enum escape {enter = 10, esc = 27};
    enum  {navig, type};
    const string reset = "\033[0m", bright = "\033[1m", fainted = "\033[2m", italic = "\033[3m", underline = "\033[4m", blink = "\033[5m", reverse = "\033[7m";
    const vector<string> menu = {"exit","help","clear","time","mode"};
    const int nopt = menu.size() - 1;
    const int width = 5, height = 5;

    int x = 0, y = 0;
    int mode = navig;
    bool running = true, modec = false;
    string input;

    while (running) {

        int opt = -1;
        int key = getc (stdin);

        cout << "\033c";

        if (mode == type) {
            if (key == enter) {
                input.clear();
            } else {
                input += key;
            }
        } else {
            switch (key) { // getkey
              case enter : opt = x; break; // enter
              case 97  : x--; break;
              case 100 : x++; break;
              case 115 : y++; break;
              case 119 : y--; break;
              default  : break;
            }
        }

        x = min (max (x, 0), nopt);
        // menu bar
        for (int j = 0; j < menu.size(); j++) {
            if (j == x) cout << reverse;
            cout << " " << menu[j] << " ";
            cout << reset;
        }
        cout << "\n";

        switch (opt) {
            case 0 : running = false; break;
            case 1 : cout << "help : Iface Usage...\n"; break;
            case 2 : cout << "\033c"; break;
            case 3 : getime(); break;
            case 4 :
                cout << "mode : \n";
                modec = true;
                break;
            default :

            cout << "input > " << input << "\n";
            break;
        }


    }

    std::cout << "\nexit\n";


    return 0;
}
