#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;
///////////////////////////////////Assert///////////////////////////////////////
typedef int test_t;
class Assert {
    public :
        static void That (test_t actual, test_t expression) {
            if (actual != expression)
                std::cout<<"actual : "<<actual<<" expected : "<<expression<<std::endl;
        }
};
test_t Equals (test_t entry) { return entry;}
void Test ();

struct Point{
    int x, y;
};
const unsigned size = 8;
typedef vector<vector<int>> Grid;
vector<Point> knight_moves {{-2,-1},{-2,+1}, {-1,-2},{-1,+2},
                            {+1,-2},{+1,+2},{+2,-1},{+2,+1}};
////////////////////////////////////func def////////////////////////////////////
Grid init_grid();
void mark (Grid &board, Point p);
int knight(std::string start, std::string finish);
void display(vector<vector<int>> board);

////////////////////////////////////////////////////////////////////////////////

int main () {

    auto start = chrono::high_resolution_clock::now();

    //knight("a1", "c1");
    Test();
    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "Process took " << elapsed.count()  << " ms" << endl;

}



Grid init_grid() {
    Grid grid;
    for (int i = 0; i < size; ++i)
        grid.push_back(vector<int>(size));

    return grid;
}
bool is_inside (Point p) { return (p.x >=0 && p.y >=0 && p.x < size && p.y < size);}
bool update (Grid &board, vector<Point> &stack, Point exit) {

    for (Point actual : stack)
        for (Point pos : knight_moves) {
              Point p = {actual.x + pos.x, actual.y + pos.y};
              if (is_inside(p) && !board[p.x][p.y]) {
                      if (p.x == exit.x && p.y == exit.y)
                          return true;

                      board[p.x][p.y] = true;
                      stack.push_back({p.x,p.y});
              }
        }
    return false;
}
int knight(std::string start, std::string end) {

    Grid board = init_grid();
    Point exit{end[1] - '0',end[0] - 'a'};
    vector<Point> actual{{start[1] - '0',start[0] - 'a'}};
    int steps = 1;

    while (update (board, actual, exit) == false)
        steps++;

    return steps;
}

void display(Grid board) {
    cout << "----------board---------\n\n";
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            cout << "[";
            cout << (board[x][y] != 0 ? "#" : " ");
            cout << "]";
        }
        cout << endl;
    }
    cout << endl;
}

void Test() {
    Assert::That(knight("a1", "c1"), Equals(2));
    Assert::That(knight("a1", "f1"), Equals(3));
    //Assert::That(knight("a1", "f3"), Equals(3));
    //Assert::That(knight("a1", "f4"), Equals(4));
    //Assert::That(knight("a1", "f7"), Equals(5));

}
