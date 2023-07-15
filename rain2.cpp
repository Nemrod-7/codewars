#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

#include <thread>
#include <random>
#include <chrono>

// using namespace std;
// const std::wstring glyph = L"1234567890qwertyuiopasdfghjklzxcvbnm,./':<>;[]?!@#$%^&*()-=_+¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶ᐿᥠᘌᕳᕲᎬƂᥫȠͳᘝ૯ΩΨΧΦῃᨒ";
const std::wstring glyph = L"/<>[]?!@#$%^&*()-=_+¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶ᐿᥠᘌᕳᕲᎬƂᥫȠͳᘝ૯ΩΨΧΦῃᨒ";

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
void rain2 () {

  		const int width = w.ws_col - 1, height = w.ws_row - 1;
  		const int nflip = 5;

  		int x = 0;
  		bool line[width] = {0};
  		std::random_device rd;
  		std::mt19937 gen(rd());
  		std::uniform_int_distribution<> dist (0, glyph.size() - 1), pos (0, width-1);
  		std::vector<std::wstring> screen (height);

  		int cycle = 100;


  while (cycle-->0) {
      int index = screen.size();
      std::wstring first;

      std::cout << "\33c";

      for (int i = 0; i < screen.size(); i++) { // render screen
          for (int j = 0; j < screen[i].size(); j++) {
              if (screen[i][j] == ' ') {
                  std::cout << ' ';
              } else if (i < screen.size() - 1 && j < screen[i+1].size() && screen[i+1][j] == ' ') {
                  std::cout << "\033[1m" << u8 (glyph[dist(gen)]) << "\033[0m";
              } else {
                  std::cout << u8 (screen[i][j]);
              }
          }

          std::cout << "\n" << std::flush;
      }

      while (index-->1) { // shift the screen down
          screen[index] = screen[index-1];
      }

      for (int i = 0; i != nflip; i++) { // generate a new line
          x = pos(gen);
          line[x] = !line[x];
      }

      for (int i = 0; i < width; i += 2) {
          first += line[i] ? glyph[dist(gen)] : ' ';
          first += ' ';
      }
      screen[0] = first;

      std::this_thread::sleep_for(std::chrono::milliseconds(70));
  }

}
void rain3 () {
  const int width = w.ws_col - 1, height = w.ws_row - 1;
  const int nflip = 5;
  const std::string reset = "\033[0m";

  int x = 0;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist (0, glyph.size() - 1), pos (0, width-1);

  std::vector<std::wstring> mask (height);
  std::vector<std::vector<bool>> matrix (height);

  int cycle = 100;
  matrix[0] = std::vector<bool> (width);

  for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
          mask[i] += glyph[dist(gen)];
      }
  }

  while (cycle-->0) {
      int index = matrix.size();

      std::cout << "\33c";

      for (int i = 0; i < height; i++) { // render matrix
          for (int j = 0; j < matrix[i].size(); j++) {
              if (matrix[i][j] == 0) {
                  std::cout << ' ';
              } else if (i < height - 1 && matrix[i+1][j] == 0) {
                  std::cout << "\033[1;95m" << u8 (glyph[dist(gen)]) << reset;
              }
              else if (i > 0 && matrix[i-1][j] == 0) {
                  std::cout << "\033[2m" << u8 (mask[i][j]) << reset;
              }
              else {
                  std::cout << u8 (mask[i][j]);
              }
          }
          std::cout << "\n";
      }

      while (index-->1) { // shift the matrix down
          matrix[index] = matrix[index-1];
      }

      for (int i = 0; i != nflip; i++) { // generate a new line
          x = pos(gen);
          matrix[0][x] = !matrix[0][x];
      }

      for (int i = 0; i < width; i += 2) {
          matrix[0][i] = 0;
      }

      std::cout << std::flush;
      std::this_thread::sleep_for(std::chrono::milliseconds(70));
  }
}

int main() {

    setlocale (LC_ALL, "en_US.UTF-8");
    initdim();

    const int width = w.ws_col - 1, height = w.ws_row - 1;

    const std::string reset = "\033[0m";

    std::cout << "\033[0m" << "reset\n" << reset;
    std::cout << "\033[1m" << "bright\n" << reset;
    std::cout << "\033[2m" << "fainted\n" << reset;
    std::cout << "\033[3m" << "italic\n" << reset;
    std::cout << "\033[4m" << "underlined\n" << reset;
    std::cout << "\033[5m" << "blink\n" << reset;
    std::cout << "\033[6m" << "blink\n" << reset;
    std::cout << "\033[7m" << "reverse\n" << reset;

    return 0;
}
