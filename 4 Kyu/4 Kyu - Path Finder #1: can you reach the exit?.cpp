#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

const char PATH{'x'}, WALL{'#'}, START{'>'}, FREE{'.'}, EXIT{'X'};
//#define PATH 'x'
#define CELL(cell) (maze.grid[cell.x][cell.y])
#define IS_FREE(cell) (CELL(cell) == FREE || CELL(cell) == EXIT)

struct Point {
    int x, y;
};
struct Maze {
    int size;
    char **grid;
};
int m_size;

bool path_finder(string maze) ;
void display_maze (Maze maze) ;
void display (string maze) ;
void Test();

int main () {

    //path_finder(".W.\n.W.\n...");
    Test();
    return EXIT_SUCCESS;
}

Maze init_maze (string input) {

    int size = input.find('\n') + 2, next = 0;
    Maze maze = {.size = size, .grid = new char*[size]};
    char cell;

    //maze.grid.resize (size,vector<char>(size));
    //cout << input << endl;
    for (int i = 0; i < size; ++i) {
        maze.grid[i] = new char[size];
        for (int j = 0; j < size; ++j) {
            if (i == 0 || i == size - 1 ||
                j == 0 || j == size - 1) {
                maze.grid[i][j] = WALL;
            } else {
                if (input[next] == '\n') next++;

                if (input[next] == 'W') cell = WALL;
                else if (next == input.size() - 1) cell = EXIT;
                else cell = FREE;

                maze.grid[i][j] = cell;
                next++;
            }
            //cout << i << "," << j << " ";
            //cout << maze.grid[i][j];

        }
        //cout << endl;

    }
    return maze;
}
bool search_exit_v2 (Maze maze, Point curr) {

    if (CELL(curr) != FREE) return false;
    if (CELL(curr) == EXIT) return true;

    Point left = {curr.x, curr.y - 1}, right = {curr.x, curr.y + 1};
    Point top = {curr.x - 1, curr.y}, down = {curr.x + 1, curr.y};

    CELL(curr) = PATH;

    return search_exit_v2 (maze,right)|| search_exit_v2 (maze,down) ||
           search_exit_v2 (maze,top) || search_exit_v2 (maze,left);
}
bool search_exit (string &maze, Point curr) {

  int coord = curr.x * m_size + curr.y;
  Point left = {curr.x, curr.y - 1}, right = {curr.x, curr.y + 1};
  Point top = {curr.x - 1, curr.y}, down = {curr.x + 1, curr.y};

  if (maze[coord] != FREE) return false;
  if (coord == maze.size() - 1) return true;

  maze[coord] = PATH;

  return search_exit (maze,right)|| search_exit (maze,left) ||
         search_exit (maze,down) || search_exit (maze,top);
}

bool path_finder(string input) {
    /*
    Maze maze = init_maze (input);
    string actual = input;
    m_size = input.find('\n') + 1;
    search_exit_v2(maze, {1,1});
    search_exit(input, {1,1});

    std::vector<std::string> vec;
    std::stringstream it(input);
    */

    //cout << input << endl <<endl;
    //display_maze (maze);


    //search_exit (input, {0,0});
    //cout << endl << input << endl;
    //    cout << "exit : " << input.size() - 1 << endl;
    //cout << "solvable : " << search_exit_v2 (input, {0,0}) << endl;
    return search_exit (input, {0,0});;
}


void display_maze (Maze actual) {

    for (int i = 0; i < actual.size; ++i) {
        for (int j = 0; j < actual.size; ++j)
            cout << actual.grid[i][j];

        cout << endl;
    }
}

void Test() {

    path_finder(".W.\n.W.\n...");
    path_finder(".W.\n.W.\nW..");
    path_finder("......\n......\n......\n......\n......\n......");
    path_finder("......\n......\n......\n......\n.....W\n....W.");
    path_finder("....W\n.W...\n.W...\nWW...\n...W.");

    path_finder("...W...W.W\nW..W...W..\nW..W....WW\nWWW..WW.WW\n.....WW.W.\n..WW.W...W\n.....W..W.\n.W........\nW...WWW...\n..W.W..W..");
    path_finder("......W.W.\n....W..W..\n..W.....W.\nW.......W.\n.WWWW.....\n.WW.WWW...\nWW........\nWWW......W\n......WWW.\nW..W...WW.");


}
