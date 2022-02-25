#include <vector>

bool valid_braces (std::string braces) {

    std::vector<char> brc;
  
    for (auto &cur : braces) {
        
        if (cur == '(' || cur == '[' || cur == '{') {
            brc.push_back (cur);
        } else {
            if (!brc.size()) return false;
            char last = brc.back();

            if ((cur == ')' && last == '(') || (cur == ']' && last == '[') || (cur == '}' && last == '{')) {
                brc.pop_back();
            } else {
                return false;
            }
        }

    }

    return brc.size() == 0;
}
