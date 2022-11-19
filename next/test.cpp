#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

#include <chrono>

using namespace std;

enum dir {north,south,west,east};
struct instr {

		static pair<int,int> move (int dir) {

				switch (dir) {
						case north : return {0,-1};	break;
						case east  : return {1, 0}; break;
						case west  : return {0,-1}; break;
						case south : return {0, 1}; break;
						default 	 : return {0, 0}; break;
				}
		}

		void jump () {

		}
};
int solve (string equ) {

		int res = 0, sign = 1, lhs = 1, xsign = 1, num = 0;
		int index = 0;

		while (index < equ.size()) {
				char ch = equ[index];

				switch (ch) {
						case '=': sign = lhs = -1; break;
						case '+': sign = lhs; break;
						case '-': sign = -lhs; break;
						case 'x': xsign = -sign; break;
						case ' ': res += num * sign; num = 0; break;
						default: num = num * 10 + ch - '0'; break;
				}
				cout << ch << " " << res << '\n';
				index++;
		}

		return (res + num * sign) * xsign;
}

vector<int> getline (vector<int> line) {
		vector<int> dots;
		int acc = 0;

		for (int i = 0; i < 5; i++) {
				acc += line[i];
				if ((line[i] == 0 || i == 4) && acc > 0) {
						dots.push_back (acc);
						acc = 0;
				}
		}

		return dots;
}

bool recurse (vector<int> line, vector<int> clue, int x) {

		if (x == 5) {
				if (getline (line) == clue) {

				}
				for (auto cell : line) {
						cout << cell << ' ';
				}
				cout << endl;
				return true;
		} else {
				for (int i = 0; i < 32; i++) {
						for (int j = 0; j < 5; j++) {
								bool bit = i&1 << j;
								line[j] = bit;
						}

				}
		}

		return false;
}
int main () {

		auto start = chrono::high_resolution_clock::now();
		instr::move (south);

		uint64_t lim = 1e17;

		uint64_t size = 45;
		uint64_t nd = 1UL << size;

		for (uint64_t num = 0; num < nd; num++) {
				uint64_t acc = 0;
				vector<uint64_t> clue;

				//cout << setw(2) << num << " => ";

				for (uint64_t i = 0; i < size; i++) {
						bool bit = (num >> i &1);
						//cout << bit;
						acc += bit;
						if ((bit == 0 || i == size - 1) && acc != 0) {
								//clue.push_back(acc);
								acc = 0;
						}
				}
				/*
				cout << " :: ";
				for (auto it : clue) {
						cout << it << ' ';
				}
				cout << endl;
				
				*/
		}

		auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "\nProcess took " << elapsed.count()  << " ms" << endl;

}
