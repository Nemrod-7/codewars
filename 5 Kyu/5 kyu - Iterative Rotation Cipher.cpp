//#include <iostream>
//#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>

void shiftright (std::string::iterator begin, std::string::iterator end, int shift) {
    int size = distance (begin, end);
    if (size == 0) return;

    int modd = shift % size;

    std::string str (begin, end), sub (end - modd, end);

    str.erase (size - modd);
    str.insert (0, sub);

    for (auto &it : str) *begin++ = it;
}
void shiftlft (std::string::iterator begin, std::string::iterator end, int shift) {
    int size = distance (begin, end);
    if (size == 0) return;

    int modd = shift % size;
    std::string src (begin, end), str (end - (size - modd), end) ;

    str.insert (str.size(), src.substr (0, modd));
    for (auto &it : str) *begin++ = it;
}

std::string decode (std::string code) {
    int pos = code.find(' '), n = stoi (code.substr (0, pos)), index = n;
    std::string str = code.substr(pos + 1);
    std::vector<int> spaces;

    for (size_t i = 0; i < str.size(); i++)
        if (str[i] == ' ') spaces.push_back(i);

    while (index-->0) {

        for (auto it = str.begin(), fst = it; it != str.end() + 1; it++) {
            if (*it == ' ' || it == str.end()) {
                shiftlft (fst, it, n);
                fst = it + 1;
            }
        }
        str.erase(std::remove(begin(str), end(str), ' '), end(str));
        shiftlft (begin(str), end(str), n);

        for (auto &it : spaces)
            str.insert (it, " ");
    }

    return str;
}
std::string encode (int n, std::string src) {
    int index = n;
    std::vector<int> spaces;

    for (size_t i = 0; i < src.size(); i++)
        if (src[i] == ' ') spaces.push_back (i);

    while (index-->0) {
        src.erase(std::remove(begin(src), end(src), ' '), end(src));

        shiftright (begin(src), end(src), n);
        for (auto &it : spaces)
            src.insert (it, " ");

        for (auto it = src.begin(), fst = it; it != src.end() + 1; it++) {
            if (*it == ' ' || it == src.end()) {
                shiftright (fst, it, n);
                fst = it + 1;
            }
        }
    }

    return std::to_string(n) + ' ' + src;
}

int main () {

    std::string src = "If you wish to make an apple pie from scratch, you must first invent the universe.";
    //string sub (end(src) - shift % size, end(src));
    //cout << sub;
    //std::cout << encode (10, src);
    std::string code = "10 hu fmo a,ys vi utie mr snehn rni tvte .ysushou teI fwea pmapi apfrok rei tnocsclet";

    //cout << decode (code);
      // Equals ("10 hu fmo a,ys vi utie mr snehn rni tvte .ysushou teI fwea pmapi apfrok rei tnocsclet"));
    //  decode (code);
/* Step-by-step breakdown:
Step 1 — remove all spaces:
`Ifyouwishtomakeanapplepiefromscratch,youmustfirstinventtheuniverse.`

Step 2 — shift the order of string characters to the right by 10:
`euniverse.Ifyouwishtomakeanapplepiefromscratch,youmustfirstinventth`

Step 3 — place the spaces back in their original positions:
`eu niv erse .I fyou wi shtom ake anap plepiefr oms crat ch,yo umustf irs tinventth`

Step 4 — shift the order of characters for each space-separated substring to the right by 10:
`eu vni seer .I oufy wi shtom eak apan frplepie som atcr ch,yo ustfum sir htinventt`

Repeat the steps 9 more times before returning the string with `10 ` prepended.
*/

}
