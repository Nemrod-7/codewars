#include <iostream>
#include <string>

#include <sys/ioctl.h>

#include <thread>
#include <random>
#include <chrono>

// using namespace std;

bool running = true;

int getwidth () {
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);
  return w.ws_col;
}

std::string convert (const wchar_t wc) {
    char buffer[8] = {0};
    bool shifts = std::wctomb(nullptr, 0); // reset the conversion state
    const int ret = std::wctomb (buffer, wc);
    if (ret < 0) throw std::invalid_argument("inconvertible wide characters in the current locale");
    buffer[ret] = '\0';  // make 'buffer' contain a C-style string
    return std::string (buffer);
}

void progress (const std::wstring &bar) {

    int cycle = 0;
    std::chrono::steady_clock::time_point now, roll = std::chrono::steady_clock::now();
    std::chrono::duration<double> interval;

    while (running) {
        now = std::chrono::steady_clock::now();
        interval = now - roll;

        if (interval >= std::chrono::milliseconds(100)) {
              std::cout << "\033c";
              std::cout << "\033[1;1m";
              std::cout << convert (bar[cycle]);

              std::cout << " type input => " << std::flush;
              // std::cout << "Waiting... " << "\n" << std::flush;
              cycle = cycle == bar.size() - 1 ? 0 : cycle + 1;
              roll = now;
        }
    }
}

void rain() {
    const int width = getwidth() - 1;
    const int nflip = 5;
    const std::wstring ch = L"1234567890qwertyuiopasdfghjklzxcvbnm,./':<>;[]?!@#$%^&*()-=_+¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶ᐿᥠᘌᕳᕲᎬƂᥫȠͳᘝ૯ΩΨΧΦῃᨒ";

    int x = 0;
    bool line[width] = {0};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist (0, ch.size() - 1), pos (0, width-1);

    while (running) {
        for (int i = 0; i < width; i += 2) {
            if (line[i]) {
              std::cout << convert (ch[dist(gen)]) << " ";
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

int main() {

    setlocale (LC_ALL, "en_US.UTF-8");

    const std::wstring bar = L"⠁⠂⠄⡀⢀⠠⠐⠈";
    std::thread t1 (rain);
    std::string input;

    while (running) {
        if (std::cin.get()) {
            running = false;
        }
    }

    if (t1.joinable())
      t1.join();

        std::string rules =
        "rules of derivation: \n\n"
        "cst : cst   => 0\n"
        "lin : x     => 1\n"
        "add : a + b => a' + b'\n"
        "min : a - b => a' - b'\n"
        "mul : a . b => a.b' + a'.b\n"
        "div : a / b => (a'. b − b'. a) / b²\n"
        "pow : x^a   => a.x^(a - 1)\n"
        "exp : e^x   => e^x\n"
        "exp : a^x   => a^x . ln a\n"
        "ln  : ln(x) => 1 / x\n"
        "sin : sin x => cos x\n"
        "cos : cos x => -sin x\n"
        "tan : tan x => 1 / cos²x\n";
    // wcout << ext;

    std::cout << "exit\n";

    return 0;
}

std::string wchartostring1 (wchar_t wch) { // deprecated
  // #include <locale>
  // #include <codecvt>
  // std::wstring_convert<std::codecvt_utf8<wchar_t>> conv1;
  // std::string u8 = conv1.to_bytes(ch[dist(gen)]);
  // std::cout << u8 << " ";
  return "";
}
