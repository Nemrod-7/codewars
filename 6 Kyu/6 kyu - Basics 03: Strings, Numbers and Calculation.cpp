#include <string>
#include <cmath>
using namespace std;

bool isoperator (char c) {
    return c == '+' || c == '-' || c == '/' || c == '*';
}
std::string calculateString(std::string calcIt) {
    string result, first, buffer;
    char op;
    cout << calcIt << endl;
    for (auto it : calcIt) {
        if (isdigit (it) || it == '.')
            buffer.push_back(it);

        if (isoperator (it)) {
            op = it;
            first = buffer;
            buffer.clear();
        }
    }
    double num =  (stod(first)), den = (stod(buffer));

    if (op == '+') result = to_string( (int)(round (num + den)) );
    if (op == '-') result = to_string( (int)(round (num - den)) );
    if (op == '/') result = to_string( (int)(round (num / den)) );
    if (op == '*') result = to_string( (int)(round (num * den)) );

    return result;
}
