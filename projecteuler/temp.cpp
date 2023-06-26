#include <iostream>
#include <vector>

#include <chrono>

using namespace std;

int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now (), end;
    chrono::duration<double> elapsed;

    for (int i = 1; i < 18; i++) {




    }


    end = chrono::steady_clock::now (), elapsed = end - alpha;
    std::cout << "\nDuration " <<fixed<< elapsed.count() << " ms" << std::endl;
}
