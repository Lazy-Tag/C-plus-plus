//
// Created by 胡博文 on 2022/5/20.
//

#include "Prediction.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

#define PATH2OUTPUT_FILE  "..\\C++\\source\\file\\output.txt"
#define PATH2DIFFERENCE_FILE  "..\\C++\\source\\file\\diff.txt"

void Prediction :: get_diff(MainWindow* w)
{
    fstream fout, fdiff;
    fout.open(PATH2OUTPUT_FILE, ios :: in);
    fdiff.open(PATH2DIFFERENCE_FILE, ios :: out);
    string str, last;
    int cnt = 0;
    while(!fout.eof())
    {
        fout >> str;
        if(last.substr(0, 16) == str.substr(0, 16))
        {
            Stock yesterday(last);
            Stock today(str);
            double s1 = yesterday.get_info("closing price"), s2  = today.get_info("closing price");
            double eps = (s1 - s2) / s2;
            fdiff <<str.substr(0, 18)  + to_string(eps) << endl;
            cnt ++;
            if(!(cnt % 130000))
            {
                w -> time += 1500;
                w -> ui -> progressBar -> setValue(w -> time);
                QApplication :: processEvents();
            }
        }
        last = str;
    }
    fout.close(), fdiff.close();
    QApplication :: processEvents();
}

void Prediction :: get_sharpe()
{
    fstream file;
    file.open(PATH2DIFFERENCE_FILE, ios :: in);
    string str, last;
    bool flag = true;
    while(!file.eof())
    {
        vector<double> diff;
        while(!file.eof())
        {
            if(flag)file >> str;
            if(last.size() && last.substr(0,16) != str.substr(0, 16))
            {
                flag = false;
                last = str;
                break;
            }
            Sharpe tmp(str);
            diff.push_back(tmp.get_diff());
            flag = true;
            last = str;
        }
        Sharpe tmp(str, diff);
        stock.push_back(tmp);
    }
    file.close();
    sort(stock.begin(), stock.end());
    stock.erase(unique(stock.begin(), stock.end()), stock.end());
    for(int i = 0; i < stock.size(); i ++ ) index[stock[i].get_info().substr(9, 6)] = i;
}

double Prediction :: predict(string option, queue<Stock> q)
{
    vector<double> nums;
    queue<Stock> tmp = q;
    while(tmp.size())
    {
        auto t = tmp.front();
        tmp.pop();
        nums.push_back(t.get_info(option));
    }
    q = tmp;
    int len = nums.size();
    double sumx = 0, sumy = 0, sumxy = 0, sumx2 = 0;
    for(int i = 0; i < nums.size(); i ++ )
    {
        sumx += i;
        sumy += nums[i];
        sumxy += i * nums[i];
        sumx2 += i * i;
    }
    double b = (len * sumxy - sumx * sumy) / (len * sumx2 - sumx * sumx);
    return nums[len - 1] + b;
}

vector<Stock> Prediction :: get_prediction(queue<Stock> q, queue<Stock> p)
{
    vector<Stock>S;
    Stock s = q.front();
    int idx = 0;
    int year = s.datetime.year;
    int month = s.datetime.month;
    if(month == 12) year ++ , month = 1;
    else month ++ ;
    for(int i = 1; i <= 30; i ++ )
    {
        Stock s = q.front();
        int day = i;
        int n = predict("number", q);
        double op = predict("opening price", q);
        double cp = predict("closing price", q);
        double maxp = predict("max price", q);
        double minp = predict("min price", q);
        string date = to_string(year);
        if(month < 10) date += '0';
        date += to_string(month);
        if(day < 10) date += '0';
        date += to_string(day);
        Stock tmp(s.name, date, op, cp, maxp, minp, n);
        q.pop(),S.push_back(tmp);
        if(!p.size()) q.push(S[idx ++ ]);
        else q.push(p.front()), p.pop();
    }
    return S;
}

