#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include <random>
#include <chrono>

using namespace std;

struct Node {
    int num, cnt;
    Node *left, *right;
};

Node *newnode (int num) {
    Node *curr = new Node;
    curr->num = num;
    curr->cnt = 0;
    curr->left = curr->right = nullptr;
    return curr;
}

int insert(Node *&root, int nsmaller, int value) {

    if (root == nullptr) {
        root = newnode(value);
        return nsmaller;
    }

    if (root->num < value) {
        return root->cnt + insert(root->right, nsmaller + 1, value);
    } else {
        root->cnt++;
        return insert(root->left, nsmaller, value);
    }
}

std::vector<int> smaller(const std::vector<int> &arr) {
    std::vector<int> res (arr.size());
    int index = arr.size();
    Node *root = nullptr;

    while (index-->0) {
        int dig = arr[index];
        res[index] = insert(root, 0 , dig);
    }

    return res;
}

int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now(), end;
    chrono::duration<double> elapsed;

    const int size = 150000;
    vector<int> arr;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> rng (-1200, 1200);

    for (int i = 0; i < size; i++) {
        arr.push_back(rng(gen));
    }

    //arr = {5, 4, 7, 9, 2, 4, 1, 4, 5, 6};

    smaller(arr);


    end = chrono::steady_clock::now (), elapsed = end - alpha;
    std::cout << "\nDuration " << fixed << elapsed.count() << " ms" << std::endl;
}
