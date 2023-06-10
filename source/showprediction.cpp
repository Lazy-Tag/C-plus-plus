#include "showprediction.h"
#include "ui_showprediction.h"
#define PATH2BACKGROUNDIMAGE "..\\C++\\source\\background.jpg"

ShowPrediction::ShowPrediction(string _name, Date _date, Prediction& pre, QWidget *parent) :
    KChart(parent),
    ui(new Ui::ShowPrediction)
{
    ui->setupUi(this);
    setMouseTracking(true);

    preDatas.clear();
    preTimes.clear();
    preHash_t.clear();

    name = _name;
    date = _date;
    Date tmp_Date = date;

    customplot1 = ui -> prediction;
    customplot2 = ui -> real;
    customplot1 -> setBackground(QPixmap(PATH2BACKGROUNDIMAGE));
    customplot1 -> setBackgroundScaledMode(Qt::IgnoreAspectRatio);
    customplot2 -> setBackground(QPixmap(PATH2BACKGROUNDIMAGE));
    customplot2 -> setBackgroundScaledMode(Qt::IgnoreAspectRatio);
    customplot1 -> xAxis -> setTickLabelRotation(60);
    customplot2 -> xAxis -> setTickLabelRotation(60);

    vector<Stock>S = pre.get_stock(name, date.get_str());
    int len = 0; ++ tmp_Date, ++ date;
    vector<Stock>N = pre.get_stock(name, date.get_str());
    queue<Stock> prediction;
    while(len <= 30)
    {
        vector<Stock> next = pre.get_stock(name, date.get_str());
        if(!next.size()) break;
        for(auto item : next) prediction.push(item);
        len += next.size();
        ++ tmp_Date;
    }

    queue<Stock> stock;
    for(auto item : S) stock.push(item);
    S = pre.get_prediction(stock, prediction);
    QVector<Stock> predict, real;
    for(auto item : S) predict.push_back(item);
    for(auto item : N) real.push_back(item);
    setupIndexDemo(customplot1, predict, true);
    setupIndexDemo(customplot2, real, false);

    tracer = new QCPItemTracer(customplot1);
    tracer -> setPen(QPen(Qt::red));
    tracer -> setBrush(QBrush(Qt::red));
    tracer -> setStyle(QCPItemTracer::tsCircle);
    tracer -> setSize(10.0);

    QObject :: connect(customplot1, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMove(QMouseEvent*)));
}

void ShowPrediction :: mouseMove(QMouseEvent* e)
{
    ui -> preTextBrowser -> clear();
    ui -> realTextBrowser -> clear();
    string info = "预测股票信息：\n股票名称: " + name + "\n日期: %1年%2月%3日\n" +
                            "开盘价: %4\n收盘价: %5\n最低价: %6\n最高价: %7\n";
    double x = customplot1->xAxis->pixelToCoord(e->pos().x());
    if(!preHash_t.count(int(x + 0.5))) return;
    int xValue = preHash_t[int(x + 0.5)];
    auto yValue = preDatas[int(x + 0.5)];
    tracer -> position -> setCoords(x, 0);

    int zValue = -1;
    for(auto item : hash_t)
        if(item.second == xValue)
        {
            zValue = item.first;
            break;
        }

    if(zValue != -1)
    {
        auto realyValue = rawDatas[zValue];
        string realInfo = "真实股票信息：\n股票名称: " + name + "\n日期: %8年%9月%10日\n" +
                "开盘价: %11\n收盘价: %12\n最低价: %13\n最高价: %14\n";
        ui -> realTextBrowser -> setText(QString(realInfo.c_str()).
                                           arg(date.year).arg(date.month).arg(hash_t[zValue]).
                                           arg(realyValue[0]).arg(realyValue[1]).arg(realyValue[2]).arg(realyValue[3]));
    }

    ui -> preTextBrowser -> setText(QString(info.c_str()).
                                        arg(date.year).arg(date.month).arg(xValue).
                                        arg(yValue[0]).arg(yValue[1]).arg(yValue[2]).arg(yValue[3]));
    customplot1->replot();
}

ShowPrediction::~ShowPrediction()
{
    delete ui;
}
