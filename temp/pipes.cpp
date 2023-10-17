#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

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

bool evaluate (const vector<wstring> &pipe, int x, int y) {

    const vector<vector<wchar_t>> accept= {{L'╋',L'┃',L'┫',L'┣',L'┏',L'┓',L'┳'}, {L'╋',L'━',L'┓',L'┛',L'┫',L'┳',L'┻'} , {L'╋',L'┃',L'┫',L'┣',L'┗',L'┛',L'┻'} , {L'╋',L'━',L'┏',L'┗',L'┣',L'┳',L'┻'}}; 
    wchar_t cell = pipe[y][x];
    vector<int> next;

    switch (cell) {
        case L'━': next = {1,3}; break;
        case L'┏': next = {1,2}; break;
        case L'┃': next = {0,2}; break;
        case L'┗': next = {0,1}; break;
        case L'┓': next = {2,3}; break;
        case L'┛': next = {0,3}; break;
        case L'┳': next = {1,2,3}; break;
        case L'┣': next = {0,1,2}; break;
        case L'┫': next = {0,2,3}; break;
        case L'┻': next = {0,1,3}; break;
        case L'╋': next = {0,1,2,3}; break;
        default: break;
    }

    if (next.size()) {
        wcout << "[" << pipe[y][x] << "] => ";
        for (int i = 0; i < next.size(); i++) {
            int dx = direct[next[i]].first, dy = direct[next[i]].second;
            int nx = x + dx, ny = y + dy;

            if (is_inside (pipe, nx, ny)) {
                if (pipe[ny][nx] == '.') {
                    wcout << " leak [" << pipe[ny][nx] << "]";
                    return false;
                } else if (pipe[ny][nx] == ' ') {
                    int ox = x + dx * 2, oy = y + dy * 2;
                    //wcout << "[" << pipe[oy][ox] << "]";
                } else {
                    if (find (accept[i].begin(), accept[i].end(), pipe[ny][nx]) != accept[i].end()) {
                        wcout << "[" << pipe[ny][nx] << "]";
                    }
                }
            }
        }
        wcout << "\n";
    }
    return false;

}
bool leak (const vector<wstring> &pipe) {

    showgraph (pipe);
    const vector<pair<int,int>> direct = {{0,-1},{1,0},{0,1},{-1,0}}; // north,east,south,west
    const vector<vector<wchar_t>> accept= {{L'╋',L'┃',L'┫',L'┣',L'┏',L'┓',L'┳'}, {L'╋',L'━',L'┓',L'┛',L'┫',L'┳',L'┻'} , {L'╋',L'┃',L'┫',L'┣',L'┗',L'┛',L'┻'} , {L'╋',L'━',L'┏',L'┗',L'┣',L'┳',L'┻'}}; 

    for (int y = 0; y < pipe.size(); y++) {
        for (int x = 0; x < pipe[y].size(); x++) {
            wchar_t cell = pipe[y][x];
            vector<int> next;

            switch (cell) {
                case L'━': next = {1,3}; break;
                case L'┏': next = {1,2}; break;
                case L'┃': next = {0,2}; break;
                case L'┗': next = {0,1}; break;
                case L'┓': next = {2,3}; break;
                case L'┛': next = {0,3}; break;
                case L'┳': next = {1,2,3}; break;
                case L'┣': next = {0,1,2}; break;
                case L'┫': next = {0,2,3}; break;
                case L'┻': next = {0,1,3}; break;
                case L'╋': next = {0,1,2,3}; break;
                default: break;
            }

            if (next.size() > 9000) {
                wcout << "[" << pipe[y][x] << "] => ";
                for (int i = 0; i < next.size(); i++) {
                    int dx = direct[next[i]].first, dy = direct[next[i]].second;
                    int nx = x + dx, ny = y + dy;

                    if (is_inside (pipe, nx, ny)) {
                        if (pipe[ny][nx] == '.') {
                            wcout << " leak [" << pipe[ny][nx] << "]";
                            return false;
                        } else if (pipe[ny][nx] == ' ') {
                            int ox = x + dx * 2, oy = y + dy * 2;
                            //wcout << "[" << pipe[oy][ox] << "]";
                        } else {
                            if (find (accept[i].begin(), accept[i].end(), pipe[ny][nx]) != accept[i].end()) {
                                wcout << "[" << pipe[ny][nx] << "]";
                            }
                        }
                    }
                }
                wcout << "\n";
            }
        }
    }

    return true;
}

int main() {

    setlocale (LC_ALL, "en_US.UTF-8");

    vector<wstring> pipe = {L"     +", L"   + ╋━━┓", L"     ┃..┃", L"   + ┛..┣ +", L"        +"};
    // pipe = {L"...┏", L"...┃", L"┛..┣"}; // thsi is leaking
    // pipe = {L"...┏", L"...┃", L"┓..┣"}; // this is not leaking
    // pipe = {L"....", L".┛┛.", L"...." };

    pipe = {L"╋━━┓", L"┃..┃", L"┛..┣" };

    for (int y = 0; y < pipe.size(); y++) {
        wchar_t x = pipe[y].size() - 1;
        evaluate (pipe, 0, y), evaluate (pipe, x, y);
    }


    int las = pipe.size()-1;

    for (int x = 0; x < pipe[0].size(); x++) {
        wcout << pipe[0][x];
    }


    for (int x = 0; x < pipe[las].size(); x++) {
        wcout << pipe[las][x];
    }



    //leak (pipe);
    wcout << L"\nend\n";

}

void Test () {

    // pipe = {"┫.....", "┫.....", "┃.....", "┫┏━━┓.", "┣┫..┗━", "┣┫....", "┣┫...."};
}
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
