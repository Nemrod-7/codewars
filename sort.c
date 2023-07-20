#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void Test ();

int factorial (int n) {
    int mul = 1;
    for (int i = 1; i <= n; i++) {
        mul *= i;
    }
    return mul;
}

void swap (unsigned char *a, unsigned char *b) {
    unsigned char temp = *a;
    *a = *b;
    *b = temp;
}
unsigned char exchange_sort (size_t size, const unsigned char seq[]) {

    unsigned char cnt = 0;
    unsigned char *arr = (unsigned char*) malloc (size * sizeof(unsigned char));
    memcpy (arr, seq, size * sizeof (unsigned char));

    for (int i = 0; i < size - 1; i++) {
        for (int j = size - 1; j > i; j--) {
            if (arr[j] < arr[i]) {
                swap (&arr[i], &arr[j]);
                cnt++;
            }
        }

        for (int j = 0; j < size; j++) {
            printf("%i", arr[j]);
        }
        printf("\n");
    }
   printf (" nperm %i ", cnt);

    return cnt;
}

int main () {

    const unsigned char seq[12] = {9,9,9,7,7,8,9,7,8,9,7,9};
    // [7][8][9];
    //  4  2  6
    // 7 000110010010
    // 8 000001001000
    // 9 111000100101
    int freq[3] = {0};
    int n = 12;

    int seven = 0, eight = 0, nine = 0;

    for (int i = 0; i < n; i++) {
        switch (seq[i]) {
            case 7: seven |= 1 << i; break;
            case 8: eight |= 1 << i; break;
            case 9: nine  |= 1 << i; break;
        }
        freq[seq[i] - 7]++;
    }
    int res = nine ^  seven;
    for (int i = 0; i < n; i++) {
        printf ("%i", res >> i&1);
    }

    // printf ("%i", res);

    for (int i = 0; i < 3; i++) {
        // printf ("%i => %i\n", i+7, freq[i]);
    }



    // const unsigned char seq[12] = {7,9,9,7,7,8,9,7,8,9,9,9};
    // const unsigned char seq[12] = {7,8,9,7,7,8,9,7,9,9,9,9};
    // const unsigned char seq[12] = {7,8,7,7,7,8,9,9,9,9,9,9};
    // const unsigned char seq[12] = {7,7,7,7,8,8,9,9,9,9,9,9};


    // unsigned char expected = 4;
    // exchange_sort (12, seq);
    //tester(12, seq, expected);

    // Test();
   // [7, 7, 8, 8, 9, 9], the result should be 0.
   // It's already a sorted seq.
   // For seq = [9, 7, 8, 8, 9, 7], the result should be 1.
   // We can switching seq[0] and seq[5].

   // For seq = [8, 8, 7, 9, 9, 9, 8, 9, 7], the result should be 4.

}


void tester(size_t length, const unsigned char seq[], unsigned char expected) {
    static int cnt;
    unsigned char submitted = exchange_sort(length, seq);

    cnt++;
    if (submitted != expected) {
        printf ("error: test %i\n", cnt);
    }
}


void Test() {
  {
    const unsigned char seq[6] = {7, 7, 8, 8, 9, 9};
    unsigned char expected = 0;
    tester(6, seq, expected);
  }
  {
    const unsigned char seq[6] = {9, 7, 8, 8, 9, 7};
    unsigned char expected = 1;
    tester(6, seq, expected);
  }
  {
    const unsigned char seq[9] = {8, 8, 7, 9, 9, 9, 8, 9, 7};
    unsigned char expected = 4;
    tester(9, seq, expected);
  }
  {
    const unsigned char seq[15] = {9, 9, 9, 9, 9, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7};
    unsigned char expected = 6;
    tester(15, seq, expected);
  }
  {
    const unsigned char seq[12] = {9, 9, 9, 7, 7, 8, 9, 7, 8, 9, 7, 9};
    unsigned char expected = 4;
    tester(12, seq, expected);
  }
  {
    const unsigned char seq[6] = {9, 9, 7, 7, 8, 8};
    unsigned char expected = 4;
    tester(6, seq, expected);
  }
  {
    const unsigned char seq[3] = {9, 7, 9};
    unsigned char expected = 1;
    tester(3, seq, expected);
  }
  {
    const unsigned char seq[3] = {8, 7, 8};
    unsigned char expected = 1;
    tester(3, seq, expected);
  }
  {
    const unsigned char seq[4] = {7, 8, 7, 8};
    unsigned char expected = 1;
    tester(4, seq, expected);
  }
  {
    const unsigned char seq[4] = {8, 8, 7, 8};
    unsigned char expected = 1;
    tester(4, seq, expected);
  }
  {
    const unsigned char seq[5] = {8, 8, 7, 7, 8};
    unsigned char expected = 2;
    tester(5, seq, expected);
  }
}


int maxperm (const int *arr) { // n permutations = n! / (n1! . n2! . n...!)
    const int size = sizeof (arr) / sizeof(arr[0]);
    int freq[10] = {0}, nperm = factorial(size);

    for (int i = 0; i < size; i++) {
       freq[arr[i]]++;
    }

    for (int i = 0; i < 10; i++) {
        nperm /= factorial (freq[i]);
    }

    return nperm;
}
