#include <iostream>
#include <vector>
#include <map>
#include <chrono>

using namespace std;

typedef vector<vector<int>> board;
typedef pair<int,int> cell;

#define x first
#define y second

//struct cell { int x, y;};

bool is_inside (cell p) { return p.x >= 0 && p.y >= 0 && p.x < 10 && p.y < 10;}
bool is_free (board &field, cell p) { return field[p.x][p.y];}
int scan (board &field, cell p, int &size) {

    if (!is_inside (p) || !is_free (field, p)) return false;

    vector<cell> diags {{1,-1},{1, 1}};
    for (auto &it : diags) { // check for diagonals
        cell next = {it.x + p.x, it.y + p.y};
        if (is_inside (next) && field[next.x][next.y]) return false;
    }

    cell right = {p.x, p.y + 1}, down = {p.x + 1, p.y};

    size++;
    field[p.x][p.y] = false;

    return scan (field,right,size)|| scan (field,down,size) + size;
}
bool validate_battlefield(board field) {
    //Debug::display (field);
    map <int,int> fleet;
    vector<cell> diags {{-1,-1},{-1, 1},{1,-1},{1, 1}};

    for (int x = 0; x < 10; x++)
        for (int y = 0; y < 10; y++)
            if (field[x][y]) {
                int size = 0;
                scan (field, {x, y}, size);

                fleet[size]++;
            }

    return fleet[1] == 4 && fleet[2] == 3 && fleet[3] == 2 && fleet[4] == 1;
}

class Debug {
  public :
  static void display (board &field) {
    for (int x = 0; x < field.size(); x++) {
      //cout << '{';
      for (int y = 0; y < field.size(); y++) {
        /*
        if (field[x][y])
        cout << "x" << " ";
        else
        cout << "  ";
        */
        cout << field[x][y] << ' ';
        //cout << (y + 1 != 10 ? "," : "},");
      }

      cout << endl;
    }
    cout << endl;
  }
};

int main () {

    auto start = std::chrono::high_resolution_clock::now();

    void Test ();
    Test ();
    //cout << recurse (2);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " <<fixed<< elapsed.count()  << " ms" << std::endl;
}

/////////////////////////////////Assert/////////////////////////////////////////
typedef bool test_t;
class Assert {
  public :
  static void That (test_t actual, test_t expression) {
    if (actual != expression)
    std::cout<<"actual : "<< actual <<" expected : "<< expression <<std::endl;
  }
};
test_t Equals (test_t entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////
void Test() {
  Assert::That(validate_battlefield(vector< vector<int> > {
       {1, 0, 0, 0, 0, 1, 1, 0, 0, 0},
       {1, 0, 1, 0, 0, 0, 0, 0, 1, 0},
       {1, 0, 1, 0, 1, 1, 1, 0, 1, 0},
       {1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
       {0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
       {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    }), Equals(true));

    Assert::That(validate_battlefield(vector< vector<int> > {
         {1, 1, 0, 1, 0, 0, 1, 1, 1, 1},
         {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
         {0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
         {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 1, 0, 0, 0, 0, 1, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 1, 0, 1, 0, 0, 1, 0},
         {1, 0, 0, 0, 0, 0, 0, 0, 1, 0}
      }), Equals(true));
      /*
*/
      Assert::That(validate_battlefield(vector< vector<int> > {
          {1, 0, 0, 0, 0, 1, 1, 0, 0, 0},
          {1, 0, 1, 0, 0, 0, 0, 0, 1, 0},
          {1, 0, 1, 0, 1, 1, 1, 0, 1, 0},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
          {0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
          {0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
      }), Equals(false));


}

/*

struct ship {
  int size;
  vector<cell> coord;
};

enum types {null,submarine,destroyer,cruiser,battleship};
enum state {water,full,visited};

void reset (ship &actual) {
  actual.size = 0;
  actual.coord.clear();
}

bool is_valid (ship curr) {
    if (curr.size < 3) return true;

    for (int i = 0; i < curr.size - 1; i++)
        if ((curr.coord[i + 1].x - curr.coord[i].x) > 1 ||
        (curr.coord[i + 1].y - curr.coord[i].y) > 1)
            return false;

    return true;
}

static void identify (ship curr) {

//cout << curr.size << endl;
switch (curr.size) {
case 1 : cout << "submarine\n"; break;
case 2 : cout <<  "destroyer\n"; break;
case 3 : cout << "cruiser\n"; break;
case 4 : cout << "battleship\n"; break;
default : cout << "unknown\n"; break;
}
//return "0";
}
*/
