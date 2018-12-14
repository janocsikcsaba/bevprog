#include "std_lib_facilities.h"

struct Date {
    int y, m, d;
};

int days (int m, int y) {
    switch (m) {
case 2:
        if (y%4 == 0 && (y%100 != 0 || y%400 == 0))
            return 29;
        else
            return 28;
case 4: case 6: case 9: case 11:
        return 30;
        break;
default:
        return 31;
        break;
    }
}

void init_day (Date & dd, int y, int m, int d) {
    if (y <= 1752 || y > 2080)
        error("The revised Julian and Gregorian calendars differ");
    if (m < 1 || m > 12) error("Invalid month");
    if (d < 1 || d > days(m,y)) error("Invalid day");
    dd.d = d;
    dd.m = m;
    dd.y = y;
}

void add_day (Date & dd, int n) {
    if (n < -500 || n > 500) error("Excessive lapse of time");
    if (n < 0)
        for (int i = 0; i < -n; ++i) {
            --dd.d;
            if (dd.d < 1) {
                --dd.m;
                if (dd.m < 1) {
                    --dd.y;
                    if (dd.y <= 1752) error("Decremented out of range");
                    dd.m = 12;
                }
                dd.d = days(dd.m,dd.y);
            }
        }
    else
        for (int i = 0; i < n; ++i) {
            ++dd.d;
            if (dd.d > days(dd.m,dd.y)) {
                ++dd.m;
                if (dd.m > 12) {
                    ++dd.y;
                    if (dd.y >= 2080) error("Incremented out of range");
                    dd.m = 1;
                }
                dd.d = 1;
            }
        }
}

void ordinal (ostream & os, int n) {
    os << n;
    if (n%10 == 1)
        os << "st";
    else if (n%10 == 2)
        os << "nd";
    else if (n%10 == 3)
        os << "rd";
    else
        os << "th";
}

ostream & operator<< (ostream & os, const Date & dd) {
    os << "This ";
    ordinal(os,dd.d);
    os << " day of the ";
    ordinal(os,dd.m);
    os << " month in the year of Our Lord " << dd.y << endl;
    return os;
}
Date today;

int main() {
    init_day(today,1978,6,25);
    Date tomorrow = today;
    add_day(tomorrow,1);
    cout << "Today = " << today;
    cout << "Tomorrow = " << tomorrow;
}
