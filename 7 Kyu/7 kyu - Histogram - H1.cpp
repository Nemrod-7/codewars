#include <string>
#include <vector>

std::string histogram (std::vector<int> data) {

    int idx = data.size();
    std::string line, os;

    while (idx-->0) {
        int dig = data[idx];
        line = std::to_string (idx + 1) + '|';
        if (dig != 0)
            line += std::string (dig, '#') + ' ' + std::to_string (dig);

        os += line + '\n';
    }

    return os;
}
