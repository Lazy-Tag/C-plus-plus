//
// Created by 胡博文 on 2022/5/9.
//

#include "Stock.h"
#include <string>

using namespace std;

Stock :: Stock() {}

Stock :: Stock(const string& _str)
{
    string date;
    char s1[20], s2[20];
    str = _str;
    sscanf(_str.c_str(),
           "%9s,%8s,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
           s1, s2, &opening_price, &max_price, &min_price,
           &closing_price, &near_closing_price, &change, &change_percent, &num, &exchange_price);
    name = s1;
    date = s2;
    datetime.setdate(date);
}

Stock :: Stock(const string& nam, const string& date, double op, double cp, double maxp, double minp, int n)
{
    name = nam, datetime.setdate(date);
    opening_price = op, closing_price = cp;
    max_price = maxp, min_price = minp, num = n;
}

bool Stock ::operator<(const Stock &t) const
{
    if(t.name == name) return datetime < t.datetime;
    return name < t.name;
}

void Stock :: show()
{
    printf("%s ", name.c_str());
    datetime.show();
    printf("%.2lf %.2lf %.2lf %.2lf %.2lf", opening_price, closing_price, max_price, min_price, num);
    puts("");
}

double Stock :: get_info(const string str)
{
    if(str == "opening price") return opening_price;
    else if(str == "closing price") return closing_price;
    else if(str == "max price") return max_price;
    else if(str == "number") return num;
    else return min_price;
}

