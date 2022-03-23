
#include <string>
#include <vector>
#include <algorithm>

std::string rtrim(const std::string &s) {
    size_t end = s.find_last_not_of(" ");
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
std::string histogram (std::vector<int> data) {

    const int size = data.size(), top = *max_element(begin(data), end(data));
    int idx = top > 0 ? top + 1 : 0;
    std::string os;

    while (idx-->0) {
        std::string line (11,' '), dig;

        for (int i = 0, pos = 0; i < size; i++, pos += 2) {
            if (data[i] == idx && data[i] != 0) {
                dig = std::to_string (idx);
                line.replace (pos, dig.size(), dig);
            } else if (data[i] > idx) {
                line[pos] = '#';
            }
        }

        line = rtrim (line);
        os += line + "\n";
    }

    os += std::string (data.size() * 2 - 1, '-') + '\n';

    for (int i = 0; i < size; i++) {
        os += std::to_string (i + 1);
        if (i < size - 1) os += " ";
    }

    return os + '\n';
}
