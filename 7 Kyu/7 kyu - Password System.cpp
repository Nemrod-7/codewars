#include <cmath>

class Crisis{
    public:
        static std::string helpZoom(std::vector<int> key) {
            const size_t end = key.size() - 1, mid = key.size() / 2 + 1, sq = sqrt (key.size());
            if (sq * sq != key.size()) return "No";
            for (size_t i = 0; i < mid; ++i) {
                if (key[end - i] != key[i])
                    return "No";
            }

            return "Yes";
        }
};
