#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <vector>
#include <list>
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
string recover_secret (vector<vector<char>> &tri) {
		int size = tri.size();
		vector<int> visit (size);
		list<char> link;

		link.push_back(tri[0][0]);
		link.push_back(tri[0][2]);

		list<char>::iterator it, nx;
		char a, b;

		int index = 6;

		while (index-->0) {
				for (auto it = link.begin(); it != link.end(); it++) {
						nx = std::next(it);
						a = *it, b = *nx;
						cout << a << b << endl;
				}

				it = link.begin(), nx = std::next(it);
				a = *it, b = *nx;

				for (int i = 0; i < size; i++) {
						if (visit[i] == false) {
								if (*it == tri[i][0] && *nx == tri[i][2]) {
									link.insert(std::next(it), tri[i][1]);
									visit[i] = true;
									cout << *it << "[" << tri[i][1] << "]" << *nx << endl;
								}
								if (*it == tri[i][0] && *nx == tri[i][1]) {
									link.insert(std::next(nx), tri[i][2]);
									visit[i] = true;
									cout << *it << *nx << "[" << tri[i][2] << "]" << endl;
								}
								if (*it == tri[i][1] && *nx == tri[i][2]) {
									link.insert(it, tri[i][0]);
									visit[i] = true;
									cout << "[" << tri[i][0] << "]" << *it << *nx << endl;
								}
						}

				}
		}


		for (auto it = link.begin(); it != link.end(); it++) {
				cout << *it;
		}

		return "";
}
int main () {

		auto start = chrono::high_resolution_clock::now();
		//instr::move (south);
		vector<vector<char>>tri = {{'t','u','p'},{'w','h','i'},{'t','s','u'},{'a','t','s'},{'h','a','p'},{'t','i','s'},{'w','h','s'}};
		recover_secret(tri);

		/*
		cycle 0 : t-p
		use {'t','u','p'} => t [u] p

		cycle 1 : t-u
		{{'w','h','i'},{'t','s','u'},{'a','t','s'},{'h','a','p'},{'t','i','s'},{'w','h','s'}};
		use {'t','s','u'} = > t [s] u p

		cycle 2 : t-s
		{{'w','h','i'},{'a','t','s'},{'h','a','p'},{'t','i','s'},{'w','h','s'}};
		use {'a','t','s'} => [a] t s u p
		use {'t','i','s'} => a t [i] s u p

		{{'w','h','i'},{'h','a','p'},{'w','h','s'}};

		*/



		auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "\nProcess took " << elapsed.count()  << " ms" << endl;

}
