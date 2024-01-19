#include <iostream>
#include <vector>
#include <numeric>

static long findMissing(std::vector<long> seq) {    
    const size_t n = seq.size();
    long fst = seq[0], nxt = seq[n - 1]; 
    long total; 
  
    if ((fst + nxt) % 2 == 0) { 
        total = (n + 1) * (fst + nxt) / 2; 
    } else { 
        total = (fst + nxt) * (n + 1) / 2; 
    } 
    
    long sum = std::accumulate (seq.begin(), seq.end(), 0);
  
    return total - sum; 
} 
  
int main () {

    std::cout << findMissing({-16,-25,-43});

}

