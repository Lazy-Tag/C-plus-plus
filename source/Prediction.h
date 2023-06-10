//
// Created by 胡博文 on 2022/5/20.
//

#ifndef MAIN_CPP_PREDICTION_H
#define MAIN_CPP_PREDICTION_H

#include "sharpe.h"
#include "File.h"

using namespace std;

class MainWindow;

class Prediction : public File
{
public:
    void get_diff(MainWindow*);
    void get_sharpe();
    vector<Sharpe> getstock() {return stock;}
    unordered_map<string, int> get_index() {return index; }
    vector<Stock> get_prediction(queue<Stock>, queue<Stock>);
    double predict(string, queue<Stock>);
private:
    vector<Sharpe> stock;
    unordered_map<string, int> index;
};

#endif //MAIN_CPP_PREDICTION_H
