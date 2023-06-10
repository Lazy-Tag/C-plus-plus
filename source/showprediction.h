#ifndef SHOWPREDICTION_H
#define SHOWPREDICTION_H

#include "qcustomplot.h"
#include "kchart.h"
#include <QCoreApplication>

namespace Ui {
class ShowPrediction;
}

class ShowPrediction : public KChart
{
    Q_OBJECT

public:
    explicit ShowPrediction(string, Date, Prediction&, QWidget *parent = nullptr);
    ~ShowPrediction();
private:
    Ui::ShowPrediction *ui;
    QCustomPlot* customplot1;
    QCustomPlot* customplot2;
    QCPItemTracer* tracer;
private slots:
    void mouseMove(QMouseEvent*);
};

#endif // SHOWPREDICTION_H
