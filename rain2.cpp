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


int main() {

    setlocale (LC_ALL, "en_US.UTF-8");
    initdim();

		const std::wstring data = L"1234567890qwertyuiopasdfghjklzxcvbnm,./':<>;[]?!@#$%^&*()-=_+¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶ᐿᥠᘌᕳᕲᎬƂᥫȠͳᘝ૯ΩΨΧΦῃᨒ";
		const int width = w.ws_col - 1, height = w.ws_row - 1;
		const int nflip = 5;
		int x = 0;
		bool line[width] = {0};
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dist (0, data.size() - 1), pos (0, width-1);

		std::cout << "width: " << width << " height: " << height << "\n";
		std::string first;
		std::vector<std::string> screen (height, std::string (width ,'.'));

		int cycle = 10;

		for (int i = 0; i != nflip; i++) {
				x = pos(gen);
				line[x] = !line[x];
		}

		for (int i = 0; i < width; i += 2) {
				if (line[i]) {
						first += u8 (data[dist(gen)]) + " ";
				} else {
						first += "  ";
				}
		}

		std::cout << first << "\n";

		for (int i = 0; i < height - 2; i++) {
				// std::cout << screen[i] << "\n";
		}



    std::cout << "exit\n";

    return 0;
}
