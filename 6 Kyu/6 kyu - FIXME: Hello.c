#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char *strdup (const char *);

typedef struct {
  char *name;
  int   age;
  char  sex;
  char order[4];
} Dinglemouse;

Dinglemouse *createDinglemouse() { return calloc(1, sizeof(Dinglemouse)); }
void destroyDinglemouse (Dinglemouse *dm) {
    free(dm->name);
    free(dm);
}
void setAge(Dinglemouse *dm, int age) {
    dm->age = age;
    if (!strchr(dm->order,'a')) strcat (dm->order,"a");
}
void setSex(Dinglemouse *dm, char sex) {
    dm->sex = sex;
    if (!strchr(dm->order,'s')) strcat (dm->order,"s");
}
void setName(Dinglemouse *dm, const char *name) {
    dm->name = strdup(name);
    if (!strchr(dm->order,'n')) strcat (dm->order,"n");
}
char *hello(Dinglemouse *dm) {
    char buffer[1024] = "Hello.", *ptr = buffer + 6;

    for (size_t i = 0; i < strlen (dm->order); i++)
        switch (dm->order[i]) {
            case 'a': ptr += sprintf (ptr, " I am %d.", dm->age); break;
            case 's': ptr += sprintf (ptr, " I am %s.", dm->sex == 'M' ? "male" : "female"); break;
            case 'n': ptr += sprintf (ptr," My name is %s.", dm->name); break;
    }
    return strdup (buffer);
}
