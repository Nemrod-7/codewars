#include <iostream>
#include <vector>

using namespace std;

bool palindrome (int n1, int n2) {
    int d1[10] = {}, d2[10] = {};

    for (int i = 0; i < 4; i++) {
        d1[n1 % 10]++, d2[n2 % 10]++;
        n1 /= 10, n2 /= 10;
    }

    for (int i = 0; i < 10; i++) {
        if (d1[i] != d2[i]) return false;
    }

    return true;
}

int main () {


		int lim = 60000;
		vector<bool> sieve (lim + 1, true);

		for (int p = 4; p <= lim ; p += 2) {
				sieve[p] = false;
		}
		for (int p = 3; p * p <= lim ; p += 2) {
		    if (sieve[p] == true) {
		        for (int i = p * p; i <= lim; i += 2 * p) {
		            sieve[i] = false;
		        }
		    }
		}

		int num = 56003;
		int rep = num % 10;
		string nu = to_string(num);


		for (int i = 0; i < 10; i++) {
			char ch = i + '0';
			string nxt = {nu[0],nu[1],ch,ch,nu[4]} ;
			int nxi = stoi(nxt);
			//cout << nx;
			if (sieve[nxi]) {
				cout << nxi << " ";
			}

			//cout << endl;
				/*
				*/
		}

		std::cout << "\nend\n";
}
