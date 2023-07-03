#include <iostream>
#include <string>

#include <thread>
#include <random>
#include <chrono>

using namespace std;

bool running = true;

void rain() {
    const int width = 140;
    const int nflip = 5;
    const wstring ch = L"1234567890qwertyuiopasdfghjklzxcvbnm,./':<>;[]?!@#$%^&*()-=_+¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶ᐿᥠᘌᕳᕲᎬƂᥫȠͳᘝ૯ΩΨΧΦῃᨒ";

    int x = 0;
    bool line[width] = {0};
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist (0, ch.size() - 1), pos (0, width-1);

    while (running) {
        for (int i = 0; i < width; i += 2) {
            if (line[i])
                wcout << ch[dist(gen)] << " ";
            else
                wcout << "  ";
        }

        for (int i = 0; i != nflip; i++) {
            x = pos(gen);
            line[x] = !line[x];
        }
        wcout << "\n";
        this_thread::sleep_for(chrono::milliseconds(70));
    }
}

int main() {

    setlocale (LC_ALL, "en_US.UTF-8");

    thread t1 (rain);
    string ext = "\033cexit\n";
    while (running) {
      if (std::cin.get())
          running = false;
    }

    if (t1.joinable())
      t1.join();

    // wcout << ext;


    return 0;
}
