#include <iostream>
#include <unistd.h>

#include <termios.h>
#include "chess.hpp"


int main() {

    setlocale (LC_ALL, "en_US.UTF-8");

    Board board;
    board.create();

    //Display::large(board);
}
