#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

string sum_strings(string a, string b);

int main () {


    return 0;
}

std::string sum_strings(std::string a, std::string b) {
    int lena = a.size(), lenb = b.size(), size = max(lena,lenb) + 2 ;
    int rem = 0, num;
    std::string sum;

    while (size-->0) {
        num = (lena ? a[--lena] - '0' : 0) + (lenb ? b[--lenb] - '0' : 0) + rem;
        rem = num / 10;
        sum.push_back(num % 10 + '0');
    }
    sum.push_back('0');

    while (sum.size() > 1 && sum.back() == '0')
        sum.pop_back();

    std::reverse(sum.begin(),sum.end());

    return sum;
}
