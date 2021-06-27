#include <iostream>

namespace process {
    int getkey (int num, int key) {
        int res = -1;

        for (int i = 0; i < 26; i++)
            if (i * key % 26 == num)
                res = res >= 0 ? -1 : i;

        return res;
    }
    std::string decode (std::string src) {
        std::string::size_type size;
        int dig, key = std::stoll (src, &size);
        std::string os, code = src.substr (size);

        for (auto &c : code) {
            dig = getkey (c - 'a', key);
            if (dig == -1) return "Impossible to decode";
            c = dig + 'a';
        }

        return code;
    }
};

int main () {

    std::cout << process::decode ("6015ekx");
}
