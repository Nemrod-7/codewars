#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef enum {WEST = -2,SOUTH,CENTER,NORTH,EAST} direction;

char** init (char **src, int size) {

    char** new = malloc (size * sizeof (char*));

    for (int i = 0; i < size; ++i) {
        new[i] = malloc (6 * sizeof (char));
        strcpy (new[i],src[i]);
    }
    return new;
}
direction get (char entry) {

  switch (entry) {
      case 'N' : return NORTH;
      case 'S' : return SOUTH;
      case 'E' : return EAST;
      case 'W' : return WEST;
      default : return 0;
  }

}
void reduce (char **arr, int end, int pos) {

    int next = pos, i = ((pos + 2) < end) ? (pos + 2) : end;

    for (; i < end; ++i) {
        strcpy (arr[next++], arr[i] );
    }
}
char** dirReduc (char** arr, int sz, int* lg) {

    int i = 0, size = sz;
    char **map = init (arr, size);

    while (i < size) {

        direction now = get (*map[i]);
        direction nxt = ((i + 1) < size) ? get (*map[i + 1]) : now;

        if ((nxt + now) == CENTER) {
            reduce (map, size, i);
            size -= 2, i = 0 ;
        }
        else
            i++;
    }

    *lg = size;
    return map;
}
