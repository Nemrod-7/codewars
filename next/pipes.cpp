#include <iostream>
#include <string>
#include <vector>

using namespace std;
const vector<pair<int,int>> direct = {{0,-1},{1,0},{0,1},{-1,0}}; // north,east,south,west

void showgraph (vector<wstring> pipe) {
   for (int i = 0; i < pipe.size(); i++) {
        for (int j = 0; j < pipe[i].size(); j++) {
            wcout << pipe[i][j];
        }
        wcout << "\n";
    }
}

bool is_inside (const vector<wstring> &pipe, int x, int y) { return x >= 0 && y >= 0 && y < pipe.size() && x < pipe[y].size(); }

vector<int> search (const vector<wstring> &pipe, vector<int> next, int x, int y) {
  vector<int> cell;

  for (int i = 0; i < next.size(); i++) {
      int nx = x + direct[next[i]].first, ny = y + direct[next[i]].second;
       if (is_inside (pipe, nx, ny)) {
         cell.push_back(static_cast<int> (pipe[ny][nx]));
      }
  }

  return cell;
}

bool leak (const vector<wstring> &pipe) {

  showgraph (pipe);
  const vector<pair<int,int>> direct = {{0,-1},{1,0},{0,1},{-1,0}}; // north,east,south,west

  for (int y = 0; y < pipe.size(); y++) {
      for (int x = 0; x < pipe[y].size(); x++) {
          wchar_t cell = pipe[y][x];
          vector<int> next;
          switch (cell) {
              case 9473:
              next = search (pipe, {1,3}, x, y);

              for (int p : next) {
                // wcout << "[" << (wchar_t) p << "]";
                  // if (p == 32 ) return false;
              }
              
              break; //  ━ - 9473 - BOX DRAWINGS HEAVY HORIZONTAL
              case 9475:
              next = search (pipe, {0,2}, x, y);

              break; //  ┃ - 9475 - BOX DRAWINGS HEAVY VERTICAL
              case 9487:
              next = search (pipe, {1,2}, x, y);

              break; //  ┏ - 9487 - BOX DRAWINGS HEAVY DOWN AND RIGHT
              case 9491:
              next = search (pipe, {2,3}, x, y);

              break; //  ┓ - 9491 - BOX DRAWINGS HEAVY DOWN AND LEFT
              case 9495:
              next = search (pipe, {0,1}, x, y);

              break; //  ┗ - 9495 - BOX DRAWINGS HEAVY UP AND RIGHT
              case 9499:
              next = search (pipe, {0,3}, x, y);

              break; //  ┛ - 9499 - BOX DRAWINGS HEAVY UP AND LEFT
              case 9507:
              next = search (pipe, {0,1,2}, x, y);

              break; //  ┣ - 9507 - BOX DRAWINGS HEAVY VERTICAL AND RIGHT
              case 9515:
              next = search (pipe, {0,2,3}, x, y);

              break; //  ┫ - 9515 - BOX DRAWINGS HEAVY VERTICAL AND LEFT
              case 9523:
              next = search (pipe, {1,2,3}, x, y);

              break; //  ┳ - 9523 - BOX DRAWINGS HEAVY DOWN AND HORIZONTAL
              case 9531:
              next = search (pipe, {0,1,3}, x, y);

              break; //  ┻ - 9531 - BOX DRAWINGS HEAVY UP AND HORIZONTAL
              case 9547:
              next = search (pipe, {0,1,2,3}, x, y);

              break; //  ╋ - 9547 - BOX DRAWINGS HEAVY VERTICAL AND HORIZONTAL
              default: break;
          }
      }
  }

  return true;
}

int main() {

    setlocale (LC_ALL, "en_US.UTF-8");

    vector<wstring> pipe = {L"     +", L"   + ╋━━┓", L"     ┃..┃", L"   + ┛..┣ +", L"        +"};
    // pipe = {L"...┏", L"...┃", L"┛..┣"}; // thsi is leaking

    leak (pipe);

    wcout << L"\nend\n";

}

void Test () {

    // pipe = {"┫.....", "┫.....", "┃.....", "┫┏━━┓.", "┣┫..┗━", "┣┫....", "┣┫...."};
    // pipe = {L"...┏", L"...┃", L"┓..┣"}; // this is not leaking

}
