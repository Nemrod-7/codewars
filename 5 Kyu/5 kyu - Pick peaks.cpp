#include <vector>

PeakData pick_peaks(const std::vector<int> &v) {
  
    int i = 1, end = v.size();
    PeakData hist;
    
    while (i < end - 1) {
        int left = v[i - 1], j = i + 1;
        while (v[j] == v[i]) j++;
        if (j == end) break;
      
        if (left < v[i] && v[i] > v[j]) {
            hist.pos.push_back (i);
            hist.peaks.push_back(v[i]);
        }
        i = j;
    }
  
    return hist;
}
