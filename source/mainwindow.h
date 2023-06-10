#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "Prediction.h"
#include "showprediction.h"
#include "showsharpe.h"
#include "animation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void init();
    ~MainWindow();
    friend class File;
    friend class Prediction;
private:
    Ui::MainWindow *ui;
    Prediction pre;
    int time = 0;
    ifstream flag;
    Date date;
    QVector<double> shp;
private slots:
    void DateSet();
    void ShowStock();
    void ShowSharpe();
    void PredictNextMonth();
    void Animate();
};
#endif // MAINWINDOW_H
