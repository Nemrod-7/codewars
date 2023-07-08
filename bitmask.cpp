#include <iostream>
#include <vector>

void showbit (int byte) {
    for (int i = 0; i < 8; i++) {

        std::cout << (byte >> i&1);
    }
    std::cout << "\n";
}

int main () {


    int n = 300;
    std::vector<int> swit {0, 2, 500, 3000};//156, 264, 451};

    int n2 = (n >> 6) + 1;
    std::vector<int64_t> mask(n2);
    std::cout << "size " << n2 << "\n";

    for (auto pos : swit) {
        std::cout << (pos >> 6) << " " << (pos &63) << "\n";
        mask[pos >> 6] |= 1ULL << (pos &63);
    } 

 //   for (int64_t i = 0; i < mask.size(); i++) {
 //       for (int j = 0; j < 64; j++) {
 //           if (mask[i] >> j&1ULL) {
 //               //std::cout << i * 64 + j << " ";
 //           }
 //       } 
 //   }
    //std::cout << (n / 64) << " " << (n >> 6);

    //showbit(mask);
    //showbit(bset);
    //showbit(bset ^ mask);





}
