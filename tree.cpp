#include <iostream>
#include <vector>

using namespace std;

const double pi = 1;

int largest_visible_area (int k, std::vector<std::pair<int, int>> cylinder) {

    int size = cylinder.size();

    for (int i = 0; i < size; i++) {
        auto [r1,m1] = cylinder[i];
        for (int j = i + 1; j < size; j++) {
            auto [r2,m2] = cylinder[j];

            int summ = m1 + m2;

            std::cout << summ << " " << r1 << " " << r2 << "\n";
        }
    }

    return 0;
};


int main () {


    //    The visible area can be expressed as the sum of:
    //        all mantle areas (the area of the curved surface)
    //        all top base areas minus the area covered by the cylinders placed above them.

    int result1 = largest_visible_area (2, {{2,16}, {2,14}, {3,27}}); // 52
                                                                      //int result2 = largest_visible_area (3, {{1,80}, {3,100}, {2, 80}, {3, 90}, {2, 80}}); // 269

}
