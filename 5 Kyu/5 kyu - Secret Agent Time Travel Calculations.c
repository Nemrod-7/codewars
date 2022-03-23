#include <stdlib.h>
#include <stdbool.h>

struct date {
    int days;
    int months;
    int years;
};

bool isleapyear (int year) {

    if (year > 1752)
        if ((year % 400 == 0) || ( ( year % 100 != 0) && (year % 4 == 0)))
            return true;

    if (year <= 1752)
        if (year % 4 == 0)
            return true;


    return false;
}
bool destination (struct date now, struct date dest) {

    if (now.days == dest.days && now.months == dest.months && now.years == dest.years)
        return true;

    return false;
}
int lastday (struct date actual) {

    switch (actual.months) {
        case 1 : return 31;
        case 2 : return isleapyear (actual.years) ? 29 : 28;
        case 3 : return 31;
        case 4 : return 30;
        case 5 : return 31;
        case 6 : return 30;
        case 7 : return 31;
        case 8 : return 31;
        case 9 : return 30;
        case 10 : return 31;
        case 11 : return 30;
        case 12 : return 31;
    }
}
int days (int date, int month, int year) {

    struct date past = {date, month, year}, actual = {24, 3, 2437};
    bool travel = !destination (past, actual);
    int time = 0, minus = 0;

    while (travel) {

        if (past.days > lastday (past)) {
            past.months++;
            past.days = 1;

            if (past.months > 12) {
                past.years++;
                past.months = 1;
            }

        }
        if (past.years == 1752)
            if (past.months == 9)
                if (past.days > 2 && past.days < 14)
                    minus++;

        past.days++, time++;
        travel = !destination (past, actual);
    }

    return time - minus;
}
