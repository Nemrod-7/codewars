#include <iostream>
#include <queue>
#include <map>
#include <numeric>
#include <limits>
#include <chrono>
// Can you control the lights?
using namespace std;
using vertex = pair<int,vector<uint64_t>>;
/*
  switch 0 [0, 1, 2],
  switch 1 [1, 2],
  switch 2 [1, 2, 3, 4],
  switch 3 [1, 4]

L0 = S0 ^ S2
L1 = S0 ^ S1 ^ S2 ^ S3
L2 = S0 ^ S1 ^ S2
L3 = S3
L4 = S3 ^ S4
S0 ^ S2 & S0 ^ S1 ^ S2 ^ S3 & S0 ^ S1 ^ S2 & S3 & S3 ^ S4
S3 = true
S0 ^ S2 & !(S0 ^ S1 ^ S2) & S0 ^ S1 ^ S2 & !S4
... This is SAT solving
https://en.wikipedia.org/wiki/Boolean_satisfiability_problem#XOR-satisfiability
*/
pair<int,vector<vector<int>>> test1();

void showbit (vector<uint64_t> num, int n) {
    int size = (n >> 6) + 1;
    for (int j = 0; j < size; j++) {
        for (int k = 0; k < n; k++) {
            bool bit = num[j] >> k & 1ULL;
            cout << bit << ' ';
        }
    }
}
void show (priority_queue<vertex> q1) {
    while (!q1.empty()) {
        auto [dist,state] = q1.top();
        q1.pop();
        cout << dist << " ";
    }
}
static inline uint64_t cntbits (uint64_t i) {
    i = i - ((i >> 1) & 0x5555555555555555);
    i = (i & 0x3333333333333333) + ((i >> 2) & 0x3333333333333333);
    return (((i + (i >> 4)) & 0xF0F0F0F0F0F0F0F) * 0x101010101010101) >> 56;
}

bool check (int n, const vector<vector<int>> &swit) {

    vector<int> hist (n);
    for (int i = 0; i < swit.size(); i++) {

        cout << swit[i].size() << " ";
        for (int j = 0; j < swit[i].size(); j++) {
            int num = swit[i][j];
            if (num >= n) {
                cout << "exceeding : " << num << '\n';
                return false;
            }
            hist[num]++;
        }
    }

    for (int i = 0; i < n; i++) {
        //cout << hist[i] << ' ';
    }

    return true;
}
vector<uint64_t> mark (int n) {
    int size = (n >> 6) + 1;
    vector<uint64_t> cell (size);

    for (uint64_t i = 0; i < n; i++) {
        cell[i >> 6] |= 1ULL << (i &63);
    }
    return cell;
}

bool light_switch1 (int n, const vector<vector<int>> &swit) { // generic disjktra algo simple bitmask 0.12

    queue<vector<uint64_t>> q1;
    map<vector<uint64_t>,bool> visit;
    const vector<uint64_t> exit = mark (n);

    q1.push({0});

    while (!q1.empty()) {
        vector<uint64_t> state = q1.front();
        q1.pop();

        if (state == exit)
            return true;

        for (auto &comb : swit) {
            vector<uint64_t> next = state;
            for (auto &num : comb) {
                next[num >> 6] ^= 1ULL << (num &63);
            }

            if (visit[next] == false) {
                q1.push(next);
                visit[next] = true;
            }
        }
    }

    return false;
}
bool light_switch2 (int n, const vector<vector<int>> &swit) { // 0.16 simple astar algorithm bitmasking // heuristic == number of set bits

    priority_queue<vertex> q1; // priority => highest number of set bits / light switches
    map<vector<uint64_t>,bool> visit;
    vector<uint64_t> exit = mark (n);
    vector<uint64_t> start ((n >> 6) + 1);

    q1.push({0, start});

    while (!q1.empty()) {
        auto [dist,state] = q1.top();
        q1.pop();

        if (state == exit) {
            return true;
        }

        for (auto &comb : swit) {
            vector<uint64_t> next = state;
            for (auto &num : comb) {
                next[num >> 6] ^= 1ULL << (num &63);
            }

            if (visit[next] == false) {
                int cnt = 0;  // count the number of set bits

                for (auto &num : next) {
                    for (int i = 0; i < 64; i++) {
                        if ((num >> i &1ULL) == true) {
                            cnt++;
                        }
                    }
                }

                q1.push({cnt,next});
                visit[next] = true;
            }
        }
    }

    return false;
}

vector<vector<uint64_t>> construct (int n, vector<vector<int>> swit) {
    const vector<uint64_t> base ((n >> 6) + 1);
    vector<vector<uint64_t>> mask (swit.size());

    for (int i = 0; i < swit.size(); i++) {
        mask[i] = base;
        for (auto &num : swit[i]) {
            mask[i][num >> 6] ^= 1ULL << (num &63);
        }
    }

    return mask;
}
bool light_switch3 (int n, const vector<vector<int>> &swit) { // 0.16

    const int size = (n >> 6) + 1;
    priority_queue<vertex> q1;
    map<vector<uint64_t>,bool> visit;
    const vector<uint64_t> exit = mark (n);
    const vector<uint64_t> start ((n >> 6) + 1);
    const vector<vector<uint64_t>> mask = construct (n, swit);
    int cycle = 0;

    q1.push({0, start});

    while (!q1.empty()) {
        auto [dist,curr] = q1.top();
        q1.pop();

        cycle++;
        // showbit (curr,n);
        if (curr == exit) {
            cout << cycle << " cycle\n";
            return true;
        }
        // cout << q1.size() << ' ';
        for (auto &comb : mask) {
            vector<uint64_t> next = curr;
            int64_t cnt = 0;
            for (int j = 0; j < size; j++) {
                next[j] = curr[j] ^ comb[j];
                cnt += cntbits (next[j]);
            }

            if (visit[next] == false) {
                q1.push({cnt,next});
                visit[next] = true;
            }
        }
    }

    return false;
}

uint64_t maxSubsetXOR (vector<uint64_t> set) { // Function to return maximum XOR subset in set
    // Initialize index of chosen elements
    int64_t index = 0, res = 0;
    const int64_t n = set.size();

    for (int64_t i = 63; i >= 0; i--) {
        // Initialize index of maximum element and the maximum element
        int64_t maxInd = index, maxEle = numeric_limits<int64_t>::min();

        for (int64_t j = index; j < n; j++) {
            // If i'th bit of set[j] is set and set[j] is greater than max so far.
            if ( (set[j] & (1 << i)) != 0 && set[j] > maxEle )
                maxEle = set[j], maxInd = j;
        }
        // If there was no element with i'th bit set, move to smaller i
        if (maxEle == numeric_limits<int64_t>::min()) continue;
        // Put maximum element with i'th bit set at index 'index'
        swap(set[index], set[maxInd]);
        maxInd = index; // Update maxInd and increment index
        // Do XOR of set[maxIndex] with all numbers having i'th bit as set.
        for (int64_t j = 0; j < n; j++) {
            // XOR set[maxInd] those numbers which have the i'th bit set
            if (j != maxInd && (set[j] & (1 << i)) != 0)
                set[j] = set[j] ^ set[maxInd];
        }
        // Increment index of chosen elements
        index++;
    }

    // Final result is XOR of all elements
    for (uint64_t i = 0; i < n; i++)
        res ^= set[i];
    return res;
}

int main () {

    chrono::steady_clock::time_point start = chrono::steady_clock::now(), end;
    chrono::duration<double> elapsed;

    /*
       [0, 1, 2],    // switch 0
       [1, 2],       // switch 1
       [1, 2, 3, 4], // switch 2
       [1, 4]        // switch 3

       L0 = s0
       L1 = s0,s1,s2,s3
       L2 = s0,s1,s2
       L3 = s2
       L4 = s2,s3
       */

    auto [n, swit] = test1();
    light_switch2 (n,swit);


                                //   3       3        5        5
    //vector<vector<int>> sw = {{0,1,2}, {1,2}, {1,2,3,4}, {1,4}};
    //vector<vector<uint64_t>> bset = construct(n, sw);

    //cout << ((1ULL << n) - 1) << '\n';


    //res = light_switch1 (31,{{0,2,4,5,6,9,10},{1,3,5,6,7,8,11},{1,2,3,4,6,7,8,11},
    //        {2,4,9},{7,8,9,10},{1,4,8,11},
    //        {6,9},{8,9,10},{2,3,5,7,10,11},
    //        {12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30},
    //        {12,13,14,15,16,17,18,19},{21,22,23,24,25,26,27,28,29},
    //        {4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30},
    //        {6,7},{17,18,19,20,21,22,23,24,25,26,27,28}}); // true
    //cout << "result: " << res;


    // int size = (n >> 6) + 1;
    // int m = mask.size();
    // int64_t num = 0;
    //
    // num ^= mask[2][0]; // 01111
    // num ^= mask[0][0]; // 10011
    // num ^= mask[1][0]; // 11111
    //
    // for (int i = 0; i < mask.size(); i++) {
    //     vector<uint64_t> curr = mask[i];
    //     for (int j = i + 1; j < mask.size(); j++) {
    //         vector<uint64_t> next = mask[j];
    //         vector<uint64_t> x = next;
    //         for (int k = 0; k < size; k++) {
    //             x[k] = curr[k] ^ next[k];
    //         }
    //     }
    // }


    end = chrono::steady_clock::now(), elapsed = end - start;
    std::cout << "\nDuration : " << fixed << elapsed.count() << " ms\n";
    return 0;
}
///////////////////////////////////////////////////////////////////////////////
pair<int,vector<vector<int>>> test1() {
    return {100, {{1, 2, 6, 9, 11, 14, 16, 17, 18, 22, 23, 30, 33, 34, 35, 36, 39, 41, 46, 47, 48, 50, 51, 53, 58, 60, 61, 62, 64, 65, 68, 71, 73, 74, 75, 77, 78, 79, 82, 83, 86, 90, 91, 92, 93, 95, 96}, {0, 5, 6, 7, 11, 12, 14, 18, 19, 21, 22, 26, 30, 32, 34, 36, 42, 46, 48, 51, 52, 53, 55, 58, 59, 62, 63, 66, 67, 71, 73, 74, 75, 77, 78, 79, 83, 87, 89, 91, 92, 93, 95}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 47, 48, 49, 50, 51, 53, 54, 55, 56, 57, 58, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99}, {34, 43, 44, 45}, {1, 4, 5, 6, 7, 9, 11, 12, 16, 17, 18, 22, 23, 28, 31, 33, 35, 36, 37, 39, 40, 41, 44, 51, 52, 55, 56, 58, 59, 60, 61, 62, 64, 68, 69, 72, 73, 74, 76, 77, 78, 79, 81, 82, 85, 87, 91, 92, 93, 94, 95, 98, 99}, {0, 1, 4, 5, 7, 9, 10, 11, 12, 14, 15, 17, 18, 21, 22, 23, 24, 26, 28, 29, 30, 31, 32, 34, 35, 39, 40, 41, 42, 44, 45, 48, 51, 52, 54, 55, 57, 58, 59, 60, 62, 65, 68, 69, 71, 74, 75, 76, 77, 81, 84, 86, 87, 91, 93, 94, 96, 98, 99}, {16, 48, 58, 64, 70, 85, 96}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 11, 13, 14, 15, 16, 17, 19, 20, 21, 23, 24, 25, 26, 27, 28, 29, 33, 34, 35, 38, 40, 42, 44, 45, 48, 49, 50, 51, 53, 55, 57, 59, 60, 63, 64, 65, 66, 68, 71, 74, 75, 76, 80, 81, 82, 83, 84, 85, 88, 89, 90, 93, 94, 95, 96, 97, 99}, {0, 1, 2, 4, 5, 7, 8, 9, 14, 16, 18, 19, 22, 24, 25, 27, 28, 30, 31, 33, 36, 37, 40, 44, 45, 46, 47, 50, 53, 56, 60, 62, 63, 64, 65, 66, 67, 68, 70, 73, 74, 75, 77, 79, 82, 84, 88, 89, 93, 94, 95, 96, 97, 98, 99}, {8}, {3, 6, 13, 18, 23, 45, 50, 71, 96}, {4, 5, 7, 12, 13, 18, 19, 20, 22, 23, 25, 28, 29, 31, 32, 35, 36, 37, 39, 42, 43, 44, 51, 52, 53, 55, 56, 57, 61, 62, 64, 65, 67, 68, 69, 71, 72, 74, 75, 76, 77, 80, 84, 85, 86, 87, 88, 90, 95, 98, 99}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 55, 56, 57, 58, 59, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99}, {7, 9, 14, 15, 20, 22, 23, 29, 40, 42, 44, 45, 46, 51, 57, 62, 64, 65, 67, 68, 72, 73, 76, 83, 85, 86, 89, 92, 94}, {3, 4, 5, 6, 8, 10, 11, 15, 20, 21, 22, 24, 25, 28, 29, 31, 34, 38, 39, 40, 42, 43, 46, 48, 50, 51, 53, 54, 57, 59, 61, 62, 63, 65, 67, 69, 70, 72, 74, 76, 78, 79, 82, 83, 84, 85, 86, 87, 89, 92, 94, 95, 97, 98}, {3, 5, 7, 8, 9, 13, 14, 19, 23, 25, 30, 32, 33, 38, 41, 42, 44, 45, 46, 48, 52, 59, 61, 62, 64, 66, 68, 69, 77, 80, 82, 89, 90, 94, 95, 96, 97, 99}, {4, 7, 15, 17, 35, 39, 46, 52, 57, 59, 67, 77, 83, 85, 87, 89}, {1, 6, 9, 19, 20, 21, 23, 25, 28, 33, 44, 49, 50, 53, 54, 56, 57, 59, 60, 63, 67, 72, 73, 82, 86, 87, 89, 92, 93, 94, 97, 98, 99}, {0, 1, 2, 4, 5, 6, 7, 9, 11, 12, 14, 15, 16, 17, 19, 20, 22, 24, 25, 27, 28, 32, 35, 36, 38, 39, 40, 42, 43, 45, 46, 48, 49, 50, 51, 52, 53, 56, 57, 58, 59, 61, 62, 64, 66, 67, 68, 69, 70, 72, 74, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 87, 88, 89, 90, 91, 92, 94, 95, 96, 97}, {0, 2, 8, 9, 12, 13, 14, 19, 22, 24, 29, 31, 32, 34, 35, 36, 38, 39, 40, 41, 42, 43, 44, 48, 49, 50, 51, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 66, 68, 69, 71, 73, 75, 77, 78, 80, 81, 83, 84, 87, 88, 91, 92, 93, 94, 95, 98}, {1, 5, 7, 28, 40, 54, 55, 76, 80, 82, 86, 87, 88, 95, 98}, {3, 5, 6, 7, 11, 12, 14, 18, 19, 22, 25, 27, 29, 31, 32, 34, 38, 40, 42, 44, 45, 46, 47, 48, 49, 53, 55, 61, 63, 64, 65, 67, 70, 71, 72, 73, 81, 84, 88, 91, 92, 93, 94, 95, 96, 98}, {0, 1, 2, 4, 5, 7, 8, 9, 10, 12, 13, 14, 15, 17, 18, 22, 24, 28, 29, 32, 33, 35, 36, 37, 38, 39, 41, 42, 46, 47, 48, 50, 54, 55, 59, 60, 61, 62, 64, 65, 66, 67, 71, 74, 77, 78, 79, 82, 83, 84, 87, 89, 91, 93, 95, 98, 99}, {1, 2, 3, 4, 6, 7, 8, 10, 11, 12, 13, 14, 15, 16, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 40, 41, 42, 43, 44, 45, 46, 47, 48, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99}, {2, 47, 48, 49, 59, 63, 94, 99}, {0, 18, 26, 27, 32, 34, 38, 44, 50, 53, 54, 61, 62, 70, 72, 74, 75, 82, 89, 95}, {0, 2, 3, 4, 5, 6, 8, 10, 11, 13, 14, 16, 17, 21, 22, 24, 27, 28, 29, 30, 32, 33, 34, 35, 36, 37, 39, 41, 43, 44, 46, 49, 53, 56, 58, 62, 63, 66, 69, 75, 79, 85, 86, 87, 92, 93, 94, 95, 99}, {5, 13, 14, 15, 17, 19, 22, 25, 27, 44, 48, 51, 53, 54, 56, 57, 58, 62, 66, 67, 68, 75, 78, 80, 81, 89, 90, 91, 95, 97, 99}, {1, 3, 4, 5, 6, 12, 14, 16, 18, 19, 21, 24, 28, 30, 31, 33, 36, 40, 43, 44, 51, 55, 59, 64, 68, 69, 70, 71, 72, 74, 75, 79, 80, 83, 87, 90, 92, 94, 95, 99}, {4, 16, 18, 21, 24, 27, 30, 33, 34, 39, 40, 42, 43, 46, 47, 49, 53, 59, 68, 69, 71, 75, 77, 78, 80, 88, 90, 92, 94, 99}, {2, 22, 24, 30, 36, 40, 50, 53, 59, 61, 76, 91, 92, 97}, {19, 28, 40, 48, 57, 59, 67, 79, 86, 90, 94, 98}, {0, 1, 4, 8, 9, 11, 12, 15, 16, 17, 18, 19, 21, 22, 23, 26, 28, 33, 34, 35, 36, 37, 42, 44, 48, 50, 53, 55, 56, 57, 58, 59, 60, 62, 63, 64, 68, 70, 71, 72, 73, 76, 77, 79, 84, 88, 89, 91, 92, 93, 94, 95, 98, 99}, {0, 1, 2, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 41, 43, 44, 45, 46, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99}, {5, 10, 12, 15, 17, 20, 22, 24, 25, 28, 29, 32, 33, 39, 40, 41, 42, 49, 51, 63, 67, 68, 71, 73, 76, 81, 83, 85, 88, 97, 99}, {1, 3, 9, 10, 12, 22, 28, 34, 40, 44, 46, 50, 51, 54, 61, 64, 68, 73, 75, 79, 80, 82, 85, 90, 94, 98}, {6, 8, 10, 17, 20, 22, 27, 29, 31, 32, 36, 39, 41, 43, 47, 50, 55, 57, 61, 65, 72, 78, 79, 80, 82, 83, 84, 86, 91, 94, 96, 98}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 19, 20, 21, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 38, 39, 41, 42, 43, 44, 45, 46, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 89, 90, 91, 92, 94, 95, 97, 98, 99}, {1, 8, 9, 12, 17, 24, 27, 35, 38, 45, 49, 60, 62, 70, 72, 74, 75, 80, 82, 84, 88, 90, 94}, {0, 3, 5, 6, 9, 12, 13, 14, 17, 20, 21, 27, 29, 31, 32, 36, 40, 44, 50, 51, 55, 58, 59, 60, 65, 66, 67, 72, 82, 86, 89, 91, 93, 96, 97, 99}, {1, 5, 7, 10, 20, 21, 22, 23, 27, 28, 29, 30, 31, 33, 34, 35, 37, 38, 45, 46, 52, 53, 54, 59, 64, 65, 67, 68, 69, 70, 71, 73, 74, 77, 80, 81, 85, 87, 89, 92, 93, 98}, {0, 2, 3, 4, 5, 6, 8, 10, 11, 15, 16, 17, 18, 20, 21, 22, 25, 26, 27, 31, 32, 33, 34, 35, 36, 37, 38, 40, 41, 42, 44, 46, 47, 48, 49, 50, 51, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 69, 70, 71, 72, 73, 74, 78, 79, 82, 83, 84, 86, 89, 90, 91, 93, 94, 95, 96, 97, 99}, {1, 11, 20, 26, 42}, {1, 2, 5, 6, 8, 10, 11, 12, 13, 14, 15, 17, 18, 19, 20, 21, 22, 24, 29, 31, 33, 37, 40, 43, 44, 45, 48, 51, 52, 53, 55, 57, 58, 59, 60, 61, 62, 63, 65, 70, 72, 73, 76, 77, 79, 80, 81, 82, 84, 85, 86, 87, 88, 90, 93, 95, 96, 99}, {0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 23, 24, 25, 28, 29, 30, 31, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 92, 93, 94, 95, 97, 98, 99}, {1, 3, 4, 5, 6, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 27, 28, 29, 31, 32, 34, 35, 36, 38, 39, 40, 42, 43, 44, 45, 48, 50, 53, 55, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 70, 71, 72, 73, 75, 77, 78, 80, 81, 83, 85, 86, 88, 89, 90, 92, 93, 94, 96, 98, 99}, {0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 15, 16, 18, 19, 21, 22, 23, 24, 25, 26, 27, 29, 30, 31, 32, 33, 34, 35, 36, 40, 42, 44, 45, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 78, 79, 80, 81, 82, 84, 85, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97}, {2, 3, 7, 9, 10, 11, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 29, 30, 31, 32, 33, 34, 35, 37, 39, 42, 43, 48, 52, 58, 59, 61, 62, 63, 64, 65, 67, 68, 72, 77, 78, 80, 81, 87, 88, 89, 91, 94, 95, 97, 98}, {0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99}, {0, 11, 13, 14, 25, 40, 47, 54, 70, 72, 75, 76, 77, 79, 90, 92, 98}, {1, 4, 5, 7, 12, 16, 17, 18, 20, 24, 25, 28, 33, 34, 41, 45, 47, 49, 50, 53, 54, 61, 64, 65, 71, 73, 75, 79, 81, 84, 90, 93, 96, 99}, {5, 8, 9, 11, 13, 16, 19, 22, 29, 31, 33, 36, 37, 43, 47, 48, 56, 66, 69, 70, 73, 74, 83, 85, 88, 89, 91, 92, 98, 99}, {0, 2, 3, 4, 7, 9, 10, 15, 16, 18, 19, 20, 22, 24, 25, 26, 27, 28, 30, 32, 33, 34, 35, 36, 39, 41, 45, 47, 48, 50, 53, 56, 57, 60, 62, 63, 64, 67, 68, 69, 71, 72, 74, 75, 76, 77, 78, 79, 80, 82, 83, 84, 86, 87, 88, 90, 91, 95, 97, 98}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 19, 20, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 42, 44, 45, 46, 47, 48, 49, 50, 51, 52, 54, 55, 56, 57, 59, 60, 61, 64, 65, 66, 67, 70, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 84, 86, 87, 88, 89, 90, 91, 93, 95, 96, 97, 98, 99}, {0, 9, 14, 32, 33, 36, 39, 56, 62, 73, 77, 78}, {0, 1, 6, 7, 8, 11, 12, 13, 14, 15, 16, 17, 18, 21, 22, 23, 24, 26, 27, 28, 29, 30, 31, 33, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 47, 48, 49, 50, 51, 55, 57, 59, 60, 64, 65, 66, 67, 69, 72, 74, 75, 76, 77, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99}, {1, 6, 12, 15, 19, 20, 22, 28, 29, 31, 33, 35, 44, 45, 46, 50, 55, 57, 62, 64, 66, 68, 71, 73, 74, 75, 77, 83, 84, 92, 97, 99}, {13, 28, 34, 36, 38, 42, 45, 65}, {0, 8, 11, 28, 30, 33, 34, 39, 50, 51, 52, 56, 58, 62, 66, 67, 68, 85, 89, 96, 97}, {2, 4, 10, 11, 12, 14, 16, 18, 20, 26, 27, 29, 31, 33, 34, 35, 36, 40, 41, 42, 43, 46, 47, 48, 49, 51, 55, 58, 59, 61, 64, 68, 70, 71, 73, 75, 77, 78, 79, 80, 81, 82, 86, 88, 91, 92, 93}, {6, 10, 16, 20, 31, 32, 33, 38, 41, 42, 48, 63, 75, 77, 78, 82, 84, 91, 92, 93, 96}, {0, 1, 5, 6, 7, 9, 10, 11, 12, 13, 15, 17, 18, 20, 22, 23, 24, 25, 28, 29, 30, 31, 32, 33, 34, 38, 39, 40, 41, 42, 43, 45, 46, 48, 49, 50, 53, 55, 56, 57, 58, 59, 61, 63, 64, 65, 66, 67, 69, 70, 74, 76, 77, 78, 79, 80, 82, 83, 85, 86, 87, 88, 92, 93, 95, 97, 98, 99}, {0, 1, 5, 6, 7, 9, 12, 13, 15, 19, 24, 26, 27, 30, 33, 34, 36, 38, 39, 42, 43, 62, 64, 65, 66, 71, 72, 75, 79, 82, 83, 84, 88, 90, 92, 94}, {7, 16, 28, 38, 53, 56, 89, 99}, {1, 3, 5, 6, 7, 9, 18, 21, 30, 31, 34, 38, 44, 45, 51, 52, 56, 57, 58, 61, 62, 65, 69, 71, 76, 77, 79, 80, 81, 82, 83, 85, 87, 90, 93, 99}, {0, 3, 5, 6, 7, 9, 10, 13, 14, 15, 18, 19, 24, 27, 28, 29, 31, 32, 33, 34, 36, 37, 38, 39, 41, 42, 45, 48, 51, 52, 53, 54, 55, 58, 60, 61, 64, 68, 69, 71, 72, 73, 77, 78, 79, 80, 81, 84, 86, 88, 89, 90, 92, 94, 96}, {4, 5, 6, 7, 8, 17, 21, 22, 26, 29, 31, 39, 45, 46, 47, 49, 52, 60, 66, 69, 70, 71, 75, 78, 83, 84, 87, 88, 92, 94, 95, 99}, {0, 3, 6, 7, 10, 11, 12, 13, 14, 15, 16, 17, 20, 21, 22, 24, 26, 27, 29, 30, 31, 32, 33, 35, 40, 41, 42, 43, 44, 45, 46, 48, 49, 51, 52, 53, 56, 58, 60, 61, 62, 63, 64, 65, 67, 68, 70, 71, 73, 74, 78, 80, 81, 83, 84, 85, 87, 88, 89, 91, 92, 93, 96, 97, 98}, {0, 2, 11, 12, 18, 29, 30, 32, 33, 38, 39, 43, 45, 49, 50, 52, 56, 58, 62, 66, 70, 73, 74, 78, 80, 81, 83, 85, 87, 88, 90, 94}, {15, 25, 55, 87, 95, 98}, {2, 4, 18, 20, 23, 25, 29, 30, 32, 33, 34, 35, 40, 41, 43, 44, 47, 48, 50, 53, 54, 57, 67, 72, 76, 77, 79, 81, 82, 85, 86, 89, 91, 92, 93, 94, 96}, {0, 1, 2, 3, 4, 5, 9, 10, 12, 13, 14, 16, 17, 18, 19, 20, 21, 22, 23, 25, 26, 28, 29, 30, 31, 32, 34, 35, 36, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 52, 53, 54, 55, 56, 57, 58, 61, 62, 63, 64, 66, 68, 69, 70, 71, 72, 74, 75, 76, 77, 78, 79, 80, 81, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 94, 95, 97, 98, 99}, {0, 2, 11, 16, 19, 27, 29, 31, 34, 39, 50, 52, 56, 58, 63, 64, 68, 71, 73, 75, 76, 80, 88, 95, 98}, {0, 2, 4, 7, 8, 9, 10, 11, 13, 14, 15, 16, 19, 20, 21, 23, 27, 28, 30, 32, 33, 35, 36, 37, 38, 41, 43, 44, 45, 47, 48, 49, 50, 51, 52, 53, 55, 56, 58, 59, 60, 61, 62, 63, 64, 65, 67, 69, 71, 72, 75, 76, 78, 79, 80, 82, 83, 84, 85, 88, 90, 91, 92, 93, 96, 98}, {0, 1, 9, 22, 44, 47, 74, 75, 78, 79, 80, 91, 99}, {0, 31, 41, 58, 78}, {9, 11, 19, 20, 21, 23, 24, 27, 29, 34, 35, 40, 43, 44, 45, 50, 56, 59, 69, 76, 98}, {0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 47, 48, 49, 50, 52, 53, 54, 55, 57, 58, 59, 60, 61, 62, 63, 64, 65, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99}, {1, 3, 4, 6, 11, 12, 16, 22, 27, 36, 41, 46, 51, 54, 55, 60, 61, 62, 65, 68, 70, 75, 81, 82, 86, 89, 95}, {0, 1, 2, 3, 4, 7, 8, 9, 12, 16, 21, 23, 24, 25, 26, 29, 30, 31, 33, 35, 36, 39, 40, 42, 43, 46, 53, 54, 55, 56, 58, 64, 68, 71, 72, 75, 78, 82, 83, 85, 87, 89, 90, 95, 96}, {1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 20, 23, 24, 25, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 40, 41, 42, 43, 46, 47, 48, 49, 51, 52, 53, 57, 58, 59, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 82, 84, 85, 86, 88, 91, 92, 93, 94, 95, 96, 98, 99}, {1, 5, 6, 7, 9, 10, 12, 14, 19, 20, 22, 24, 25, 26, 27, 29, 30, 31, 34, 38, 39, 40, 41, 42, 43, 44, 45, 46, 48, 49, 50, 51, 53, 54, 55, 56, 60, 62, 65, 67, 69, 70, 71, 83, 84, 87, 88, 89, 90, 92, 93, 97, 98, 99}, {0, 1, 2, 3, 4, 6, 8, 9, 10, 11, 12, 14, 15, 16, 22, 24, 25, 26, 27, 29, 30, 31, 32, 33, 34, 35, 37, 38, 41, 42, 43, 44, 47, 48, 50, 52, 53, 54, 55, 57, 58, 60, 62, 63, 64, 65, 68, 69, 72, 73, 74, 79, 80, 81, 82, 83, 84, 88, 91, 92, 93, 94, 95, 97}, {1, 6, 7, 12, 14, 16, 19, 21, 22, 25, 27, 31, 32, 33, 36, 38, 44, 50, 51, 52, 54, 57, 60, 61, 62, 65, 66, 68, 69, 73, 76, 77, 78, 79, 81, 83, 84, 87, 89, 90, 93, 98}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 16, 17, 18, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 39, 40, 41, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 83, 84, 85, 86, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99}, {7, 8, 10, 16, 21, 25, 26, 27, 28, 29, 30, 31, 34, 35, 37, 38, 39, 42, 51, 53, 57, 58, 66, 67, 69, 71, 78, 82, 83, 84, 85, 86, 87, 90, 91, 93, 94}, {1, 2, 9, 12, 13, 15, 19, 21, 22, 24, 25, 26, 27, 28, 29, 30, 32, 34, 35, 36, 39, 40, 41, 42, 48, 51, 53, 54, 55, 56, 62, 63, 64, 69, 71, 72, 74, 76, 77, 80, 82, 83, 85, 86, 88, 93, 97}, {0, 1, 2, 3, 4, 5, 6, 7, 10, 13, 14, 15, 16, 18, 19, 20, 21, 23, 25, 26, 27, 28, 29, 30, 32, 34, 35, 36, 38, 40, 41, 42, 44, 45, 47, 49, 50, 51, 52, 53, 54, 55, 56, 57, 59, 60, 62, 64, 66, 67, 69, 70, 71, 74, 75, 76, 78, 83, 84, 85, 87, 89, 90, 92, 93, 94, 95, 96, 98, 99}, {0, 1, 2, 4, 5, 6, 7, 8, 10, 12, 13, 14, 15, 16, 17, 19, 20, 21, 22, 25, 26, 27, 29, 32, 33, 34, 35, 37, 38, 39, 40, 41, 43, 45, 47, 48, 49, 50, 52, 53, 54, 56, 57, 58, 59, 61, 63, 64, 65, 66, 67, 68, 71, 72, 73, 74, 75, 76, 77, 79, 80, 81, 84, 85, 88, 90, 92, 93, 95, 96, 97, 98}, {1, 29, 31, 44, 48, 51, 54, 59, 68, 70, 73, 87, 92, 98}, {0, 1, 4, 8, 17, 22, 23, 24, 26, 27, 30, 32, 37, 38, 39, 40, 41, 42, 43, 45, 46, 47, 49, 50, 52, 56, 57, 60, 62, 64, 67, 69, 71, 73, 74, 76, 77, 78, 79, 83, 84, 85, 89, 90, 93, 94, 96, 97}, {0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 19, 20, 22, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 47, 48, 49, 50, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99}, {2, 3, 4, 5, 7, 8, 9, 10, 11, 13, 15, 16, 18, 19, 20, 21, 22, 23, 25, 26, 27, 28, 30, 31, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 49, 51, 52, 53, 54, 55, 57, 59, 60, 61, 62, 64, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 78, 79, 80, 81, 82, 84, 85, 86, 87, 88, 89, 91, 92, 93, 94, 95, 96, 97, 98, 99}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 21, 22, 24, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 87, 88, 89, 90, 91, 92, 94, 95, 96, 97, 98, 99}, {2, 6, 7, 10, 13, 17, 20, 21, 22, 24, 25, 26, 27, 29, 30, 31, 32, 33, 34, 35, 36, 40, 41, 43, 44, 47, 48, 49, 50, 52, 53, 57, 59, 60, 61, 62, 64, 65, 70, 71, 72, 75, 76, 77, 78, 80, 82, 86, 87, 90, 91, 94, 96, 97}, {12, 15, 63, 77}, {2, 4, 11, 12, 13, 15, 16, 18, 19, 20, 23, 24, 25, 26, 27, 29, 30, 34, 35, 38, 39, 40, 41, 42, 43, 44, 47, 49, 50, 51, 53, 55, 56, 57, 59, 61, 62, 64, 65, 68, 70, 71, 72, 74, 76, 77, 78, 79, 81, 82, 84, 87, 88, 89, 90, 93, 96, 97, 98, 99}, {1, 6, 7, 11, 14, 22, 24, 25, 28, 31, 39, 46, 49, 50, 54, 55, 57, 61, 64, 68, 70, 72, 75, 77, 79, 80, 81, 86, 87, 91, 96, 99}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 29, 30, 31, 32, 33, 34, 36, 37, 38, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99}}};
}