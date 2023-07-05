#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>


#include <thread>
#include <random>
#include <chrono>

// using namespace std;

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

template<typename Func> static void load (Func fn);

namespace module {
    bool process;

    void rain() {
      const int width = w.ws_col - 1;
      const int nflip = 5;
      const std::wstring ch = L"1234567890qwertyuiopasdfghjklzxcvbnm,./':<>;[]?!@#$%^&*()-=_+¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶ᐿᥠᘌᕳᕲᎬƂᥫȠͳᘝ૯ΩΨΧΦῃᨒ";

      int x = 0;
      bool line[width] = {0};
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> dist (0, ch.size() - 1), pos (0, width-1);

      while (process) {
        for (int i = 0; i < width; i += 2) {
          if (line[i]) {
            std::cout << u8 (ch[dist(gen)]) << " ";
          } else {
            std::cout << "  ";
          }
        }

        for (int i = 0; i != nflip; i++) {
          x = pos(gen);
          line[x] = !line[x];
        }

        std::cout << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(70));
      }
    }
};
namespace prog {
    void execute () {
      load (module::rain);
    }
}

template<typename Func> static void load (Func fn) {

    input_enter_off();
    module::process = true;
    std::thread t1 (fn);

    while (module::process) {
        if (getchar()) {
            module::process = false;
        }
    }

    if (t1.joinable())
        t1.join();

      input_enter_on();
}


int main() {

    setlocale (LC_ALL, "en_US.UTF-8");
    initdim();



    std::cout << "exit\n";

    return 0;
}
