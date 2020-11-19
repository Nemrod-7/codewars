#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
typedef vector<string> test_t;
class Assert {
    public :
        static void That (test_t actual, test_t expression) {
            //if (actual != expression)
                //cout<<"actual : "<<actual<<" expected : "<<expression<<endl;
        }
};
test_t Equals (test_t entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////
/*
┌───┬───┬───┐
│ 1 │ 2 │ 3 │
├───┼───┼───┤
│ 4 │ 5 │ 6 │
├───┼───┼───┤
│ 7 │ 8 │ 9 │
└───┼───┼───┘
    │ 0 │
    └───┘
*/
vector<string> pad {"08","124","2135","326","4157","52468","6359","748","80579","968"};
vector<string> get_pins(string observed);

int main () {

    auto start = std::chrono::high_resolution_clock::now();

    get_pins ("11");

    std::cout << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Process took " << elapsed.count()  << " ms" << std::endl;

    return 0;
}

vector<string> get_pins(string observed) {

    size_t index = 1, i = 0;
    string key;
    for (auto it : observed)
        index *= pad[it - '0'].size();

    vector<string> out(index);

    while (index-->0) {
        for (auto dig : observed) {
            key = pad[dig - '0'];
            out.at(index) += key[i % key.size()];
            i /= key.size();
        }
        i = index;
    }

    return out;
}

void Test () {
vector<string> output = get_pins("8"); sort(output.begin(), output.end());
        vector<string> result = {"0", "5", "7", "8", "9"};
        Assert::That(output, Equals(result));

        output = get_pins("11"); sort(output.begin(), output.end());
        result = {"11", "12", "14", "21", "22", "24", "41", "42", "44"};
        Assert::That(output, Equals(result));

        output = get_pins("369"); sort(output.begin(), output.end());
        result = {"236", "238", "239", "256", "258", "259", "266", "268", "269", "296", "298", "299", "336", "338", "339", "356", "358", "359", "366", "368", "369", "396", "398", "399", "636", "638", "639", "656", "658", "659", "666", "668", "669", "696", "698", "699"};
        Assert::That(output, Equals(result));
}
