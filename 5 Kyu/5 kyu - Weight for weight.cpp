#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

class WeightSort {
    private :
        static bool comp (const pair<int,string> &a, const pair<int,string> &b) {

            if (a.first == b.first) return a.second < b.second;          
            return a.first < b.first;
        }
    public :
        static string orderWeight (const string &src) {
          
            string num, os;
            stringstream iss (src);
            vector<pair<int,string>> base;

            while (iss >> num) {
                int weight = accumulate (num.begin(), num.end(), 0,
                    [](int s, char c) { return s + c - '0'; });

                base.push_back ({weight, num});
            }

            if (!base.size()) return "";
            sort (base.begin(), base.end(), comp);

            for (size_t i = 0; i < base.size(); i++) {
                os += base[i].second;
                if (i < base.size() - 1) os += " ";
            }

            return os;
        }
};
