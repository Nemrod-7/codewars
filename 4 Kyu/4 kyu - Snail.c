#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include <string.h>
#include <time.h>

typedef struct _point {
  int x, y;
} point;

const point coord[4] = {{0,1},{1,0},{0,-1},{-1,0}};

bool is_inside (size_t height, size_t width, point p) {
    return (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height);
}
int *snail(size_t *outsz, const int **mx, size_t rows, size_t cols) {

    bool grd[rows][cols];
    int *output = malloc (rows * cols * sizeof (int));
    size_t pos = 0, size = 0, index = rows * cols;
    point p = {0,0}, next;

    memset(grd, 0, index * sizeof(bool));

    while (index-->0) {
        next.x = p.x + coord[pos].x, next.y =  p.y + coord[pos].y;

        if (!is_inside (rows, cols, next) || grd[next.x][next.y])
            pos += (pos != 3 ? 1 : -3);

        output[size++] = mx[p.x][p.y];
        grd[p.x][p.y] = true;

        p.x += coord[pos].x, p.y += coord[pos].y;
    }

    *outsz = size;
    return output;
}

int main () {
  time_t start = clock(), stop;


  Test();

  stop = clock();
  printf ("\nProcess took %.06f s", (double)(stop - start) * 2 / CLOCKS_PER_SEC);
  return 0;
}
/*
int **init_matrix (int size) {
    int it = 1;
    int **grid = malloc (size * sizeof(int));
    for (int i = 0; i < size; ++i) {
        grid[i] = malloc (size * sizeof(int));
        for (int j = 0; j < size ; ++j)
            grid[i][j] = it++;
    }

    return grid;
}
*/
////////////////////////////////////////////////////////////////////////////////
#define DIMLEN      0x4
#define FMT_INT     (CHAR_BIT * sizeof(int) / 3 + 1)
#define FMT_ALIGN   0x2
#define FMT_MX      ((FMT_ALIGN + (FMT_INT + FMT_ALIGN) * DIMLEN) * DIMLEN + 1)

typedef enum {
    ASSERT_PASS,
    ASSERT_FAIL
} assertop;

int **ia_to_imx(int **mx, const int *a, size_t rows, size_t cols) {
    while (rows--)
        *(mx+rows) = (int *)a + rows * cols;
    return mx;
}
char *ia_to_s(char *s, const int *a, size_t n, const char *sep){
    const char *p = sep ? sep : ", ";
    size_t pos;
    if (!a) {
        pos = sprintf(s, "NULL");
        return s;
    }
    pos = sprintf(s, "[ %i", *a++);
    while (--n)
        pos += sprintf(s+pos, "%s%i", p, *a++);
    sprintf(s+pos, " ]");
    return s;
}
char *imx_to_s(char *s, const int **mx, size_t nrows, size_t ncols) {
    size_t pos, i, j;
    for (pos = i = 0; i < nrows; ++i) {
        pos += sprintf(s+pos, "[ ");
        for (j = 0; j < ncols; ++j)
            pos += sprintf(s+pos, "%i, ", mx[i][j]);
        if (j)
            pos -= 2;
        pos += sprintf(s+pos, " ]\n");
    }
    if (i)
        --pos;
    *(s+pos) = '\0';
    return s;
}
assertop assert_mem_eq(const void *actual, const void *expected, size_t n, size_t size) {
   size_t i;
    if (!actual || !expected)
        return actual != expected ? ASSERT_FAIL : ASSERT_PASS;
    for (i = 0ul; n--; i += size)
        if (memcmp ((char *)actual+i, (char *)expected+i, size))
            return ASSERT_FAIL;
    return ASSERT_PASS;
}
void assert_data(const int *mx, size_t m, size_t n, const int *expected_snail, size_t expected_outsz) {
    char outpf[3][FMT_MX];
    int *pmx[DIMLEN];
    size_t actual_outsz;
    int *actual_snail = snail(&actual_outsz, ia_to_imx(pmx, mx, m, n), m, n);

    if (!actual_snail || actual_outsz != expected_outsz || assert_mem_eq(actual_snail, expected_snail, expected_outsz, sizeof(int)) == ASSERT_FAIL)
        printf("[*Actual*]\n  Outsz: %lu\n  Snail: %s\n[Expected]\n  Outsz: %lu\n  Snail: %s\nMatrix:\n%s\n",
            actual_outsz,  ia_to_s(outpf[0], actual_snail, expected_outsz, 0),
            expected_outsz, ia_to_s(outpf[1], expected_snail, expected_outsz, 0),
            imx_to_s(outpf[2], (const int **)pmx, m, n));
    //else
    //    cr_assert(1);
    if (actual_snail)
        free(actual_snail);
}
Test() {
    assert_data((const int[]){ 1,2,3, 4,5,6, 7,8,9 }, 3ul, 3ul, (const int[]){ 1,2,3,6,9,8,7,4,5 }, 9ul);
    assert_data((const int[]){ 1,2,3, 8,9,4, 7,6,5 }, 3ul, 3ul, (const int[]){ 1,2,3,4,5,6,7,8,9 }, 9ul);
    assert_data((const int[]){ 1,2,3,1, 4,5,6,4, 7,8,9,7, 7,8,9,7 }, 4ul, 4ul, (const int[]){ 1,2,3,1,4,7,7,9,8,7,7,4,5,6,9,8 }, 16ul);
}
