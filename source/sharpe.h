//
// Created by 胡博文 on 2022/5/20.
//

#ifndef MAIN_CPP_SHARPE_H
#define MAIN_CPP_SHARPE_H

#include "Date.h"
#include "Stock.h"
#include <vector>
#include <cmath>
#include <iostream>

class Sharpe
{
public:
    Sharpe(const string);
    Sharpe(const string, const vector<double>);
    bool operator<(const Sharpe&) const;
    bool operator==(const Sharpe& t) const {return datetime == t.datetime && shp == t.shp; }
    double get_diff(){return eps; }
    string get_name() {return name; }
    string get_info(){ return name + datetime.get_info(); }
    double get_shp(){return  shp; }
    void show();
protected:
    string name, str;
    Date datetime;
    double eps, mean, std, shp;
};


#endif //MAIN_CPP_SHARPE_H
