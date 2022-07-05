#include <iostream>
#include <chrono>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
class Assert {
    public :
        template<class T> static void That (const T& a, const T& b) {
            if (a != b) {
                cout << "actual : " << a << " expected : " << b;
                cout << endl;
            }
        }
};
template<class T> T Equals (const T& entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////
class Timer {
    //using time = chrono::steady_clock;
    private :
        chrono::steady_clock::time_point alpha, end;
        chrono::duration<double> elapsed;
        uint64_t index;

        void update () {
            end = chrono::steady_clock::now ();
            elapsed = end - alpha;
        }
    public :
        Timer () {
            alpha = chrono::steady_clock::now ();
            index = 0;
        }
        void start () { alpha = chrono::steady_clock::now ();}
        void stop () { update();}
        void get_duration () {
            cout << "\nDuration "
                      <<fixed<< elapsed.count()
                      << " ms" << endl;
        }
        bool running (double total) {
            update();
            index++;
            if (elapsed.count() < total) return true;
                cout << "index :: " << index << endl;
            return false;
        }
};
////////////////////////////////////////////////////////////////////////////////
