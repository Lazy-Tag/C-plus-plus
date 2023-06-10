//
// Created by 胡博文 on 2022/5/9.
//

#ifndef C__PROJECT_STOCK_H
#define C__PROJECT_STOCK_H

#include "Date.h"

using namespace std;

class Stock
{
public:
    Stock();
    Stock(const string& );
    Stock(const string& , const string &, double, double, double, double, int);
    bool operator<(const Stock& t) const;
    string get_str(){return str; }
    void show();
    double get_info(string);
    string get_info() { return name + datetime.get_info();}
    friend class MainWindow;
    friend class Prediction;
    friend class KChart;
protected:
    Date datetime ;//交易日期
    string str;
    string name;//股票代码
    double opening_price, closing_price, near_closing_price;//开盘价，收盘价
    double max_price, min_price;//最高价，最低价
    double change, change_percent;
    double num, exchange_price;// 交易量
};

#endif //C__PROJECT_STOCK_H
