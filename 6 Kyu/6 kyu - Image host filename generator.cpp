#include <iostream>
#include <chrono>
#include <random>

using namespace std;

class passwd {
    public :
        static string generate() {
            string os;
            mt19937 rng (random_device {}()); // generate entropy pool => does not deplete the system entropy
            uniform_int_distribution<int> length (8, 20), distribution (57, 122);
            size_t size = length (rng);

            while (size-->0) os += static_cast<char> (distribution (rng));

            return os;
        }
};

int main () {

    int index = 50;
    //auto rnd = [](int lim) -> int {return rd() % lim;};
    while (index-->0) {
        cout << passwd::generate() << endl;
    }

}
