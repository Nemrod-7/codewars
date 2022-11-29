#include <iostream>
#include <vector>
#include <algorithm>

#include <random>
#include <chrono>

using namespace std;

void read_clust (const vector<int> &input) {
    for (int i = 0; i < input.size(); i++)
        cout << input[i] << " ";
    cout << endl;
}

class combinations2 {
    private :
        int size;
        vector<vector<int>> subs;

        void recurse (vector<int> curr, int i) { //

            int *arr = curr.data();

            for (int j = 0; j < subs[i].size(); j++) {
                int dig = subs[i][j];

                if (find (&arr[0], &arr[i], dig) != &arr[i]) continue;
                arr[i] = dig;

                if (i == size - 1) {
                    //read_clust (curr);
                    combs.push_back (curr);
                } else {
                    recurse (curr, i + 1);
                }
            }
        }
        void recurse2 (string comb) { //

            int i = comb.size();

            for (int j = 0; j < subs[i].size(); j++) {
                char dig = subs[i][j];
                if (comb.find (dig) != string::npos) continue;

                string nxt = comb + dig;

                if (nxt.size() == size) {
                    /*
                    for (auto &it : nxt) {
                        cout << static_cast<int> (it) ;
                    }
                    cout << endl;
                    */
                    //combs.push_back (comb);
                } else {
                    recurse2 (nxt);
                }

            }
        }

        void makecomb () {

            vector<pair<int,vector<int>>> s1 ({{0, vector<int> (size)}});

            while (!s1.empty()) {
                auto [index, comb] = s1.back();
                s1.pop_back();

                if (index == size) {
                    //read_clust (comb);
                    combs.push_back (comb);
                } else {
                    for (int i = 0; i < subs[index].size(); i++) {
                        int dig = subs[index][i];
                        if (find (&comb[0], &comb[index], dig) != &comb[index]) continue;

                        comb[index] = dig;

                        s1.push_back ({index + 1, comb});
                    }
                }
            }

        }
        void makecomb2 () {

            vector<pair<int, string>> s1 ({{0, string (size, 0)}});

            while (!s1.empty()) {
                auto [index, comb] = s1.back();
                s1.pop_back();

                if (index == size) {
                    /*
                    for (auto &it : comb) {
                        cout << static_cast<int> (it) ;
                    }
                    cout << endl;
                    */
                    //combs.push_back (comb);
                } else {
                    for (int i = 0; i < subs[index].size(); i++) {
                        char dig = subs[index][i];
                        if (find (&comb[0], &comb[index], dig) != &comb[index]) continue;

                        comb[index] = dig;
                        s1.push_back ({index + 1, comb});
                    }
                }
            }
        }
        void makecomb3 () {

            vector<string> s1 ({""});

            while (!s1.empty()) {
                string comb = s1.back();
                int index = comb.size();
                s1.pop_back();

                if (comb.size() == size) {
                    /*
                    for (auto &it : comb) {
                        cout << static_cast<int> (it) ;
                    }
                    cout << endl;

                    */
                    //combs.push_back (comb);
                } else {
                    for (int i = 0; i < subs[index].size(); i++) {
                        char dig = subs[index][i];
                        if (comb.find (dig) != string::npos) continue;

                        s1.push_back (comb + dig);
                    }
                }

            }
        }
        void makecomb4 () {

            vector<pair<int,vector<int>>> s1 ({{0, vector<int> (size + 1)}});

            while (!s1.empty()) {
                auto [index, comb] = s1.back();
                s1.pop_back();

                if (index == size) {
                    //read_clust (comb);
                    combs.push_back (comb);
                } else {
                    for (int i = 0; i < subs[index].size(); i++) {
                        int dig = subs[index][i];
                        if (comb[dig]) continue;
                        vector<int> nxt = comb;

                        nxt[dig] = index + 1;

                        s1.push_back ({index + 1, nxt});
                        /*
                        */
                    }
                }
            }

        }

    public :
        vector<vector<int>> combs;

        combinations2 (const vector<vector<int>> &data) {

            subs = data;
            size = data.size();
            vector<int> arr (data.size());

            //recurse (arr, 0); // 0.7063
            //makecomb (); // 1.37593
            //makecomb2 (); // 0.658248

            makecomb3 (); // 0.354266
            //recurse2(""); // 0.20937
            //makecomb4 (); // 0.354266
        }
};

void filter (const vector<vector<int>> &subs) {

    vector<string> s1 ({""});

    while (!s1.empty()) {
        string comb = s1.back();
        int index = comb.size();
        s1.pop_back();

        if (comb.size() == subs.size()) {
            for (auto &it : comb) {
                cout << static_cast<int> (it) << ' ';
            }
            cout << endl;
            /*
            */
        } else {
            for (int i = 0; i < subs[index].size(); i++) {
                char dig = subs[index][i];
                if (comb.find (dig) != string::npos) continue;

                s1.push_back (comb + dig);
            }
        }

    }
}


int main() {

    auto start = std::chrono::high_resolution_clock::now();

    vector<int> mask = {8,8,8,8,8,8,8,8,8};

    int size = mask.size();
    vector<vector<int>> subs (size);

    for (int i = 0; i < size; i++) {
        for (int j = 1; j <= mask[i]; j++) {
            subs[i].push_back (j);
        }
    }

    //filter (subs);
    /*
    combinations2 total (subs);
    */

    //cout << total.combs.size();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}
