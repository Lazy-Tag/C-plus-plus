//
// Created by 胡博文 on 2022/5/20.
//

#include "sharpe.h"

Sharpe::Sharpe(const string _str)
{
    string date;
    char s1[20], s2[20];
    str = _str;
    sscanf(_str.c_str(),"%9s,%8s%lf", s1, s2, &eps);
    name = s1, date = s2;
    datetime.setdate(date);
}

Sharpe :: Sharpe(const string _str, const vector<double> diff)
{
    string date;
    char s1[20], s2[20];
    str = _str;
    sscanf(_str.c_str(),"%9s,%8s,%lf", s1, s2, &eps);
    name = s1, date = s2;
    datetime.setdate(date);
    double sum = 0, n = diff.size();
    for(auto i : diff) sum += i;
    mean = sum / n, sum = 0;
    for(auto i : diff) sum += (i - mean) * (i - mean);
    std = sqrt(sum / n);
    shp = mean / std;
    if(shp > 10) shp = -1e9;
}

bool Sharpe :: operator<(const Sharpe& t) const
{
    if (datetime == t.datetime) return shp > t.shp;
    return datetime < t.datetime;
}

void Sharpe :: show()
{
    cout << name << ' ';
    datetime.show();
    cout << mean << ' ' << std << ' ' <<shp << endl;
}
