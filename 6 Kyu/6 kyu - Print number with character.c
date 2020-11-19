#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int numbers[10][6] = {
    {0,1,1,1,1,0},{2,3,4,2,2,5},{0,1,6,2,7,5},{0,1,6,6,1,0},
    {1,1,1,8,9,9},{5,10,11,9,9,11},{6,2,0,1,1,0},{5,1,6,2,7,7},
    {0,1,0,0,1,0},{0,1,1,0,2,7}};

const char patterns2[12][7] = {
    " **** ","**  **","  **  "," ***  ","* **  ","******",
    "   ** "," **   "," *****","    **","**    ","***** "};

const char border[] = "****************************************\n";
const char empty[] =  "*                                      *\n";

void replace (char *src, char ch) {
      do {
        if (*src == '*')
            *src = ch;
      } while (*src++);
}
char *print_number(unsigned num, char ch) {

    char *output = malloc (409 * sizeof(char)), *out = output;
    char digits[6];
    sprintf (digits, "%05i", num);

    out += sprintf (out, "%s%s",border, empty);

    for (int y = 0; y < 6; ++y) {
        out += sprintf (out, "*  ");
        for (int x = 0; x < 5; ++x) {
            out += sprintf (out, "%s ", patterns2[numbers[digits[x] - '0'][y]]);
        }
        out += sprintf (out, " *\n");
    }
    out += sprintf (out, "%s%s",empty, border);

    output[409] = '\0';
    replace (output,ch);
    //printf ("%c %i\n",output[409]);
    return output;
}


int main () {

    int num = 456;
    char digits[6];
    sprintf (digits, "%05i", num);

    printf ("%s", print_number(num, '$'));

}
