#include <vector>

unsigned long long solution(const std::vector<unsigned long long>& arr){

    unsigned long long i = arr.size(), val = arr.back();

    while (i--> 0)
        val = std::__gcd (val, arr[i]);

    return arr.size() * val;
}
