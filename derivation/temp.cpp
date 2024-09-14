#include <iostream>
#include <complex>
#include <cmath>

using namespace std;

int main () {

		string expr = "cot(39.6^11.5*42.3/x+93.7^73.3)";
		complex<double> x (-4.87,-7.25);

		complex<double> a(39.6), b(11.5), c(42.3), d(93.7), e(73.3);
		complex<double> rz = pow(a,b) * c / x + pow(d,e);

		cout << cos(rz);


}
