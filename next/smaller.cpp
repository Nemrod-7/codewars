#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include <random>
#include <chrono>

using namespace std;

std::vector<int> smaller(const std::vector<int> &arr) {
    std::vector<int> res (arr.size());
    std::map<int,int> sweep;
    int index = arr.size();

    while (index-->0) {
        int dig = arr[index], cnt = 0;

        for (auto &[num, freq] : sweep) {
            if (num < dig) {
                cnt += freq;
            } else {
                break;
            }
        }

        res[index] = cnt;
        sweep[dig]++;
    }

    return res;
}


int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now(), end;
    chrono::duration<double> elapsed;

    // const int size = 15000;
    // vector<int> arr;
    // random_device rd;
    // mt19937 gen(rd());
    // uniform_int_distribution<> rng (-1200, 1200);
    //
    // for (int i = 0; i < size; i++) {
    //     arr.push_back(rng(gen));
    // }
    //
    // smaller(arr);

    vector<int> arr = {5, 4, 7, 9, 2, 4, 1, 4, 5, 6};
    vector<int> sed = arr;
    map<int,int> sweep;

    sort(sed.begin(), sed.end());

    for (int i = 0; i < arr.size(); i++) {
    // for (int i = arr.size() - 1; i >= 0; i--) {
        sweep[sed[i]] = i;
    }

    for (int i = 0; i < arr.size(); i++) {
        // cout << sweep[arr[i]] << ' ';
    }

    end = chrono::steady_clock::now (), elapsed = end - alpha;
    std::cout << "\nDuration " << fixed << elapsed.count() << " ms" << std::endl;
}
