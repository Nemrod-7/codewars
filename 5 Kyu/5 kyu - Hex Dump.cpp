#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;

std::string hexdump(const std::vector<char> &data){

    ostringstream os;
    string txt;

    size_t i, addr = 0;
    os << hex << setfill('0');

    for (i = 0; i < data.size(); i++) {
        int dig = static_cast<unsigned char> (data[i]);

        if (i % 16 == 0) {
            if (i != 0) {
                os << " " << txt << "\n";
            }

            os << setw (8) << addr << ": ";
            addr += 16;
            txt.clear();
        }
        os << setw(2) << dig << " ";
        txt += isprint (data[i]) ? data[i] :  '.';
    }

    while (i % 16 != 0) {
        os << "   ";
        i++;
    }
    os << ' ' << txt;

    return os.str();
};


std::vector<char> dehex(const std::string &text){
    vector<char> out;
    string line;
    stringstream iss (text);

    while (getline (iss, line)) {
        int fst = line.find (" "), scd = line.find("  ");

        for (; fst != scd; fst += 3) {
            string hxval = line.substr (fst + 1, 2);
            int num = stoi (hxval, nullptr, 16);
            out.push_back(num);
        }
    }

    return out;
};
