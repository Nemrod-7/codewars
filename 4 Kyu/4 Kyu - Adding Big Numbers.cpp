#include <iostream>
#include <string>
#include <algorithm>

#define TO_DIG(x) ((x) - '0')
#define TO_CHAR(x) ((x) + '0')

std::string add (std::string a, std::string b) {

    std::string output;
    int i = a.length(), j = b.length(), index = max(i,j);
    int rem = 0, digit, num;
    
    while (index-->0) {
        num = (i > 0 ? TO_DIG (a[--i]) : 0) + (j > 0 ? TO_DIG (b[--j]) : 0) + rem;
        rem = num / 10;
        output += TO_CHAR (num % 10);
    }

    if (rem > 0) output += TO_CHAR (rem);

    reverse (output.begin(), output.end());
    output.erase(0,  std::min (output.find_first_not_of('0'), output.size()-1));

    return output;
}
