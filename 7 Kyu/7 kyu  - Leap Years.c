#include <stdbool.h>

bool IsLeapYear(int year) {

    if (year % 4 == false && year % 100 != false || year % 400 == false)
        return true;

    return false;
}
