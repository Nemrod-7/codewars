#include <iostream>

struct date { int day,month,year; };

bool isleapyear (int &year) {

    if (year > 1752)
        if ((year % 400 == 0) || ( ( year % 100 != 0) && (year % 4 == 0)))
            return true;

    if (year <= 1752)
        if (year % 4 == 0)
            return true;

    return false;
}
int lastday (date &actual) {
    const int nday[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};

    if (actual.month == 2)
        return isleapyear (actual.year) ? 29 : 28;
    return nday[actual.month];
}
bool destination (date &curr, date &dest) {

    if (curr.day == dest.day && curr.month == dest.month && curr.year == dest.year) {
        return true;
    }
    return false;
}
int main () {

    int time = 0; // 01 01 1900 monday january 1900

    const char *days[7] = {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
    const char *months[13] = {"","January","February","March","April","May","June","July","August","September","October","November","December"};

    bool travel = true;
    date now = {1,1,1900}, dest = {31,12,2000};
    int cnt = 0;

    while (travel) {

        if (now.day > lastday (now)) {
            now.month++;
            now.day = 1;

            if (now.month > 12) {
                now.year++;
                now.month = 1;
            }
        }

        if (now.year > 1900) {
            if (now.day == 1 && time % 7 == 6) {
                cnt++;
            }
        }

        if (destination (now, dest)) {
            travel = false;
        } else {
            now.day++;
            time++;
        }
    }

    std::cout << cnt;
    //printf ("%s %i %s %i\n", days[time%7], now.day, months[now.month], now.year);
}
