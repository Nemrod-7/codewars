#include <iostream>
#include <sstream>
#include <vector>


std::string replace_all (std::string src, std::string from, std::string to) {

    int pos = 0;

    while ((pos = src.find(from, pos)) != std::string::npos) {
        src.replace(pos, from.length(), to);
        pos += to.size();
    }

    return src;
}

std::vector<std::string> tokenize(std::string src) {

    src = replace_all(src, "01", "0 1");
    src = replace_all(src, "10", "1 0");
    std::string buffer;
    std::istringstream iss (src);
    std::vector<std::string> vec;

    while (iss >> buffer) vec.push_back(buffer);

    return vec;
}

int main () {

    std::string src = "0000000011011010011100000110000001111110100111110011111100000000000111011111111011111011111000000101100011111100000111110011101100000100000";





}
