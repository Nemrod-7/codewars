#include <iostream>

using namespace std;

int train_crash (string track, const string &a_train, int a_train_pos, const string &b_train, int b_train_pos, int limit) {

    track[a_train_pos] = 'A';
    track[b_train_pos] = 'B';
    cout  << track;

    return 42;
}

int main () {

const std::string example_track =
         "                                /------------\\\n"
         "/-------------\\                /             |\n"
         "|             |               /              S\n"
         "|             |              /               |\n"
         "|        /----+--------------+------\\        |   \n"
         "\\       /     |              |      |        |     \n"
         " \\      |     \\              |      |        |                    \n"
         " |      |      \\-------------+------+--------+---\\\n"
         " |      |                    |      |        |   |\n"
         " \\------+--------------------+------/        /   |\n"
         "        |                    |              /    | \n"
         "        \\------S-------------+-------------/     |\n"
         "                             |                   |\n"
         "/-------------\\              |                   |\n"
         "|             |              |             /-----+----\\\n"
         "|             |              |             |     |     \\\n"
         "\\-------------+--------------+-----S-------+-----/      \\\n"
         "              |              |             |             \\\n"
         "              |              |             |             |\n"
         "              |              \\-------------+-------------/\n"
         "              |                            |               \n"
         "              \\----------------------------/ \n";

     train_crash(example_track, "Aaaa", 147, "Bbbbbbbbbbb", 288, 1000);

}
