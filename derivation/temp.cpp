#include <iostream>
#include <vector>
#include <regex>

using namespace std;

void show (const std::vector<std::string> &vs) {

    for (int i = 0; i < vs.size(); i++) {
        std::cout << "[" << vs[i] << "]";
    }

    cout << endl;
}

vector<string> tokenize(const string &input) {

    const regex tokn ("(\\(-)?foo|[*-/^]");
    sregex_token_iterator iter (input.begin (), input.end (), tokn);
    return vector<string> (iter, sregex_token_iterator ());
}

int main () {

    string expr = "6-foo";



    cout << "\nend\n";
}
