#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <limits>

#include <chrono>
// NEQ5xBztxeg43aP
using namespace std;

vector<int> tokenize (const string &src, char delim) {
    istringstream iss (src);
    string token;
    vector<int> v;

    while (getline (iss, token, delim))
        v.push_back (stoi (token));

    return v;
}
vector<vector<int>> getfile (string name) {
    int num;
    string buff;
    fstream file (name);
    vector<vector<int>> mat;

    while (getline (file, buff)) {
        mat.push_back(tokenize (buff, ','));
    }

    file.close();
    return mat;
}

using vertex = pair<int, pair<int,int>>;

void project82 () {
  vector<int> path = {131,201,96,342,746,422,121,37,331};

  vector<vector<int>> grid = {
    {131,673,234,103,18},
    {201,96,342,965,150},
    {630,803,746,422,111},
    {537,699,497,121,956},
    {805,732,524,37,331}};

  //grid = getfile("p081_matrix.txt");

  int size = grid.size();
  pair<int,int> exit = {size-1, size-1 };
  const vector<pair<int,int>> compass = {{0,1}, {1,0} , {0,-1}};

  priority_queue<vertex, vector<vertex>, greater<vertex>> q1;
  vector<vector<int>> dist (size, vector<int> (size));
  vector<vector<int>> visit (size, vector<int> (size));

  for (int i = 0; i < 0; i++) {
      q1.push({grid[0][i], {i,0}});
  }

  while (!q1.empty()) {
      auto [cost, u] = q1.top();
      q1.pop();

      dist[u.second][u.first] = cost;
      visit[u.second][u.first] = 1;
      if (u == exit) break;

      for (auto dir : compass) {
          int nx = u.first + dir.first;
          int ny = u.second + dir.second;

          if (nx>= 0 && ny >= 0 && nx < size && ny < size) {
              int alt = cost + grid[ny][nx];

              if ( dist[ny][nx] < alt && !visit[ny][nx]) {
                  q1.push({alt, {nx, ny}});
              }
          }
      }
  }

  for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
          cout << setw(5) << dist[i][j];
      }
      cout << endl;
  }

  /*

      for (int i = 1; i < size; i++) {
          grid[0][i] = grid[0][i] + grid[0][i-1];
          grid[i][0] = grid[i][0] + grid[i-1][0];
      }

      for (int i = 1; i < size; i++) {
          for (int j = 1; j < size; j++) {
              grid[i][j] = grid[i][j] + min (grid[i][j-1],grid[i-1][j]);
              //cout << setw(5) << grid[i][j];
          }
      }

      for (int i = 0; i < size; i++) {
          for (int j = 0; j < size; j++) {
              cout << setw(5) << grid[i][j];
          }
          cout << endl;
      }
      */
}

int main () {

    auto start = std::chrono::high_resolution_clock::now();

    vector<int> path = {131,201,96,342,234,103,18,150,111,422,121,37,331}; // 2297

    vector<vector<int>> grid = {
      {131,673,234,103,18},
      {201,96,342,965,150},
      {630,803,746,422,111},
      {537,699,497,121,956},
      {805,732,524,37,331}};

    grid = getfile("p083_matrix.txt");

    int size = grid.size();
    pair<int,int> exit = {size-1, size-1 };
    const vector<pair<int,int>> compass = {{0,1}, {1,0} , {0,-1}, {-1,0}};

    priority_queue<vertex, vector<vertex>, greater<vertex>> q1;
    vector<vector<int>> dist (size, vector<int> (size));
    vector<vector<int>> visit (size, vector<int> (size));

    for (int i = 0; i < 1; i++) {
        q1.push({grid[0][i], {i,0}});
    }

    while (!q1.empty()) {
        auto [cost, u] = q1.top();
        q1.pop();

        dist[u.second][u.first] = cost;
        visit[u.second][u.first] = 1;

        if (u == exit) {
            cout << cost;
            break;
        }

        for (auto dir : compass) {
            int nx = u.first + dir.first;
            int ny = u.second + dir.second;

            if (nx>= 0 && ny >= 0 && nx < size && ny < size) {
                int alt = cost + grid[ny][nx];

                if (dist[ny][nx] < alt && !visit[ny][nx]) {
                    q1.push({alt, {nx, ny}});
                }
            }
        }
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
        //    cout << setw(5) << dist[i][j];
        }
        //cout << endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}
