#include <iostream>
// #include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

int64_t fact[21];

void getperm (string str) {

    sort (str.begin(), str.end());

    do {
        cout << str << "\n";
    } while (next_permutation (str.begin(), str.end()));
}
string kth_permutation2 (int k, string alpha) {
    const int n = alpha.size();
    string os;
    uint64_t xsum, sum;
    uint64_t freq[26] = {0};
    fact[0] = 1;

    for (auto &ch : alpha) {
        freq[ch - 'A']++;
    }
    // Iterate till sum equals n
    while (sum != k) {
        sum = 0;
        // Check for characters present in freq[]
        for (int i = 0; i < 26; i++) {
            if (freq[i] == 0) continue;
            freq[i]--; // Remove character
                       // Calculate sum after fixing a particular char
            xsum = fact[n - os.size() - 1];
            for (int j = 0; j < 26; j++) {
                xsum /= fact[freq[j]];
            }
            sum += xsum;
            // if sum > n fix that char as present char and update sum and required nth after fixing char at that position
            if (sum >= k) {
                os += (i + 'a');
                k -= (sum - xsum);
                cout << xsum << " " << k << "\n";
                break;
            }
            // if sum < n, add character back
            if (sum < k) freq[i]++;
        }
    }
    // if sum == n means this char will provide its greatest permutation as nth permutation
    for (int i = 25; i >= 0; i--) {
        if (freq[i] == 0) continue;
        os += (i + 'A');
        freq[i++]--;
    }

    return os;
}

int64_t list_position (string result) {
    string alpha ;
    int ch, n = result.size();
    int64_t k = 1, freq[26] = {0};

    for (int i = 0; i < n; i++) {
        ch = result[n - i - 1];
        freq[ch - 'A']++;
        alpha += ch;
        sort(alpha.begin(), alpha.end());

        int64_t maxs =  fact[i];

        for (int j = 0; j < 26; j++) {
            maxs /= fact[freq[j]];
        }

        cout << maxs << " :: ";
        for (int j = 0; j < alpha.size(); j++) {
            if (alpha[j] == ch) {
                cout << j << ' ';
            }
        }

        cout << "\n";
        k += alpha.find(ch) * maxs;
        // cout << maxs << " :: ";
    }
    cout << result << " => " << k << "\n";
    return k;
}

int main () {

    chrono::steady_clock::time_point start = chrono::steady_clock::now(), end;
    chrono::duration<double> elapsed;

    fact[0] = 1;

    for (int64_t i = 1; i < 21; i++) {
        fact[i] = fact[i-1] * i;
    }

    //    getperm ("YMYM");
    //list_position ("AAAB"); // 1
    //list_position ("ABAB"); // 2
    //list_position ("BAAA"); // 4
    //list_position ("YMYM"); // 5
    //list_position ("QUESTION"); // 24572
    //list_position ("BOOKKEEPER"); // 10743

    "MMY";

    //  n! / (n1! . n2! . n...!);

    // string result = kth_permutation (0, word);
    // cout << word << " " << k << " " << result << '\n';

    end = chrono::steady_clock::now(), elapsed = end - start;
    std::cout << "\nDuration : " << fixed << elapsed.count() << " ms\n";
}
