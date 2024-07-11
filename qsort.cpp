#include <iostream>
#include <vector>

int partition(int arr[], int low, int high) {
    int pivot = arr[low];
    int k = high;

    for (int i = high; i > low; i--) {
        if (arr[i] > pivot)
            swap(arr[i], arr[k--]);
    }

    swap(arr[low], arr[k]);
    return k;
}
void quickSort(int arr[],injt low,int high) {
  if(low < high) {
    int ix = partition (arr, low, high);

    quickSort (arr, low, ix - 1);
    quickSort (arr, ix + 1, high);
  }
}

int main () {

    std::vector<std::vector<int>> res;
    std::vector<int> vec = {30, 50, 5, 30, 51, 10, 20, 99, 18, 30, 100, 33};

    int i = 0, j = vec.size() - 1;

    int ref = vec[i];

    while (vec[j] >= ref) {
        j--;
    }

    if (vec[i] >= ref) {
        i++;
    }

    if (vec[i] > vec[j]) {
        std::swap(vec[i] , vec[j]);
    }

    ref = vec[i];

    std::cout << vec[i] << " " << vec[j] << "\n";


    for (int i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << " ";
    }



}
