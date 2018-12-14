#include "std_lib_facilities.h"

class Date {
    int y, m, d;
public:
    Date (int y, int m, int d);
    void add_day (int n);
    int year() const {return y;}
    int month() const {return m;}
    int day() const {return d;}
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

Date::Date (int y0, int m0, int d0) {
    if (y0 <= 1752 || y0 > 2080)
        error("The revised Julian and Gregorian calendars differ");
    if (m0 < 1 || m0 > 12) error("Invalid month");
    if (d0 < 1 || d0 > days(m0,y0)) error("Invalid day");
    d = d0;
    m = m0;
    y = y0;
}

void Date::add_day (int n) {
    if (n < -500 || n > 500) error("Excessive lapse of time");
    if (n < 0)
        for (int i = 0; i < -n; ++i) {
            --d;
            if (d < 1) {
                --m;
                if (m < 1) {
                    --y;
                    if (y <= 1752) error("Decremented out of range");
                    m = 12;
                }
                d = days(m,y);
            }
        }
    else
        for (int i = 0; i < n; ++i) {
            ++d;
            if (d > days(m,y)) {
                ++m;
                if (m > 12) {
                    ++y;
                    if (y >= 2080) error("Incremented out of range");
                    m = 1;
                }
                d = 1;
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
    ordinal(os,dd.day());
    os << " day of the ";
    ordinal(os,dd.month());
    os << " month in the year of Our Lord " << dd.year() << endl;
    return os;
}

int main() {
    Date today(1978,6,25);
    Date tomorrow = today;
    tomorrow.add_day(1);
    cout << "Today = " << today;
    cout << "Tomorrow = " << tomorrow;
}
