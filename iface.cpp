#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include <functional>

#include <thread>
#include <chrono>

using namespace std;
using func = std::function<std::string(int,int)>;


std::string u8 (const wchar_t wc) {
    char buffer[8] = {0};
    bool shifts = std::wctomb(nullptr, 0); // reset the conversion state
    const int ret = std::wctomb (buffer, wc);
    if (ret < 0) throw std::invalid_argument("inconvertible wide characters in the current locale");
    buffer[ret] = '\0';  // make 'buffer' contain a C-style string
    return std::string (buffer);
}

void header (vector<int> row) {
    for (int width : row) { // make head
        cout <<  "┌";
        for (int j = 0; j < width; j++) {
            cout << "─";
        }
        cout << "┐";
    }
    cout << "\n";
}
void bottom (vector<int> row) {
  for (int width : row) { // make bottom
      cout <<  "└";
      for (int j = 0; j < width; j++) {
          cout << "─";
      }
      cout << "┘";
  }
  cout << "\n";
}

string progress (const int &i, const int &j) {
    return j < i ? "#" : " "; 
}
string waiting (const int &i, const int &j) {
    wstring bar = L"▉▊▋▌▍▎▏▎▍▌▋▊▉";
    return u8 (bar[(i + j) % bar.size()]);
}

class Bar {
    public :
        int width, height;
        func fn;

        Bar (int a, int b, func cls) {
            width = a, height = b, fn = cls;
        }
};


string mkup (const Bar &curr) {
    string os;
        os += "┌";
        for (int j = 0; j < curr.width; j++) {
            os +="─";
        }
        os +="┐";
    

    return os;
}
string mkbot (const Bar &curr) {
    string os;
      os +=  "└";
      for (int j = 0; j < curr.width; j++) {
          os += "─";
      }
      os += "┘";
    return os;
}

string inner (const Bar &curr, int i) {
    string os;

    os += "│";
    for (int j = 0; j < curr.width; j++) {
        
        os += curr.fn (i,j);
    }

    os += "│";
    return os;
}
int main () {

    setlocale (LC_ALL, "en_US.UTF-8");

    wstring bar = L"▉▊▋▌▍▎▏▎▍▌▋▊▉";

    Bar curr (5,1, waiting);


    cout << mkup (curr) << "\n";

    cout << mkbot (curr) << "\n";

    //      vector<int> row {50,5};
    //
    //      for (int i = 0; i < 51; i++) {
    //            cout << "\033c\033[36m";
    //            header (row);
    //
    //            cout << "│";
    //            for (int j = 0; j < row[0]; j++) {
    //                if (j < i) {
    //                    cout << "▉";
    //                } else {
    //                    cout << " ";
    //                }
    //            }
    //            cout << "│";
    //
    //            cout << "│";
    //            for (int j = 0; j < row[1]; j++) {
    //                    cout << u8 (bar[(i + j) % bar.size()]);
    //            }
    //            cout << "│";
    //            cout << "\n";
    //
    //            bottom (row);
    //            std::this_thread::sleep_for(std::chrono::milliseconds(70));
    //      }
    //
}
