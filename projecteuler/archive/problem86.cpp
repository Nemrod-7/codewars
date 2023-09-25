#include <iostream>
#include <cmath>

using namespace std;

int main () {

    int dim = 0;
    int cnt = 0;

    int x = 1;

    while (x++) {
        for (int y = 1; y <= x; y++) {
            for (int z = 1; z <= y; z++) {
                double dist = hypot (x, y + z);

                if (abs (dist - round(dist)) < 1e-8) {
                    // std::cout << x << " " << y << " " << z << " :: " << dist << "\n";
                    cnt++;
                    if (cnt > 1000000) {

                        std::cout << "M:: " << x << " cnt: " << ;
                        return 1;
                    }
                }
            }
        }
    }

}
