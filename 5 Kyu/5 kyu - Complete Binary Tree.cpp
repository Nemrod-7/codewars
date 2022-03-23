#include <iostream>
#include <vector>

void inorder (std::vector<int>& input, int index, int &order) {
    if (index >= input.size()) return;

    inorder (input, 2 * index + 1, order);
    input[index] = order++;
    inorder (input, 2 * index + 2, order);
}
std::vector<int> complete_binary_tree (const std::vector<int>& input) {

    std::vector<int> order (arr.size());
    int pos = 0;
    inorder (order, 0, pos);

    for (int i = 0; i < order.size(); i++) {
        order[i] = arr[order[i]];
    }

    return order;
}

int main () {

    std::vector<int> arr = {1,2,3,4,5,6,7,8,9,10};
    std::vector<int> expected {7, 4, 9, 2, 6, 8, 10, 1, 3, 5};

}
