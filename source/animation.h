#ifndef ANIMATION_H
#define ANIMATION_H

#include <QWidget>
#include "showsharpe.h"
#include "qcustomplot.h"

namespace Ui {
class Animation;
}

class Animation : public ShowSharpe
{
    Q_OBJECT

public:
    explicit Animation(Prediction&, QVector<double>, QWidget *parent = nullptr);
    ~Animation();

private:
    Ui::Animation *ui;
    Date date, last;
    Prediction pre;
    QTimer* timer;
    QVector<double> shp;
private slots:
    void Draw();
};

#endif // ANIMATION_H
