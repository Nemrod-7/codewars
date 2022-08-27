#include <iostream>
#include <vector>
#include <limits>

using namespace std;

void display (const vector<int> &arr, const vector<int> &flood, int height) {

    while (height-->0) {
        for (int i = 0; i < arr.size(); i++) {
            if (arr[i] <= height) {
                if (flood[i] > height) {
                    cout << "o";
                } else {
                    cout << " ";
                }
            } else {
                cout << "#";
            }
        }
        cout << endl;
    }
    for (int i = 0; i < arr.size(); i++) {
        cout << "#";
    }
    cout << endl;
}
void display2 (const vector<int> &arr, const vector<int> &flood, int height) {
    /*
    for (int i = right[0]; i >= 0; i--) {

        cout << (arr[0] <= i ? " " : "#");

        for (int j = 1; j < n - 1; j++) {
            int var = min (left[j - 1], right[j + 1]);
            if (arr[j] <= i) {
                if (var > arr[j] && var > i) {
                    cout << "o";
                } else {
                    cout << " ";
                }
            } else {
                cout << "#";
            }
        }
        cout << (arr[n-1] <= i ? " " : "#");
        cout << "\n";
    }

    for (int j = 0; j < n; j++) {
        cout << "#";
    }
    cout << '\n';
    */
}

int maxWater(vector<int> arr) {

    int res = 0;

    for (int i = 1; i < arr.size() - 1; i++) {

        int left = arr[i], right = arr[i];
        for (int j = 0; j < i; j++)     // Find the maximum element on its left
            left = max (left, arr[j]);

        for (int j = i + 1; j < arr.size(); j++) // Find the maximum element on its right
            right = max (right, arr[j]);

        res = res + (min (left, right) - arr[i]);
    }

    return res;
}
int findWater(vector<int> arr) {

    int width = arr.size();
    int sum = 0;
    int left[width], right[width];

    left[0] = arr[0];
    right[width - 1] = arr[width - 1];

    for (int i = 1; i < width; i++)
        left[i] = max (left[i - 1], arr[i]);

    for (int i = width - 2; i >= 0; i--)
        right[i] = max (right[i + 1], arr[i]);

    for (int i = 1; i < width - 1; i++) {
        int var = min (left[i - 1], right[i + 1]);
        if (var > arr[i]) {
            sum += var - arr[i];
        }
    }

    return sum;
}
int main () {

    vector<int> array = {0,1,0,2,1,0,1,3,2,1,2,1};

    int minv = numeric_limits<int>::max(), maxv = numeric_limits<int>::min();

    for (int i = 0; i < array.size(); i++) {
        minv = min (minv, array[i]);
        maxv = max (maxv, array[i]);
    }
    //findWater (array);
    int height = maxv;
    vector<int> flood (array.size(), maxv);

    int left = 0, right = array.size() - 1;

    for (int alt = 0; alt < height + 1; alt++) {
        /*
        while (array[left] < i) {
            if (flood[left] >= i) flood[left] = i - 1;
            left++;
        }
        while (array[right] < i) {
            if (flood[right] >= i) flood[right] = i - 1;
            right--;
        }
        */
    }

}
