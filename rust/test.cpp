#include <iostream>

namespace Assert {

    enum {equal, noteq };

    template<class T> 
        struct result {
            int type;
            T x;
        };

    template<class T> 
        void That(const T &actual, const result<T> &expected, const std::string &msg = "") {
            bool success = true;

            if (expected.type == equal && actual != expected.x) {
                success = false;
            } else if (expected.type == noteq && actual == expected.x) {
                success = false;
            }

            if (success == false) {
                if (msg == "") {
                    std::cout << "\nactual   : " << actual;
                    std::cout << "\nexpected : " << expected.x;
                }
            }
        }
}

std::string ExtraMessage(const char *src) { return src; }
template<class T> Assert::result<T> Equals (const T &x) { return { Assert::equal, x }; }

int tower (int a, int b, int x) { return 0; }

int main () {

    Assert::That(3, Equals(3));
    Assert::That(tower(729, 0, 1), Equals(0), ExtraMessage(" Incorrect answer for base=729 exp=0 mod=1"));

}

