#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main () {


}

std::string range_extraction(std::vector<int> args){

    int minval = args[args.size()], maxval = args[0], cnt = 0;
    ostringstream os;

    for (int i = 0; i < args.size(); ++i) {
        minval = min (minval, args[i]), maxval = max (maxval, args[i]);
        cnt++;
        if (args[i] + 1 != args[i + 1]) {

            if (cnt < 2)       os << minval;
            else if (cnt == 2) os << minval << "," << maxval;
            else               os << minval << "-" << maxval;

            if (i + 1 < args.size())     os << ",";

            minval = args[i + 1], cnt = 0;
        }
    }
    return os.str();
}
