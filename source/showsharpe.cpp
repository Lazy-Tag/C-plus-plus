#include "showsharpe.h"
#include "ui_showsharpe.h"
#define PATH2BACKGROUNDIMAGE "..\\C++\\source\\background.jpg"

ShowSharpe::ShowSharpe(QWidget *parent) : QWidget(parent), ui(new Ui::ShowSharpe){}

ShowSharpe::ShowSharpe(int year, int month, Prediction& pre, QVector<double>& _shp, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowSharpe)
{
    ui -> setupUi(this);
    customplot = ui -> widget;
    shp = _shp;

    tracer = new QCPItemTracer(customplot);
    tracer -> setPen(QPen(Qt::red));
    tracer -> setBrush(QBrush(Qt::red));
    tracer -> setStyle(QCPItemTracer::tsCircle);
    tracer -> setSize(10.0);

    connect(customplot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMove(QMouseEvent*)));

    paint(customplot, ui -> textBrowser, year, month, pre, shp);
}

void ShowSharpe :: paint(QCustomPlot* customplot, QTextBrowser* textBrowser, int year, int month, Prediction& pre, QVector<double>& shp)
{
    shp.clear(), ticks.clear(), labels.clear();
    customplot -> clearPlottables();
    customplot -> addGraph();
    customplot -> setBackground(QPixmap(PATH2BACKGROUNDIMAGE));
    customplot -> setBackgroundScaledMode(Qt::IgnoreAspectRatio);

    QCPAxis *keyAxis = customplot->xAxis;
    QCPAxis *valueAxis = customplot->yAxis;
    QCPBars *sharpe = new QCPBars(keyAxis, valueAxis);
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);

    textBrowser -> clear();
    textBrowser -> setText(QString("%1年%2月").arg(year).arg(month));

    sharpe -> setAntialiased(false);
    sharpe -> setName("Sharpe in month");
    sharpe -> setPen(QPen(QColor(0, 168, 140).lighter(130)));
    sharpe -> setBrush(QColor(0, 168, 140));

    keyAxis -> setTickLabelRotation(60);
    keyAxis -> setSubTicks(false);
    keyAxis -> setTickLength(0, 4);
    keyAxis -> setTicker(textTicker);
    keyAxis -> setLabel("Stock Date");

    valueAxis -> setLabel("Sharpe");
    valueAxis -> setRange(-0.6, 0.6);
    valueAxis -> setPadding(35);

    string date = to_string(year) + to_string(month);
    auto index = pre.get_index();
    auto stock = pre.getstock();
    idx = index[date];
    for(int i = idx + 1; i <= idx + 30; i ++ )
    {
        shp.push_back(stock[i].get_shp());
        ticks << i - idx;
        labels << stock[i].get_name().c_str() ;
        textTicker -> addTicks(ticks, labels);
        customplot -> rescaleAxes();
        sharpe -> setData(ticks, shp);
    }
}

void ShowSharpe :: mouseMove(QMouseEvent* e)
{
    double x = customplot->xAxis->pixelToCoord(e->pos().x());
    int xValue = x + 0.5 ;
    if(xValue >= ticks.size() || xValue <= 0) return ;
    double yValue = shp[xValue - 1];
    string name = labels[xValue - 1].toStdString();
    tracer -> position -> setCoords(xValue, yValue);
    string info = "股票名称:" + name + "\nSharpe指数: %1";
    ui -> Sharpe -> setText(QString(info.c_str()).arg(yValue));

    customplot -> replot();
}

ShowSharpe::~ShowSharpe()
{
    delete ui;
}

