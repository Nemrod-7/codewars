#include <stdlib.h>
#include <string.h>

typedef struct State {
    char *id;
    struct State *zero, *one;
    int out;
} State;

typedef struct Fsm {
    State **data;
    size_t size;
} Fsm;

State *search (const Fsm *fsm, const char *src) {

    for (size_t i = 0; i < fsm->size; i++)
        if (strcmp (fsm->data[i]->id, src) == 0)
            return fsm->data[i];
     return NULL;
}
State **init (size_t size) {
    State **next = malloc (size * sizeof (State));
    while (size-->0)
        next[size] = malloc (sizeof (State));
    return next;
}
State **set (char *base[][4], size_t size) {

  State **data = init (size);

  for (size_t i = 0; i < size; i++) {
      data[i]->id = base[i][0];

      for (size_t j = 0; j < size; j++) {
          if (strcmp (base[i][1], base[j][0]) == 0)
              data[i]->zero = data[j];
          if (strcmp (base[i][2], base[j][0] ) == 0)
              data[i]->one = data[j];
      }
      data[i]->out = atoi (base[i][3]);
  }

  return data;
}
Fsm *compile(const char *instructions) {
    char *src = strdup (instructions), *token = strtok (src, " ,;\n");
    Fsm *fsm = malloc (sizeof (Fsm));

    size_t size = 0, idx = 0;
    char *base[10][4] = {{0}};

    while (token) {

        base[size][idx++] = token;

        if (idx == 4)
           idx = 0, size++;

        token = strtok (NULL, " ,;\n");
    }
    //free (src);
    fsm->data = set (base, size);
    fsm->size = size;
    return fsm;
}

int run_fsm(const Fsm *fsm, const char *start, const _Bool *input, const size_t len_input,
            const char **final_state, const char **path) {

    State *now = search (fsm, start);
    int index = 0;
    path[index++] = now->id;

    for (size_t i = 0; i < len_input; i++) {
        now = input[i] == 0 ? now->zero : now->one;
        path[index++] = now->id;
    }

    *final_state = now->id;

    return now->out;
}

void free_fsm(Fsm *fsm) {
    int index = fsm->size;
    while (index-->0)
        free (fsm->data[index]);

    free(fsm);
}
