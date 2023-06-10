#ifndef SHOWSHARPE_H
#define SHOWSHARPE_H

#include <QWidget>
#include "qcustomplot.h"
#include "Prediction.h"

namespace Ui {
class ShowSharpe;
}

class ShowSharpe : public QWidget
{
    Q_OBJECT

public:
    ShowSharpe(QWidget *parent = nullptr);
    explicit ShowSharpe( int, int, Prediction&, QVector<double>&, QWidget *parent = nullptr);
    ~ShowSharpe();
    QCPItemTracer* tracer;
    friend class Animation;

protected:
    Ui::ShowSharpe *ui;
    QCustomPlot *customplot;
    QVector<double> ticks;
    QVector<double> shp;
    QVector<QString> labels;
    QCPItemText* tracerLabel;
    int idx = 0;
    void paint(QCustomPlot*, QTextBrowser*, int, int, Prediction&, QVector<double>&);

protected slots:
    void mouseMove(QMouseEvent*);
};

#endif // SHOWSHARPE_H

