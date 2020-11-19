#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFSIZE 1024

#define NORTH '^'
#define WEST '<'
#define EAST '>'
#define SOUTH 'v'

#define MARK 'x'
#define SPACE ' '

#define IS_BODY(x) ((x) == EAST || (x) == WEST || (x) == NORTH || (x) == SOUTH)

#define CELL(cell) (*(*(maze->grid + cell.x) + cell.y))
#define IS_FREE(cell) (CELL(cell.p) == SPACE)

typedef unsigned short int value;
typedef enum {REAR = -2,LEFT,FORW,RIGHT,BACK} position;

typedef struct s_Maze {
    unsigned height, width;
    char** grid;
} Maze;
typedef struct _point {
    int x, y;
} Point;
typedef struct _cell {
    position state;
    Point p;
} Cell;
typedef struct _node {
    char *data;
    struct _node *next;
} Node;

Node base, *path = &base;

Maze* copy (const Maze *maze) {

    Maze *new = newMaze (maze->height,maze->width);

    for (size_t i = 0; i < maze->height; i++)
        for (size_t j = 0; j < maze->width; j++)
            new->grid[i][j] = maze->grid[i][j];

    return new;
}
Node *newnode (Node *link, char *data) {

    Node *next = link;

    link = malloc (sizeof (Node));
    link->data = data;
    link->next = next;

    return link;
}
Cell get_cell (Point p, const Maze *maze) {

    position direction;
    switch (CELL(p)) {
        case EAST: direction = RIGHT; break;
        case WEST: direction = LEFT; break;
        case NORTH: direction = FORW; break;
        case SOUTH: direction = BACK; break;
    }
    Cell curr = {.state = direction, .p = p };

    return curr;
}
Cell locate_body (Maze *maze) {

    const value endx = maze->height, endy = maze->width;
    Cell origin;

    for (size_t i = 0; i < endx; i++)
        for (size_t j = 0; j < endy; j++) {
            Point p = {i,j};
            if (IS_BODY (CELL (p)))
                origin = get_cell (p,maze);

        }

    return origin;
}
char *getsign(Cell prev, Cell next) {

    position orientation = (next.state) - (prev.state);

    if (orientation == -3) orientation = RIGHT;
    if (orientation == 3) orientation = LEFT;

    switch (orientation) {
        case FORW : return "F"; break;
        case RIGHT: return "RF"; break;
        case LEFT : return "LF"; break;
        case BACK : return "BF"; break;
        case REAR : return "BF"; break;
    }
}
_Bool is_insideMaze (Maze *maze, Cell curr) {

    return (curr.p.x == 0 || curr.p.x == (maze->height - 1) ||
            curr.p.y == 0 || curr.p.y == (maze->width - 1)) ? true : false;
}
_Bool search_exit (Maze *maze, Cell curr) {

    Point now = curr.p;
    Cell left = {.state = LEFT,now.x, now.y - 1}, right = {.state = RIGHT,now.x, now.y + 1};
    Cell top = {.state = FORW, now.x - 1, now.y}, back = {.state = BACK,now.x + 1, now.y};

    if (is_insideMaze (maze,curr)) return true;

    CELL(now) = MARK;

        if (IS_FREE (left) && search_exit (maze,left)) {
            path = newnode (path, getsign (curr,left));
            return true;
        }

        if (IS_FREE (right) && search_exit (maze,right)){
            path = newnode (path, getsign (curr,right));
            return true;
        }

        if (IS_FREE (top) && search_exit (maze,top)){
            path = newnode (path, getsign (curr,top));
            return true;
        }

        if (IS_FREE (back) && search_exit (maze,back)){
            path = newnode (path, getsign (curr,back));
            return true;
        }

    CELL(now) = SPACE;

    return false;
}
char *escape(const Maze *maze) {

    //printMaze (maze,"mm");
    Maze *map = copy (maze);

    char buffer[BUFFSIZE] = {"\0"}, *result; // path = newnode (path,getsign (curr,right));
    Cell body = locate_body (map);
    _Bool escape = search_exit (map,body);

    if (escape == false) return NULL;

    while (path->data != NULL) {
        strcat (buffer,path->data);
        path = path->next;
    }
    //free (map);
    //printMaze (maze,"m1");
    //printf ("\n%s\n",buffer);
    result = malloc ((strlen (buffer) + 1 )* sizeof (char));

    return strcpy(result, buffer);
}
