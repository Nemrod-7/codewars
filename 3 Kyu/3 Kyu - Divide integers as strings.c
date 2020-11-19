#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define BUFFSIZE 64
#define MAX_DIGIT 1000
#define NUM(x) ((x) - '0')
#define CHAR(x) ((x) + '0')

struct number {
    int size;
    int *num;
};

char **divide_strings(char *a, char *b) ;
struct number convert (char *source, int size) ;
struct number substract (struct number top, struct number down, int start);
bool is_sup (struct number top, struct number down, int start);

void read_data (struct number input);
void Test ();

int string_to_number (char *s) {

   int num = 0;

   for (int i = 0; s[i] != '\0'; ++i)
      num = num * 10 + (s[i] - '0');

    return num;
}

int main () {
    time_t start = clock(), stop;


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

bool is_sup (struct number top, struct number down, int start) {

    int posa = top.size, posb = down.size, cnt = 0;
    //printf ("size  %i %i\n",posa, posb - start);

    if (posa == 1) {
        //printf ("num %i %i\n",down.num[posb - 1], top.num[posa - 1]);
        return (top.num[posa - 1] >= down.num[posb - 1]) ? true : false;
    }

    if (posa == posb - start)
        while (down.num[--posb] >= top.num[--posa]) {
            //if (down.num[posb] > top.num[posa])
                return false;

          //  posa--, posb--;
        }

  //  if (cnt > 0) return false;

    return true;
}
struct number substract (struct number source, struct number down, int start) {

    struct number out = {.size = source.size, .num = malloc (source.size * sizeof (int))};
    int i = 0, j = start, sub;
    bool rem = 0;

    for (; i < source.size; ++i, ++j) {
        sub = source.num[i] - ((j < down.size) ?  down.num[j] + rem : rem);
        if (sub < 0) {
            rem = 1;
            sub = 10 + sub;
        }
        else
            rem = 0;

        out.num[i] = sub;
    }

    //printf ("size1  %i \n",out.size);
    while (out.num[--i] == 0)
        out.size--;

    if (out.size < 0) out.size = 0;
    //printf ("size2  %i \n",out.size);
    return out;
}
struct number convert (char *source, int size) {

    int len = strlen (source), next = 0;
    struct number out = {.size = size, .num = malloc (size * sizeof (int))};

    //printf ("%i %i\n",len,size);
    while (size--> 0)
        out.num[next++] = (size >= len) ? 0 : NUM (source[size]);
        //printf ("%i", NUM (source[len]));

    //read_data (out);
    return out;
}
char *format (struct number input) {

    int index = input.size, next = 0;
    char *out = malloc (index * sizeof (char));

    if (index > 0) {
        while (input.num[--index] == 0)
            ;
        index++;

        while (index--> 0)
            out[next++] = CHAR (input.num[index]);
    }

    if (next == 0)
        out[next++] = '0';

    out[next] = '\0';
        //printf ("\n");
    printf ("%s\n",out);
    return out;
}
char **divide_strings (char *a, char *b) {

    int lena = strlen (a), lenb = strlen (b), len = (lena - lenb);

    struct number rem = convert (a, lena), denom = convert (b, lena), quot = convert ("",len + 1);
    int index = 20, pos = 0;
    bool numera;
    char **output = malloc (2 * sizeof (char *));
    //read_data (rem);
    while (rem.size >= lenb) {
    //while (index-->0) {
        numera = is_sup (rem, denom, pos);
        if (numera == false) {
            if (rem.size == lenb)
                break;

            pos++;
        }
        read_data (rem), read_data (denom);
        rem = substract (rem, denom, pos);
        quot.num[len - pos]++;
    }
    read_data (rem);
    output[0] = format (quot), output[1] = format (rem);

    return output;
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

  char **actual = divide_strings("0", "5");
  cr_assert_str_eq(*actual, "0", "0 / 5 == 0");
  cr_assert_str_eq(*(actual + 1), "0", "0 \x25 5 == 0");
  free(*actual);
  free(*(actual + 1));
  free(actual);
  actual = divide_strings("4", "5");
  cr_assert_str_eq(*actual, "0", "4 / 5 == 0");
  cr_assert_str_eq(*(actual + 1), "4", "4 \x25 5 == 4");
  free(*actual);
  free(*(actual + 1));
  free(actual);
  actual = divide_strings("10", "2");
  cr_assert_str_eq(*actual, "5", "10 / 2 == 5");
  cr_assert_str_eq(*(actual + 1), "0", "10 \x25 2 == 0");
  free(*actual);
  free(*(actual + 1));
  free(actual);
  actual = divide_strings("20", "3");
  cr_assert_str_eq(*actual, "6", "20 / 3 == 6");
  cr_assert_str_eq(*(actual + 1), "2", "20 \x25 3 == 2");
  free(*actual);
  free(*(actual + 1));
  free(actual);
  actual = divide_strings("60", "5");
  cr_assert_str_eq(*actual, "12", "60 / 5 == 12");
  cr_assert_str_eq(*(actual + 1), "0", "60 \x25 5 == 0");
  free(*actual);
  free(*(actual + 1));
  free(actual);
  actual = divide_strings("219", "11");
  cr_assert_str_eq(*actual, "19", "219 / 11 == 19");
  cr_assert_str_eq(*(actual + 1), "10", "219 \x25 11 == 10");
  free(*actual);
  free(*(actual + 1));
  free(actual);
  actual = divide_strings("729", "9");
  cr_assert_str_eq(*actual, "81", "729 / 9 == 81");
  cr_assert_str_eq(*(actual + 1), "0", "729 \x25 9 == 0");
  free(*actual);
  free(*(actual + 1));
  free(actual);
  actual = divide_strings("1000", "10");
  cr_assert_str_eq(*actual, "100", "1000 / 10 == 100");
  cr_assert_str_eq(*(actual + 1), "0", "1000 \x25 10 == 0");
  free(*actual);
  free(*(actual + 1));
  free(actual);
  actual = divide_strings("600001", "100");
  cr_assert_str_eq(*actual, "6000", "600001 / 100 == 6000");
  cr_assert_str_eq(*(actual + 1), "1", "600001 \x25 100 == 1");
  free(*actual);
  free(*(actual + 1));
  free(actual);
}
