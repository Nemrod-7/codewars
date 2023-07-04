#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sys/ioctl.h>

void head (int width) {
    std::cout << "┌";
    for (int i = 0; i < width - 2; i++) {
        std::cout << "─";
    }
    std::cout << "┐\n";
}
int main () {

    setlocale (LC_ALL, "en_US.UTF-8");

    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    const int width = w.ws_col, height = w.ws_row;

    // const wstring sw = L"─";
    // const wstring ss = L"┌", es = L"┐\n";
    // const wstring sl = L"│", nl = L"│\n";
    // const wstring ns = L"└", ne = L"┘\n";

    std::cout << "┌";
    for (int i = 0; i < width - 2; i++) {
        std::cout << "─";
    }
    std::cout << "┐\n";

    for (int i = 0; i < height - 2; i++) {
        std::cout << "│";

        std::cout << std::setw (width+2);


        std::cout << "│\n";
    }



    // std::cout << "exit";
}
