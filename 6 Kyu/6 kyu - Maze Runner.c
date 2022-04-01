#include <stddef.h>
#include <string.h>

enum {floor, wall, start, end};

extern char *strdup (const char *);

char *maze_runner (const int *maze, size_t n, const char *directions) {

    const int size = n * n;
    int index = size, curr;
    _Bool running = 1;

    while (index--> 0)
        if (maze[index] == start) curr = index;
  
    index = 0;

    while (running) {
        char ins = directions[index];

        switch (ins) {
            case 'N' : curr -= n; break;
            case 'S' : curr += n; break;
            case 'E' : curr += curr % n == n - 1 ? size : 1; break;
            case 'W' : curr -= curr % n == 0 ? size : 1; break;        }

        if (curr < 0 || curr >= size) return strdup ("Dead");
        if (maze[curr] == wall) return strdup ("Dead");
        if (maze[curr] == end) return strdup ("Finnish");

        if (directions[index++] == '\0') running = 0;
    }

    return strdup ("Lost");
}
