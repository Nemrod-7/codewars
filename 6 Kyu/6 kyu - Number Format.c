#include<string>

std::string numberFormat(long long n) {
  
    std::string num = std::to_string (n), res;
    int size = num.size(), index = 0;

    while (size-->0) {
        res += num[index++];
        
        if (size != 0 && num[index - 1] != '-' && size % 3 == 0) res += ',';
    }
  
    return res;
}
