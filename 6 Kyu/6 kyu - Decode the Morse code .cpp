#include <iostream>
#include <sstream>
#include <map>
#include <vector>

std::map<std::string, std::string> MORSE_CODE =
{{".-", "A"},{"-...", "B"},{"-.-.", "C"},{"-..", "D"},{".", "E"},{"..-.", "F"},
{"--.", "G"},{"....", "H"},{"..", "I"},{".---", "J"},{"-.-", "K"},{".-..", "L"},{"--", "M"},
{"-.", "N"},{"---", "O"},{".--.", "P"},{"--.-", "Q"},{".-.", "R"},{"...", "S"},{"-", "T"},
{"..-", "U"},{"...-", "V"},{".--", "W"},{"-..-", "X"},{"-.--", "Y"},{"--..", "Z"},{"-----", "0"},
{".----", "1"},{"..---", "2"},{"...--", "3"},{"....-", "4"},{".....", "5"},{"-....", "6"},
{"--...", "7"},{"---..", "8"},{"----.", "9"}};

std::string decodeMorse(std::string morseCode);

int main () {


    std::cout << decodeMorse(".... . -.--   .--- ..- -.. .");
    return 0;
}

std::string decodeMorse(std::string morseCode) {


    std::istringstream iss(morseCode);
    std::string tok, os;

    while (iss >> tok) {
        os += MORSE_CODE[tok];
        if (iss.get() == ' ' && iss.get() == ' ')
            os += ' ';
        else
            iss.unget();
    }
    os.erase(os.find_last_not_of(' ')+1);

    return os;
}
