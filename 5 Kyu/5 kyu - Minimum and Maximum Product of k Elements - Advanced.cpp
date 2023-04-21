#include <iostream>
#include <vector>
#include <numeric>
#include <functional>
#include <queue>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
class Assert {
    public :
        template<class T> static void That (const T& a, const T& b) {
            if (a != b) {
                cout << "actual : ";
                for (auto &it : a) cout << it << " ";

                cout << " expected : ";
                for (auto &it : b) cout << it << " ";

                cout << endl;
            }
        }
        template<class T> static void That (const vector<T> &a, const vector<T> &b) {
            if (a != b) {
                cout << "actual : ";
                for (auto &it : a) cout << it << " ";

                cout << " expected : ";
                for (auto &it : b) cout << it << " ";

                cout << endl;
            }
        }
        template<class T> static void That (const pair<T,T> &a, const pair<T,T> &b) {
            if (a != b) {
                cout << "actual : ";
                cout << "[" << a.first << "," << a.second << "]\n";

                cout << " expected : ";
                cout << "[" << b.first << "," << b.second << "]\n";
                cout << endl;
            }
        }
};
template<class T> T Equals (const T& entry) { return entry;}
template<class T> pair<T,T> Equals (const pair<T,T> &entry) { return entry;}
template<class T> T EqualsContainer (const T& entry) { return entry;}

void Test ();
////////////////////////////////////////////////////////////////////////////////
void showvect (const vector<int32_t> &arr) {
  for (auto &it : arr) cout << it << " ";
  cout << "\n";
}
pair<int64_t,int64_t> find_min_max_product2 (vector<int32_t> arr, int k) {
    // vector<int32_t>::iterator start, end;
    sort (arr.begin(), arr.end());

    if (k > arr.size()) throw::exception();
    if (k == 1) return {arr.front(), arr.back()};

    int32_t minv = arr.front(), maxv = accumulate (arr.end() - (k + 1), arr.end(), 1, multiplies<int32_t>());

    showvect(arr);

    // cout << accumulate (arr.begin(), arr.begin() + k, 1, multiplies<int32_t>());;
    // cout << minv * maxv << ' ' << maxv * arr[arr.size() - k];
    return {minv * maxv, maxv * arr[arr.size() - k]};
}

int32_t maxprod (vector<int32_t> arr, int k) {
    int32_t maxv = -999;
    for (int i = 0; i <= arr.size() - k; i++) {
        int32_t prod = 1;
        for (int j = i; j < i + k; j++) {
            prod *= arr[j];
        }
        cout << prod << " ";
        maxv = max (maxv, prod);
    }
    return maxv;
}
int32_t minprod (vector<int32_t> arr, int k) {

    priority_queue<int, vector<int>, greater<int> > pq;
    for (int i = 0; i < arr.size(); i++)
        pq.push(arr[i]);

    int32_t count = 0, res = 1;
    // One by one extract items from max heap
    while (pq.empty() == false && count < k) {
        res *= pq.top();
        pq.pop();
        count++;
    }

    return res;
}

int32_t minProdK (vector<int32_t> &arr, int k) {

    int32_t prod = 1, zeros = 0;
    int N = arr.size();

    for (int i = 0; i < k; i++) {
        if (arr[i] == 0) {
            zeros++;
        } else {
            prod *= arr[i];
        }
    }

    int res = zeros == 0 ? prod : 0;

    for (int right = k; right < N; right++) {

        if (arr[right] == 0) {
          zeros++;
        } else {
          prod *= arr[right];
        }

        if (arr[right - k] == 0) {
            zeros--;
          } else {
            prod /= arr[right - k];
          }

        if (zeros == 0) {
            res = min (res, prod);
        } else if (res > 0) {
            res = 0;
        }
    }

    // Return the answer as res
    return res;
}
pair<int64_t,int64_t> find_min_max_product (vector<int32_t> arr, int k) {
    // vector<int32_t>::iterator start, end;
    sort (arr.begin(), arr.end());

    if (k > arr.size()) throw::exception();
    if (k == 1) return {arr.front(), arr.back()};


    cout << minProdK (arr,k) << " " << 0 << "\n";

    return {0,0};
}
int main () {

    int k = 2;

    find_min_max_product({1, -2, -3, 4, 6, 7}, k); // std::pair {-21,42}
    find_min_max_product({0,-1,-2,-3,-4}, k); // std::pair {0, 12}

  /*
    using Pair = std::pair<int64_t, int64_t>;

    // k = 1: -3, 7
    Assert::That(find_min_max_product({1, -2, -3, 4, 6, 7}, 1), Equals(Pair{-3, 7}));
    // k = 2: -3 * 7 = -21, 6 * 7 = 42
    Assert::That(find_min_max_product({1, -2, -3, 4, 6, 7}, 2), Equals(Pair{-21, 42}));
    // k = 3: -3 * 6 * 7 = -126, 4 * 6 * 7 = 168
    Assert::That(find_min_max_product({1, -2, -3, 4, 6, 7}, 3), Equals(Pair{-126, 168}));
    AssertThrows(std::exception, find_min_max_product({1, -2, -3, 4, 6, 7}, 7));
    */
}
