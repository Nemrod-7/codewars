#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define IS_BODY(x) ((x) == EAST || (x) == WEST || (x) == NORTH || (x) == SOUTH)


typedef struct s_Maze {
    unsigned height, width;
    char **grid;
} Maze;

typedef struct _point {
    int x, y;
} Point;

bool is_inside(Point p, const Maze *m) {
    return p.x >= 0 && p.y >= 0 && p.x < m->width && p.y < m->height;
}

char *escape(const Maze *maze) {

    const Point compass[4] = {{0,1},{1,0},{0,-1},{-1,0}};
    char *path = NULL;
    Point p;

    for (int i = 0; i < 4; i++) {
        int nx = p.x + compass[i].x, ny = p.y + compass[i].y;

        if (is_inside(p, maze) && maze->grid[ny][nx] == ' ') {

        }
    }

/*
 ** @param maze a valid Maze* (see description) with consistent height, width and grid.
 ** Note: maze->grid[0][0] is the top left-hand corner and maze->grid[maze->height - 1][maze->width - 1] is the bottom right-hand corner.
 ** @return a heap-allocated string composed of the instructions 'F', 'R', 'B' and 'L'. NULL if no exit is reachable.
 ** Note: It is not strictly necessary to null terminate that array of char, but  I recommend doing so,
 ** both for good practice and to avoid a segfault in my validator if your solution doesn't reach the exit.
 */


    if (maze->grid == NULL) return NULL;


    return path;
}

Maze *newEmptyMaze() {
    Maze *next = malloc (sizeof(Maze));
    next->height = 0;
    next->width = 0;
    next->grid = NULL;

    return next;
}
Maze *loadMaze (Maze *n, int height, int width, const char *grid) {
    Maze *new = newEmptyMaze();

    return NULL;
}

int main () {

    Maze *m1 = newEmptyMaze();
    Maze *m2 = newEmptyMaze();

    m1 = loadMaze(
        m1, 21, 41,
        "#########################################" \
        "#<    #       #     #         # #   #   #" \
        "##### # ##### # ### # # ##### # # # ### #" \
        "# #   #   #   #   #   # #     #   #   # #" \
        "# # # ### # ########### # ####### # # # #" \
        "#   #   # # #       #   # #   #   # #   #" \
        "####### # # # ##### # ### # # # #########" \
        "#   #     # #     # #   #   # # #       #" \
        "# # ####### ### ### ##### ### # ####### #" \
        "# #             #   #     #   #   #   # #" \
        "# ############### ### ##### ##### # # # #" \
        "#               #     #   #   #   # #   #" \
        "##### ####### # ######### # # # ### #####" \
        "#   # #   #   # #         # # # #       #" \
        "# # # # # # ### # # ####### # # ### ### #" \
        "# # #   # # #     #   #     # #     #   #" \
        "# # ##### # # ####### # ##### ####### # #" \
        "# #     # # # #   # # #     # #       # #" \
        "# ##### ### # ### # # ##### # # ### ### #" \
        "#     #     #     #   #     #   #   #    " \
        "#########################################" \
    );
    //testMaze(m1, m2, 1);

}
