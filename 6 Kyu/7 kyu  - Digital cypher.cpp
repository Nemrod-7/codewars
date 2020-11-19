#include <iostream>

class Kata {
    public:
        static std::vector<int> Encode(std::string str, int n) {
            std::vector<int> code;
            std::string key = std::to_string(n);
            size_t next = 0;

            for (auto &c : str) {
                if (next == key.size()) next = 0;
                code.push_back ((c - 'a' + 1) + (key[next++] - '0'));
            }

            return code;
        }
};
