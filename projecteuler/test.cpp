#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>

// NEQ5xBztxeg43aP
using namespace std;

pair<int,int> getm (vector<vector<int>> &adj) {
    int maxv = 0;
    pair<int,int> pt;

    for (int i = 0; i < adj.size(); i++) {
        for (int j = 0; j < adj[i].size(); j++) {
              if (adj[i][j] > maxv) {
                  maxv = adj[i][j];
                  pt = {i,j};
              }
        }
    }
    return pt;
}

bool detach (vector<vector<int>> &adj, pair<int,int> pt) {
    const int size = adj.size();
    int weight = adj[pt.first][pt.second];

    adj[pt.first][pt.second] = 0;
    adj[pt.second][pt.first] = 0;

    //cout << "["<< pt.first << "," << pt.second << "] => " << weight << "\n";

    int left = 0, right = 0;

    for (int i = 0; i < size; i++) {
        if (adj[pt.first][i] != 0) left++;
        if (adj[pt.second][i] != 0) right++;
    }

    if (left == 0 || right == 0) {
        adj[pt.first][pt.second] = weight;
        adj[pt.second][pt.first] = weight;
        return false;
    } else {
        return true;
    }
}


bool Hamiltonian_path (vector<vector<int>> &adj) {

    const int N = adj.size();
    int dp[N][(1 << N)];
    // Initialize the table
    //memset(dp, 0, sizeof dp);
    // Set all dp[i][(1 << i)] to true
    for (int i = 0; i < N; i++)
        dp[i][(1 << i)] = true;

    // Iterate over each subset of nodes
    for (int i = 0; i < (1 << N); i++) {
        for (int j = 0; j < N; j++) {
            // If the jth nodes is included in the current subset
            if (i & (1 << j)) {
                // Find K, neighbour of j also present in the current subset
                for (int k = 0; k < N; k++) {
                    if (i & (1 << k) && adj[k][j] && j != k && dp[k][i ^ (1 << j)]) {
                        // Update dp[j][i] to true
                        dp[j][i] = true;
                        break;
                    }
                }
            }
        }
    }

    // Traverse the vertices
    for (int i = 0; i < N; i++) { // Hamiltonian Path exists
        if (dp[i][(1 << N) - 1])
            return true;
    }

    return false;
}
int main () {

    auto start = std::chrono::high_resolution_clock::now();

    vector<vector<int>> adj {{0,16,12,21,0,0,0}, {16,0,0,17,20,0,0}, {12,0,0,28,0,31,0}, {21,17,28,0,18,19,23}, {0,20,0,18,0,0,11}, {0,0,31,19,0,0,27}, {0,0,0,23,11,27,0}};
    const int size = 50;


    /*

         A	 B	 C	 D	 E	 F	 G
      A	 -	16	12	21	 -	 -	 -
      B	16	 -	 -	17	20	 -	 -
      C	12	 -	 -	28	 -	31	 -
      D	21	17	28	 -	18	19	23
      E	 -	20	 -	18	 -	 -	11
      F	 -	 -	31	19	 -	 -	27
      G	 -	 -	 -	23	11	27	 -


      */

      pair<int,int> pt = getm (adj);
      int weight = adj[pt.first][pt.second];

      adj[pt.first][pt.second] = 0;
      adj[pt.second][pt.first] = 0;


    /*
    vector<vector<int>> graph (size);

    for (int i = 0; i < adj.size(); i++) {
        for (int j = 0; j < adj[i].size(); j++) {
              //cout << setw(2) << adj[i][j] << " ";
              if (adj[i][j] != 0) {
                  graph[i].push_back(j);
                  //grp[j].push_back(i);
              }
        }
        //cout << endl;
    }
    for (int i = 0; i < graph.size(); i++) {
        cout << static_cast<char> (i + 65) << " => ";

        for (int j = 0; j < graph[i].size(); j++) {
            cout << static_cast<char> (graph[i][j] + 65) << " ";
        }
        cout << "\n";
        //cout << grp[i].size() << "\n";
    }
    */
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}
