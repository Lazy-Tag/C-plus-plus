#ifndef KCHART_H
#define KCHART_H

#include <QWidget>
#include <algorithm>
#include "qcustomplot.h"
#include "Prediction.h"

namespace Ui {
class KChart;
}

class KChart : public QWidget
{
    Q_OBJECT

public:
    explicit KChart(string, Date, Prediction&, QWidget *parent = nullptr);
    KChart(QWidget *parent = nullptr);
    ~KChart();

private:
    Ui::KChart *ui;
    QCustomPlot *customplot;
    QCPItemTracer* tracer;
protected:
    string name;
    Date date;
    QVector<QString> rawTimes;
    QVector<QVector<double>> rawDatas;
    QVector<QString> preTimes;
    QVector<QVector<double>> preDatas;
    unordered_map<int, int> preHash_t;
    unordered_map<int, int> hash_t;
protected slots:
    void setupIndexDemo(QCustomPlot* , QVector<Stock>&, bool);
    QVector<double> calculateMA(const QVector<QVector<double>>&, int);
    void mouseMove(QMouseEvent*);
};

#endif // KCHART_H
