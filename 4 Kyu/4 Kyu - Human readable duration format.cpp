#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <chrono>

using namespace std;

class Assert {
    public :
        template<class T> static void That (const T &actual, const T &expression) {
            if (actual != expression)
                std::cout<<"actual : "<<actual<<" expected : "<<expression<<std::endl;
        }
};
void Test();

string format_duration(double duration) {

    if (duration == 0.0) return "now";
    const vector<pair<int,string>> base {{31536000,"year"},{86400,"day"}, {3600,"hour"},{60,"minute"}, {1,"second"}};

    int count = 0;
    vector<int> period;
    ostringstream os;

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
    cout << duration << "\n";
    return os.str();
}

int main() {
    auto start = chrono::high_resolution_clock::now();

    double time = 345.62;

    int duration = time;
    double misc = time - duration;
    cout << format_duration(345.62);

    if (misc > 0.0) {
      // cout << misc << " ms ";
    }

    // Test();

    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "\nProcess took " << elapsed.count()  << " ms" << endl;
}

string Equals (string entry) { return entry;}

void Test () {
  Assert::That(format_duration(0), Equals("now"));
  Assert::That(format_duration(1), Equals("1 second"));
  Assert::That(format_duration(62), Equals("1 minute and 2 seconds"));
  Assert::That(format_duration(120), Equals("2 minutes"));
  Assert::That(format_duration(3662), Equals("1 hour, 1 minute and 2 seconds"));
}
