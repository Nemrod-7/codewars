#include <iostream>
#include <iomanip>
#include <map>
#include <algorithm>
#include <chrono>

using namespace std;

int factorial (int n) {
    int mul = 1;

    for (int i = 1; i <= n; i++) {
        mul *= i;
    }
    return mul;
}

void getperm (string str) {

    sort (str.begin(), str.end());
    int i = 1;
    do {
        cout << i << " => " << str << "\n";
        i++;
    } while (next_permutation (str.begin(), str.end()));
}
int countperm (string str) {
  int n = str.size(), np = factorial(n);
  int hist[26] = {0};

  for (int i = 0; i < n; i++) {
      hist[str[i] - 'A']++;
  }

  for (int i = 0; i < 26; i++) {
      np /= factorial (hist[i]);
  }
  return np;
}
string kth_permutation2 (int k, string alpha) {
    const int n = alpha.size();
    string os;
    uint64_t xsum, sum;
    uint64_t freq[26] = {0};

    for (auto &ch : alpha) {
        freq[ch - 'A']++;
    }
    // Iterate till sum equals
    while (sum != k) {
        sum = 0;
        // Check for characters present in freq[]
        for (int i = 0; i < 26; i++) {
            if (freq[i] == 0) continue;
            freq[i]--; // Remove character
                       // Calculate sum after fixing a particular char
            xsum = factorial (n - os.size() - 1);
            for (int j = 0; j < 26; j++) {
                xsum /= factorial (freq[j]);
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

int64_t list_position (string str) {

    int n = str.size();
    int64_t k = 1;

    for (int i = 0; i < n; i++) {
        int infer = 0;
        for (int j = i + 1; j < n; j++) {
            if (str[j] < str[i]) {
                infer++;
            }
        }

        int freq[26] = {0};
        for (int j = i; j < n; j++) {
            freq[str[j] - 'A']++;
        }

        int64_t maxs =  factorial (n - i - 1) * infer;

        for (int j = 0; j < 26; j++) {
            maxs /= factorial (freq[j]);
        }

        k += maxs ;
    }
    cout << "\n" << str << " => " << k << "\n\n";
    return k;
}


int main () {

    chrono::steady_clock::time_point start = chrono::steady_clock::now(), end;
    chrono::duration<double> elapsed;

    // nperm = n! / (n1! . n2! . n...!)
    string str = "STRING";
    int n = str.size();
    string alpha;

    list_position ("YMYM");
  
    // list_position ("AAAB"); // 1
    // list_position ("ABAB"); // 2
    // list_position ("BAAA"); // 4
    // list_position ("YMYM"); // 5
    // list_position ("QUESTION"); // 24572
    // list_position ("BOOKKEEPER"); // 10743

    // string result = kth_permutation (0, word);
    // cout << word << " " << k << " " << result << '\n';

    end = chrono::steady_clock::now(), elapsed = end - start;
    std::cout << "\nDuration : " << fixed << elapsed.count() << " ms\n";
}
