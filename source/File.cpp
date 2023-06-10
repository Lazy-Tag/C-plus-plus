//
// Created by 胡博文 on 2022/5/9.
//
#include "File.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

#define PATH2SOURCE_FILE "..\\C++\\source\\file\\source_file.csv"
#define PATH2WRITTEN_FILE "..\\C++\\source\\file\\process file\\file"
#define PATH2OUTPUT_FILE "..\\C++\\source\\file\\output.txt"
#define  MAX_DATA   4e5

void File :: write(MainWindow* w)
{
    fstream file;
    file.open(PATH2SOURCE_FILE, ios :: in);
    string str;
    getline(file, str);
    string path = PATH2WRITTEN_FILE ;
    for(int filename = 1; ; filename ++ )
    {
        fstream f;
        name.push_back(to_string(filename));
        f.open(path + to_string(filename), ios :: out);
        bool flag = true;
        vector<Stock> S;
        for(int i = 0; i < MAX_DATA; i ++ )
        {
            string str;
            file >> str;
            if(str.size() > 8) S.push_back(Stock(str));
            if(file.eof())
            {
                flag = false;
                break;
            }
        }
        sort(S.begin(), S.end());
        for(auto item : S) f << item.get_str() << endl;
        if(!flag) break;
        w -> time += 2100;
        w -> ui -> progressBar -> setValue(w -> time);
        QApplication :: processEvents();
        f.close();
    }
    file.close();
}

vector<Stock> File :: get_stock(string file_name)
{
    vector<Stock> S;
    fstream f;
    string str;
    f.open(PATH2WRITTEN_FILE + file_name, ios :: in);
    while(!f.eof())
    {
        getline(f, str);
        if(str.size() < 8) break;
        Stock temp_stock(str);
        S.push_back(temp_stock);
    }
    f.close();
    return S;
}

void File :: Sort(MainWindow* w)
{
    fstream fin[11], fout;
    string sin;
    bool st[11];
    st[0] = true;
    int flag = 0, cnt = 0;
    fout.open(PATH2OUTPUT_FILE, ios :: out);
    for(int i = 1; i <= name.size(); i ++ )
    {
        fin[i].open(PATH2WRITTEN_FILE + to_string(i), ios :: in);
        fin[i] >> sin;
        heap.push({Stock(sin), i});
    }
    fout << endl;
    while(cnt < 10)
    {
        if(!st[flag])
        {
            fin[flag] >> sin;
            if(sin.size() < 5 || fin[flag].eof())
            {
                st[flag] = true;
                fin[flag].close(), cnt ++ ;
                w -> time += 3500;
                w -> ui -> progressBar -> setValue(w -> time);
            }
            if(sin.size() > 5) heap.push({Stock(sin), flag});
        }
        auto sout = heap.top();
        heap.pop();
        fout << sout.first.get_str() << '\n';
        flag = sout.second;
    }
    fout.close();
}

void File :: set_index()
{
    fstream  F;
    string path = PATH2OUTPUT_FILE;
    F.open(path, ios :: in);
    long long idx = 0;
    int eps = 0;
    string str = "";
    while(!F.eof())
    {
        idx  = F.tellg() ;
        eps ++ ;
        F >> str;
        if(!index.count(str.substr(0, 16)))
            index[str.substr(0, 16)] = idx - eps, eps = 0;
    }
    F.close();
}

vector<Stock> File :: get_stock(string name, string date)
{
    fstream file;
    string id = name + ',' + date;
    string str;
    vector<Stock> stock;
    if(!index.count(id)) return stock;
    long long idx = index[id];
    file.open(PATH2OUTPUT_FILE, ios :: in);
    file.clear();
    file.seekg(idx, ios :: beg);
    file >> str;
    while(!file.eof())
    {
        file >> str;
        if(!(str.substr(0, 16) ==id)) break;
        Stock tmp(str);
        stock.push_back(tmp);
    }
    file.close();
    return stock;
}
