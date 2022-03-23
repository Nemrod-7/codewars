#include <stdio.h>
#include <stdbool.h>

#include <stdbool.h>

bool is_winning (const char *grid, char play) {
    int diag1 = 0, diag2 = 0;

    for (int i = 0; i < 3; i++) {
        int line = 0, col = 0;
        for (int j = 0; j < 3; j++) {
            if (grid[i * 3 + j] == play) line++;
            if (grid[j * 3 + i] == play) col++;
        }
        if (line == 3 || col == 3) return true;
        if (grid[i * 3 + i] == play) diag1++;
        if (grid[2 * i + 2] == play) diag2++;
    }

    if (diag1 == 3 || diag2 == 3) return true;

    return false;
}
bool is_it_possible(const char *grid) {
    int o = 0, x = 0;

    for (int i = 0; i < 9; i++) {
      if (grid[i] == '0') o++;
      if (grid[i] == 'X') x++;
    }

    if (x < o || x - o > 1) return false;

    if (is_winning (grid, 'X') && o >= x) return false;
    if (is_winning (grid, '0') && x > o) return false;

    return true;
}
int main () {

    const char grid[] = "XXX000---";




    printf ("%d", abs (x - o));
    /*

tester("_________",  true);
tester("__0______", false);
tester("____X____",  true);

tester("XXXXXXXXX", false);
tester("XXX000___", false);

tester("0X_0X0_X_", false);
tester("0X00X0__X", false);
tester("XX_X_X000", false);

tester("0XXXX000X",  true);
tester("XXX0_0___",  true);
tester("X000X0XXX",  true);
tester("X_00X0XX0",  true);
*/

}
