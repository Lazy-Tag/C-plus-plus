#include "mainwindow.h"
#include "ui_mainwindow.h"
#define PATH2OUTPUT_FILE "..\\C++\\source\\file\\output.txt"

using namespace  std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
        ui -> setupUi(this);
        connect(ui ->pushButton, &QPushButton :: clicked, this, &MainWindow :: ShowStock);
        connect(ui -> pushButton_2, &QPushButton :: clicked, this, &MainWindow :: ShowSharpe);
        connect(ui -> pushButton_3, &QPushButton :: clicked, this, &MainWindow :: PredictNextMonth);
        connect(ui -> pushButton_4, &QPushButton :: clicked, this, &MainWindow :: DateSet);
        connect(ui -> pushButton_5, &QPushButton :: clicked, this, &MainWindow :: Animate);

        flag.open(PATH2OUTPUT_FILE);
        if(flag)
        {
            pre.set_index();
            pre.get_sharpe();
        }
        flag.close();
        ui -> progressBar -> setMaximum(100000);
        ui -> progressBar -> setValue(0);
}

void MainWindow :: DateSet()
{
    ui -> progressBar -> setValue(0);
    pre.write(this);
    pre.Sort(this);
    pre.get_diff(this);
    pre.set_index();
    pre.get_sharpe();
    ui -> progressBar -> setValue(100000);
}

void MainWindow :: PredictNextMonth()
{
    Date date;
    string name = ui -> LineX -> text().toStdString();
    date.setdate(ui -> LineY -> text().toStdString());
    ShowPrediction *w =  new ShowPrediction(name, date, pre);
    w -> show();
}

void MainWindow :: ShowStock()
{
    string name = ui -> LineX -> text().toStdString();
    date.setdate(ui -> LineY -> text().toStdString());
    KChart *w = new KChart(name, date, pre);
    w -> show();
}

void MainWindow :: ShowSharpe()
{
    int year = ui -> LineYear -> text().toInt();
    int month = ui -> LineMonth -> text().toInt();
    class ShowSharpe *w = new class ShowSharpe(year, month, pre, shp);
    w -> show();
}

void MainWindow :: Animate()
{
       Animation *w = new Animation(pre, shp);
       w -> show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

