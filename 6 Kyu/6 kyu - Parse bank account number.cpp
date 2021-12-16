#include <iostream>
#include <vector>

using namespace std;
const vector<vector<string>> digits {
{" _ ", "| |", "|_|"},{"   ", "  |", "  |"},{" _ ", " _|", "|_ "},
{" _ ", " _|", " _|"},{"   ", "|_|", "  |"},{" _ ", "|_ ", " _|"},
{" _ ", "|_ ", "|_|"},{" _ ", "  |", "  |"},{" _ ", "|_|", "|_|"},
{" _ ", "|_|", " _|"}};

long parse_bank_account(const std::string &src) {
    int size = src.size() / 3;
    long number = 0;

    for (int i = 0; i < size - 3; i += 3) {
        vector<string> curr (3);
        for (int y = 0; y < 3; y++) {
            for (int x = i; x < i + 3; x++) {
                curr[y] += src[y * size + x];
            }
        }

        for (int j = 0; j < 10; j++) {
            if (curr == digits[j])
                number = number * 10 + j;
        }
    }
    return number;
}

int main () {
    const string src =
    " _  _  _  _  _  _  _  _  _ \n"
    "|_| _| _||_||_ |_ |_||_||_|\n"
    "|_||_  _||_| _||_| _||_| _|\n";



    return 0;
}
