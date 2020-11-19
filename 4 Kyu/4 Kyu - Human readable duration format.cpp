#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <chrono>

using namespace std;

class Assert {
    public :
        static void That (auto actual, auto expression) {
            if (actual != expression)
                std::cout<<"actual : "<<actual<<" expected : "<<expression<<std::endl;
        }
};

string format_duration(int duration) ;
void Test();

void ttt (auto entry) {

    cout << entry;
}

int main() {
    auto start = chrono::high_resolution_clock::now();

    Test();

    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "Process took " << elapsed.count()  << " ms" << endl;
}


string format_duration(int duration) {

    if (duration == 0) return "now";

    vector<pair<int,string>> base{{31536000,"year"},{86400,"day"},
      {3600,"hour"},{60,"minute"}, {1,"second"}};

    vector<int> period;
    ostringstream os;
    int count = 0;

    for (auto it : base) {
        period.push_back (duration / it.first);

        if (period.back() != 0)
            count++;

        duration -= (period.back() * it.first);
    }

    for (int i = 0; i < 5; ++i) {
        if (period[i] != 0) {
            count--;
            os << period[i] << " " << base[i].second;
            if (period[i] > 1) os << "s";
            if (count > 1) os << ", ";
            if (count == 1) os << " and ";
        }
    }

    return os.str();
}


auto Equals (auto entry) { return entry;}

void Test () {
  Assert::That(format_duration(0), Equals("now"));
  Assert::That(format_duration(1), Equals("1 second"));
  Assert::That(format_duration(62), Equals("1 minute and 2 seconds"));
  Assert::That(format_duration(120), Equals("2 minutes"));
  Assert::That(format_duration(3662), Equals("1 hour, 1 minute and 2 seconds"));
}
