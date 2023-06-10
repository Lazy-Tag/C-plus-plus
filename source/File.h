//
// Created by 胡博文 on 2022/5/9.
//

#ifndef FILE_H
#define FILE_H

#include <string>
#include <fstream>
#include <queue>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <algorithm>
#include <cstring>
#include "direct.h"
#include "Stock.h"

using namespace std;

class MainWindow;

class File
{
public:
    void write(MainWindow*);
    void Sort(MainWindow*);
    vector<Stock> get_stock(string);
    void set_index();
    long long get_index(string, string);
    vector<Stock> get_stock(string, string);
protected:
    vector<string> name;
    priority_queue<pair<Stock, int>, vector<pair<Stock, int>>, greater<pair<Stock, int>>> heap;
    unordered_map<string, long long> index;
};

#endif //C__PROJECT_FILE_H
