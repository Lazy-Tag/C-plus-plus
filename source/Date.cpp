//
// Created by 胡博文 on 2022/5/9.
//

#include "Date.h"

using namespace std;

bool Date :: operator<(const Date& t) const
{
    if(t.year != year) return year < t.year;
    if(t.month != month) return month < t.month;
    return day < t.day;
}

void Date ::show()
{
    printf("%d-%d-%d ", year, month, day);
}

void Date::setdate(const string &_str)
{
    year = atoi(_str.substr(0, 4).c_str());
    month = atoi(_str.substr(4, 2).c_str());
    day = atoi(_str.substr(6, 2).c_str());
}

void Date :: operator++ ()
{
    if(month != 12) month ++ ;
    else month = 1, year ++ ;
}

string Date :: get_str()
{
    string str = to_string(year);
    if(month < 10) str += '0';
    str += to_string(month);
    return str;
}
