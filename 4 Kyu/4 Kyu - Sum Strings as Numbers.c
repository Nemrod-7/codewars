#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define NUM(x) ((x) - '0')
#define CHAR(x) ((x) + '0')

struct number {
    int size;
    int *num;
};

struct number substract (struct number top, struct number down, int start);
char *strsum(const char *a, const char *b) ;

void read_data (struct number input);
void Test ();

int main () {
    time_t start = clock(), stop;

    strsum("79468954669731368983277216529249965321894906401" , "390991402387639753718");
    //printf ("%s\n",strsum("" , "5"));
    // --> sanitize <--

    //divide_strings ("53395474", "1985");
    //divide_strings ("6831174101264298785", "213248359");
    //divide_strings ("9223372036854775807", "2147483647"); // 4294967298
    //char **actual = divide_strings("729", "9");
    //printf ("divide %i\n", 10 / 2);
    //divide_strings("20", "3");
    //int next = 0, num = 53395474, den = 1985;
    //Test ();

    stop = clock();
    printf ("Process took %.06f s", (double)(stop - start) * 2 / CLOCKS_PER_SEC);

    return 0;
}

struct number convert (const char *source, int size) {

    int len = strlen (source), next = 0;
    struct number out = {.size = size, .num = calloc (size , sizeof (int))};

    size = len;
    while (size--> 0)
        out.num[next++] = NUM (source[size]);

    return out;
}
char *strsum(const char *a, const char *b) {
    int lena = strlen(a), lenb = strlen(b), size = (lena > lenb ? lena : lenb)+1;
    int i, rem = 0, num;
    char *result = malloc (size * sizeof(char));
    struct number fst = convert (a, size), sec = convert (b, size), sum = convert ("",size);

    for (i = 0 ; i < size; ++i) {
        num = fst.num[i] + sec.num[i] + rem;
        printf ("%i %i\n",fst.num[i],sec.num[i]);
        rem = num / 10;
        sum.num[i] = num % 10;
    }

    while (sum.num[--size] == 0);//size--;

    i = 0;
    if (size >= 0) {
        do {
            result[i++] = CHAR(sum.num[size]);
        } while (size-->0);
    }
    else
        result[i++] = '0';

    result[i] = '\0';
    //printf ("%s\n",result);
    return result;
}

void read_data (struct number input) {
    int index = input.size;

    while (index--> 0)
        printf ("%i", input.num[index]);

    printf ("\n");
}

void cr_assert_str_eq (char *input, char *expected, char *error) {

    if (strcmp (input, expected) < 0)
        printf ("error : expected ==> %s got ==>s %s\n",error,input);
}
void Test () {


}
