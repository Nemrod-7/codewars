#include <iostream>
#include <string>
#include <codecvt>
#include <locale>


#include <thread>
#include <random>
#include <chrono>

// using namespace std;

bool running = true;

void rain() {
    const int width = 140;
    const int nflip = 5;
    const std::wstring ch = L"1234567890qwertyuiopasdfghjklzxcvbnm,./':<>;[]?!@#$%^&*()-=_+¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶ᐿᥠᘌᕳᕲᎬƂᥫȠͳᘝ૯ΩΨΧΦῃᨒ";
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv1;

    int x = 0;
    bool line[width] = {0};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist (0, ch.size() - 1), pos (0, width-1);

    while (running) {
        for (int i = 0; i < width; i += 2) {
            if (line[i]) {
              std::string u8 = conv1.to_bytes(ch[dist(gen)]);
              std::cout << u8 << " ";
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

    std::wstring ch = L"1234567890qwertyuiopasdfghjklzxcvbnm,./':<>;[]?!@#$%^&*()-=_+¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶ᐿᥠᘌᕳᕲᎬƂᥫȠͳᘝ૯ΩΨΧΦῃᨒ";


    rain();
    // int size = ch.size() - 1;
    // std::wstring_convert<std::codecvt_utf8<wchar_t>> conv1;
    // std::string u8 = conv1.to_bytes(ch[size]);
    // std::cout << u8;
    // std::cout << "\n";

    // thread t1 (rain);
    // string ext = "\033cexit\n";
    // while (running) {
    //   if (std::cin.get())
    //       running = false;
    // }
    //
    // if (t1.joinable())
    //   t1.join();

    // wcout << ext;


    return 0;
}
