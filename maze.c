#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct s_Maze {
    unsigned    height, width;
    char**      grid;
} Maze;

/*
** @param maze a valid Maze* (see description) with consistent height, width and grid.
** Note: maze->grid[0][0] is the top left-hand corner and maze->grid[maze->height - 1][maze->width - 1] is the bottom right-hand corner.
** @return a heap-allocated string composed of the instructions 'F', 'R', 'B' and 'L'. NULL if no exit is reachable.
** Note: It is not strictly necessary to null terminate that array of char, but  I recommend doing so,
** both for good practice and to avoid a segfault in my validator if your solution doesn't reach the exit.
*/
char *escape(const Maze *maze) {
    char *path = NULL;
    printf ("Maze(w: %u, h: %u):\n", maze->width, maze->height);

    if (maze->grid == NULL) return NULL;

    for (unsigned int i = 0; i < maze->height; ++i) {
      printf("\"%s\"\n", maze->grid[i]);
    }
    printf("\n");
    // Remember that path must be heap-allocated (otherwise you can't safely return it)
    path = (char*)calloc(strlen(my_answer) + 1, sizeof(char));
    return path;
}
