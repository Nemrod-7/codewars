#include <iostream>

#include <stdio.h>
#include <cstdint>
#include <string>

uint32_t ip_to_int32 (const std::string& ip) {

    uint32_t add[4], num = 0;
    sscanf (ip.c_str(), "%d.%d.%d.%d", &add[0], &add[1], &add[2], &add[3]);

    for (int i = 0; i < 4; i++) {
        num <<= 8;
        num |= static_cast<uint8_t>(add[i]);
    }

    return num;
}

int main () {

    //std::cout << ip_to_int32("128.32.10.1"); // 2149583361
    uint32_t num = 4294967295;
    for (int i = 0; i < 4; i++) {
        std::cout << num % 256;
        num >>= 8;
    }

}
