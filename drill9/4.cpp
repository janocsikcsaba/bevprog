#include "std_lib_facilities.h"

class Date {
public:
    enum Month {
        January = 1, February, March, April, May, June, July, August,
        September, October, November, December
    };
    Date (int y, Month m, int d);
    void add_day (int n);
    int year() const {return y;}
    Month month() const {return m;}
    int day() const {return d;}
private:
    int y, d;
    Month m;
};

int days (Date::Month m, int y) {
    switch (m) {
case Date::February:
        if (y%4 == 0 && (y%100 != 0 || y%400 == 0))
            return 29;
        else
            return 28;
case Date::April: case Date::June: case Date::September:
case Date::November:
        return 30;
        break;
default:
        return 31;
        break;
    }
}

Date::Date (int y0, Date::Month m0, int d0) {
    if (y0 <= 1752 || y0 > 2080)
        error("The revised Julian and Gregorian calendars differ");
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
                m = Month(int(m)-1);    // Ugh
                if (m < January) {
                    --y;
                    if (y <= 1752) error("Decremented out of range");
                    m = December;
                }
                d = days(m,y);
            }
        }
    else
        for (int i = 0; i < n; ++i) {
            ++d;
            if (d > days(m,y)) {
                m = Month(int(m)+1);    // Ugh
                if (m > December) {
                    ++y;
                    if (y >= 2080) error("Incremented out of range");
                    m = January;
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

static ostream & operator<< (ostream & os, const Date::Month & mon) {
    switch (mon) {
case Date::January: os << "January"; break;
case Date::February: os << "February"; break;
case Date::March: os << "March"; break;
case Date::April: os << "April"; break;
case Date::May: os << "May"; break;
case Date::June: os << "June"; break;
case Date::July: os << "July"; break;
case Date::August: os << "August"; break;
case Date::September: os << "September"; break;
case Date::October: os << "October"; break;
case Date::November: os << "November"; break;
case Date::December: os << "December"; break;
     }
     return os;
}
 
ostream & operator<< (ostream & os, const Date & dd) {
    os << "This ";
    ordinal(os,dd.day());
    os << " day of " << dd.month() <<
        " in the year of Our Lord " << dd.year() << endl;
    return os;
}

int main() {
    Date today(1978,Date::June,25);
    Date tomorrow = today;
    tomorrow.add_day(1);
    cout << "Today = " << today;
    cout << "Tomorrow = " << tomorrow;
}
