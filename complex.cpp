#include <iostream>
#include <complex>
#include <sstream>

using namespace std;

complex<double> ctos (const string &input) {
    istringstream iss (input);
    complex<double> zx;
    iss >> zx;
    return zx;
}

int main () {


    stringstream oss;
    complex<double> z1 (2,3);
    oss << z1;

    string cell = oss.str();


    
    
    cout << ctos(cell);




    
}
