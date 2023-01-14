#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <set>
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
string recover_secret2 (vector<vector<char>> &src) {

		list<char> word;
		list<vector<char>> tri (src.begin() + 1, src.end());

		word.push_back(src[0][0]);
		word.push_back(src[0][1]);
		word.push_back(src[0][2]);

		int index = 3;
		/*
		while (index-->0) {

				for (auto it = word.begin(); it != word.end(); it++) {
						char a = *it, b = *std::next(it);
						cout << a << " " << b << " => ";
						for (auto it2 = tri.begin(); it2 != tri.end(); ) {
								vector<char> level = *it2;
								char fst = level[0], sec = level[1], thr = level[2];
								cout << "[" << fst << " " << sec << " " << thr  << "]";

								if (a == fst && b == thr) {
									word.insert(std::next(it), sec);
									it2 = tri.erase(it2);
									//cout << a << "[" << sec << "]" << b << '|';
								} else if (a == fst && b == sec) {
									word.insert(std::next(it,2), thr);
									it2 = tri.erase(it2);
									//cout << a << b << "[" << thr << "]" << '|';
								} else if (a == sec && b == thr) {
									word.insert(it, fst);
									it2 = tri.erase(it2);
									//cout << "[" << fst << "]" << a << b << '|';
								} else {
										it2++;
								}
						}
						cout << endl;
				}
				cout << endl;
		}
		*/
		cout << "\n";
		for (auto it = word.begin(); it != word.end(); it++) {
				cout << *it;
		}

		return "";
}
string recover_secret3 (vector<vector<char>> &tri) {

		map<char, set<char>> mult;

		for (auto &level : tri) {
				char fst = level[0], sec = level[1], thr = level[2];
				cout << "[" << fst << " " << sec << " " << thr  << "]\n";
				mult[fst].insert(sec);
				mult[fst].insert(thr);
				mult[sec].insert(thr);
		}
		char start;

		for (auto it : mult) {
				bool found = false;
				char letter = it.first;

				for (auto it2 : mult) {
						auto after = it2.second;
						if (after.find(letter) != after.end()) {
								found = true;
						}
				}
				if (found == false)
					start = letter;
		}

		for (auto letter : mult) {
				cout << letter.first << " => ";
				for (auto after : letter.second) {
						cout << after << " ";
				}
				cout << endl;
		}
		/*
		*/
		return "";
}

bool search (vector<vector<char>> &tri, char letter) {
		for (int i = 0; i < tri.size(); i++) {
				for (int j = 1; j < tri[i].size(); j++) {
						if (tri[i][j] == letter) {
								return true;
						}
				}
		}
		return false;
}
bool search2 (vector<vector<char>> &tri, char letter) {

		for (int i = 0; i < tri.size(); i++) {
				int end = tri[i].size() - 1;
				if (end > 0) {
						for (int j = 0; j < end; j++) {
								if (tri[i][j] == letter) {
										return true;
								}
						}
				}
		}
		return false;
}
string recover_secret (vector<vector<char>> &tri) {

		string word;
		char curr;
		list<vector<char>> tt (tri.begin(), tri.end());
		vector<set<char>> hist (2);

		for (auto it = tt.begin(); it != tt.end(); it++) {
				vector<char> level = *it;
				for (int j = 0; j < 2; j++) {
						hist[j].insert(level[j]);
				}
		}

		bool running = true;
		while (running) {
				running = false;

				for (int i = 0; i < tri.size(); i++) {
						int size = tri[i].size();

						if (size > 0) {
								char last = tri[i][size - 1];
								running = true;

								if (search2 (tri, last) == false) {
										//cout << " [" << last << "]\n";
										word += last;
										for (int j = 0; j < tri.size(); j++) {
												if (tri[j].back() == last) {
														tri[j].pop_back();
												}
										}
										break;
								}
						}
				}
		}

		reverse(word.begin(),word.end());

		cout << word;

		/*
		for (int i = 0; i < tri.size(); i++) {
				cout << "[";
				for (int j = 0; j < tri[i].size(); j++) {
						cout << tri[i][j];
				}
				cout << "]\n";
		}
		*/
		return "";
}
int main () {

		auto start = chrono::high_resolution_clock::now();
		//instr::move (south);
		vector<vector<char>> tri = {
			{'t','u','p'},
			{'w','h','i'},
			{'t','s','u'},
			{'a','t','s'},
			{'h','a','p'},
			{'t','i','s'},
			{'w','h','s'}};

			tri = {{'t','s','f'}, {'a','s','u'}, {'m','a','f'}, {'a','i','n'}, {'s','u','n'}, {'m','f','u'}, {'a','t','h'}, {'t','h','i'}, {'h','i','f'}, {'m','h','f'}, {'a','u','n'}, {'m','a','t'}, {'f','u','n'}, {'h','s','n'}, {'a','i','s'}, {'m','s','n'}, {'m','s','u'}};
			 recover_secret(tri); /*

{'t','u','p'},
{'h','i'},
{'t','s','u'},
{'a','t','s'},
{'h','a','p'},
{'t','i','s'},
{'h','s'}};

=> w



*/
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
