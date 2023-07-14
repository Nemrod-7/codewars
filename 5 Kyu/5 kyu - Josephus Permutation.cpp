#include <iostream>
#include <vector>

std::vector<int> josephus(std::vector<int> items, int k) {
    std::vector<int> seq;
    int index = 0;

    while (items.size()) {
        index = (index + (k - 1)) % items.size();
        std::cout << index << ' ';
        seq.push_back(items[index]);
        items.erase(items.begin() + index);
    }
    return seq;
}


int main () {


  // josephus({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 1);
  // josephus({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 2);
  int index = 1;
  std::vector<int> items = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  items.erase(items.begin() + index);


  for (auto it : items) {
      std::cout << it << ' ';
  }

  std::cout << "\nexit\n";
}
