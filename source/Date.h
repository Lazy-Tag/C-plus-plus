//
// Created by 胡博文 on 2022/5/9.
//

#ifndef C__PROJECT_DATE_H
#define C__PROJECT_DATE_H

#include <string>

using namespace std;

class Date {
public:
    void setdate(const string& str);
    string get_info(){return to_string(year) + to_string(month); }
    string get_str();
    int get_year() { return year; }
    bool operator<(const Date&) const;
    void operator ++();
    bool operator==(const Date &t) const
    { return month == t.month && year == t.year;}
    void show();
    friend class ShowPrediction;
    friend class MainWindow;
    friend class Stock;
    friend class Sharpe;
    friend class Prediction;
    friend class KChart;
    friend class Animation;
protected:
    int year, month, day;
};


#endif //C__PROJECT_DATE_H
