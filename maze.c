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
typedef struct _node {
    Point *data;
    struct _node *next;
} Node;
typedef struct {
    Node *root;
} Stack;

void stack_push(Stack *stack, Point *p) {

    Node *cell = stack->root;
    stack->root = malloc (sizeof (Node));
    stack->root->data = p;
    stack->root->next = cell;
}
Point *stack_pop(Stack *stack) {

    Node *cell = stack->root;
    Point *value = stack->root->data;
    stack->root = stack->root->next;

    free (cell);

    return value;
}
bool is_inside(Point p, const Maze *m) {
    return p.x >= 0 && p.y >= 0 && p.x < m->width && p.y < m->height;
}
bool is_free(Point p, const Maze *m) {
    return m->grid[p.y][p.x] == ' ';
}

Point locate (const Maze *m) {
    for (int y = 0; y < m->height; y++) {
        for (int x = 0; x < m->width; x++) {
            char cell = m->grid[y][x];

            if (cell == '>' || cell == '<' || cell =='^' || cell == 'v') {
                return (Point){x,y};
            }
        }
    }
    return (Point){-1,-1};
}
char *escape(const Maze *maze) {

    const char direct[4] = "v>^<";
    const Point compass[4] = {{0,1},{1,0},{0,-1},{-1,0}};
    char *path = NULL;

    Point p = locate(maze);
    char visit[64][64] = {{0}};

    int index = 0;
    Point stack[1024];

    stack[index++] = p;

    while(index > 0) {
        p = stack[--index];
        // printf("[%i,%i] -> %c", p.x, p.y, maze->grid[p.y][p.x]);
        if (p.x == maze->width - 1 || p.y == maze->height - 1) {

            break;
        }

        for (int i = 0; i < 4; i++) {
            Point nxp = {p.x + compass[i].x, p.y + compass[i].y};

            if (is_inside(nxp, maze) && is_free(nxp, maze) && !visit[nxp.y][nxp.x]) {
                visit[nxp.y][nxp.x] = direct[i];
                stack[index++] = nxp;
            }
        }
    }

    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            if (!visit[y][x]) {
                printf(" ");
            } else {
                printf("%c", visit[y][x]);
            }
        }
        printf("\n");
    }

    return path;
}

Maze *newEmptyMaze() {
    Maze *next = malloc (sizeof(Maze));
    next->height = 0;
    next->width = 0;
    next->grid = NULL;

    return next;
}
Maze *loadMaze (Maze *next, int height, int width, const char *grid) {

    next->height = height;
    next->width = width;
    next->grid = malloc(height * sizeof(char*));

    for (int y = 0; y < height; y++) {
        next->grid[y] = malloc(width * sizeof(char));

        for (int x = 0; x < width; x++) {
            next->grid[y][x] = grid[y * width + x];
        }
    }
    return next;
}
Maze *deleteMaze (Maze *maze) {
    maze->height = maze->width = 0;
    free(maze->grid);
    return NULL;
}
Maze *clearMaze (Maze *maze){

}
Maze *resizeMaze (Maze *maze, unsigned height, unsigned width) {

}

void testMaze(Maze *m, Maze *mCopy, int solvable) {
    escape(m);
}

void Test () {
    Maze        *m1;
    Maze        *m2;

    m1 = newEmptyMaze();
    m2 = newEmptyMaze();
    m1 = loadMaze(
        m1, 3, 3,
        "# #" \
        " > " \
        "# #" \
    );
    testMaze(m1, m2, 1);

    m1 = loadMaze(
        m1, 3, 10,
        "##########" \
        "#>       #" \
        "######## #" \
    );
    testMaze(m1, m2, 1);

    m1 = loadMaze(
        m1, 3, 11,
        "# #########" \
        "#        >#" \
        "###########" \
    );
    testMaze(m1, m2, 1);

    m1 = loadMaze(
        m1, 3, 11,
        "# #########" \
        "#    ^    #" \
        "######### #" \
    );
    testMaze(m1, m2, 1);

    m1 = loadMaze(
        m1, 4, 9,
        "####### #" \
        "#>#   # #" \
        "#   #   #" \
        "#########" \
    );
    testMaze(m1, m2, 1);

    m1 = loadMaze(
        m1, 8, 10,
        "##########" \
        "#        #" \
        "#  ##### #" \
        "#  #   # #" \
        "#  #^# # #" \
        "#  ### # #" \
        "#      # #" \
        "######## #" \
    );
    testMaze(m1, m2, 1);

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
    testMaze(m1, m2, 1);

    m1 = loadMaze(
        m1, 9, 9,
        "  #####  " \
        " #     # " \
        "#       #" \
        "#       #" \
        "#   ^   #" \
        " #     # " \
        "  #   #  " \
        "   # #   " \
        "    #    " \
    );
    testMaze(m1, m2, 0);

    deleteMaze(m1);
    deleteMaze(m2);
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
    testMaze(m1, m2, 1);

    printf("\nend\n");

}
