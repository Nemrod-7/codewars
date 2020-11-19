#include <string.h>

int from_roman (char * source) {

    int index = strlen (source), num = 0;
    short int dig, dec ,cen;
    dig = dec = cen = 1;

    while (index-->0) {
       char val = source[index];
       switch (val) {
           case 'M': num += 1000;      cen = -1;       break;
           case 'D': num += 500;       dec = cen = -1; break;
           case 'C': num += 100 * cen; dig = dec = -1; break;
           case 'L': num += 50;        dig = dec = -1; break;
           case 'X': num += 10 * dec;  dig = -1;       break;
           case 'V': num += 5;         dig = -1;       break;
           case 'I': num += 1 * dig;                   break;
       }
    }
    return num;
}
void to_roman (int n, char *dest) {
    int index = 0;
    int dec[] = {1000,900,500,400,100,90,50,40,10,9,5,4,1};
    char roman[][3] = {"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};

    while (n) {
        if (n >= dec[index]) {
            strcat (dest,roman[index]);
            n -= dec[index];
        }
        else
          index++;
    }
}
