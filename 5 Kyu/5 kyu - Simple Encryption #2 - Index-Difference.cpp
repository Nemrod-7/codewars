#include <iostream>
#include <string>

using namespace std;

const string region = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,:;-?! '()$%&\""};

class Kata {
    private :

        char alternate (char c) { return isupper(c) ? tolower(c) : toupper(c);}

    public :
        string encrypt(string input) {
            //if (!is_valid (input)) throw exception ();
            string os = input;
            int val, idx = region.find(input[0]);

            for (size_t i = 1; i < input.size() ;++i) {
                if (region.find(input[0]) == string::npos) throw exception ();
                if (i % 2) input[i] = alternate (input[i]);

                val = region.find(input[i - 1]) - region.find(input[i]);
                idx = val >= 0 ? val : val + 77;

                os[i] = region[idx];
            }
            os[0] = region[76 - region.find(input[0])];

            return os;
        }
        string decrypt(string input) {
            //if (!is_valid (input)) throw exception ();
            input[0] = region[76 - region.find(input[0])];

            string os = input ;
            int val, idx = region.find(input[0]);

            for (size_t i = 1; i < input.size(); ++i) {
                if (region.find(input[0]) == string::npos) throw exception ();

                val = (77 - region.find(input[i])) + region.find(input[i - 1]);
                idx = val > 76 ? val - 77 : val;

                input[i] = region[idx];

                os[i] = (i % 2) ? alternate (input[i]) : input[i];

            }

            return os;

        }
};

int main () {


}
