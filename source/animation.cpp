#include "animation.h"
#include "ui_animation.h"
#define PATH2BACKGROUNDIMAGE "..\\C++\\source\\background.jpg"

Animation::Animation(Prediction& _pre, QVector<double> _shp, QWidget *parent) :
    ShowSharpe(parent),
    ui(new Ui::Animation)
{
    ui->setupUi(this);
    customplot = ui -> widget;
    date.setdate("20000901");
    last.setdate("20220101");
    pre = _pre, shp = _shp;
    timer = new QTimer(this);
    timer -> setInterval(10000000);
    timer -> start(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(Draw()));
    connect(ui -> Run, SIGNAL(clicked(bool)), timer, SLOT(start()));
    connect(ui -> Pause, SIGNAL(clicked(bool)), timer, SLOT(stop()));
}

void Animation :: Draw()
{
    paint(customplot, ui -> textBrowser, date.year, date.month, pre, shp);
    ++ date;
    if(date == last) date.setdate("20000901");
    Sleep(1000);
    customplot -> replot();
}

Animation::~Animation()
{
    delete ui;
}
