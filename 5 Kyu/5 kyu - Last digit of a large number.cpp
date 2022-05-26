#include <string>
#include <vector>

int getnum (const std::string &str) {
    return (str.size() < 4) ? stoi (str) : stoi (str.substr(str.size() - 4));
}
int last_digit (const std::string &str1, const std::string &str2) {

    if (str2 == "0") return 1;
    if (str1 == "1") return 1;
    int a = getnum (str1), b = getnum (str2);
    std::vector<std::vector<int>> base {{0},{1},{6,2,4,8},{1,3,9,7},{6,4},{5},{6},{1,7,9,3},{6,8,4,2},{1,9}};

    int mod = base[a % 10].size();
    return base[a % 10][b % mod];
}
