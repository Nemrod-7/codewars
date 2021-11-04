std::vector < int> josephus(std::vector < int > items, int k) {
    std::vector<int> seq;
    int index = 0;

    while (items.size()) {

        for (auto dig : items)
            std::cout << dig;

        std::cout << "\n";

        index = (index + (k - 1)) % items.size();

        seq.push_back(items[index]);
        items.erase(items.begin() + index);
    }
    return seq;
}
