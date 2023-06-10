#include "kchart.h"
#include "ui_kchart.h"
#define PATH2BACKGROUNDIMAGE "..\\C++\\source\\background.jpg"

class MyAxisTickerText : public QCPAxisTickerText
{
protected:
    virtual QVector<double> createTickVector(double tickStep, const QCPRange &range) Q_DECL_OVERRIDE
    {
        Q_UNUSED(tickStep)
        QVector<double> result;
        if (mTicks.isEmpty())
            return result;

        auto start = mTicks.lowerBound(range.lower);
        auto end = mTicks.upperBound(range.upper);
        if (start != mTicks.constBegin()) --start;
        if (end != mTicks.constEnd()) ++end;

        int count = cleanMantissa(std::distance(start, end) / double(mTickCount + 1e-10));

        auto it = start;
        while (it != end) {
            result.append(it.key());
            int step = count;
            if (step == 0) ++it;
            while (--step >= 0 && it != end)
                ++it;
        }

        return result;
    }
};

KChart::KChart(QWidget *parent) : QWidget(parent), ui(new Ui::KChart){}

KChart::KChart(string _name, Date _date, Prediction& pre, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KChart)
{
    ui -> setupUi(this);
    customplot = ui -> widget;
    this -> setMouseTracking(true);

    name = _name, date = _date;
    customplot -> setBackground(QPixmap(PATH2BACKGROUNDIMAGE));
    customplot -> setBackgroundScaledMode(Qt::IgnoreAspectRatio);
    customplot -> xAxis -> setTickLabelRotation(60);
    connect(customplot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMove(QMouseEvent*)));

    vector<Stock>S = pre.get_stock(name, date.get_str());
    QVector<Stock> res;
    for(auto item : S) res.push_back(item);
    setupIndexDemo(customplot, res, false);
    customplot -> addGraph();

    tracer = new QCPItemTracer(customplot);
    tracer -> setPen(QPen(Qt::red));
    tracer -> setBrush(QBrush(Qt::red));
    tracer -> setStyle(QCPItemTracer::tsCircle);
    tracer -> setSize(10.0);
}

void KChart :: mouseMove(QMouseEvent *e)
{
    ui -> textBrowser -> clear();
    string info = "股票名称: " + name + "日期: %1年%2月%3日\n"
                                   "开盘价: %4\n收盘价: %5\n最低价: %6\n最高价: %7\n";
    double x = customplot->xAxis->pixelToCoord(e->pos().x());
    if(!hash_t.count(int(x + 0.5))) return;
    int xValue = hash_t[int(x + 0.5)];
    auto yValue = rawDatas[int(x + 0.5)];
    double y = (yValue[2] + yValue[3]) / 2;

    tracer -> position -> setCoords(x, y);
    ui -> textBrowser -> setText(QString(info.c_str())
                                       .arg(date.year).arg(date.month).arg(xValue).
                                 arg(yValue[0]).arg(yValue[1]).arg(yValue[2]).arg(yValue[3]));
    customplot->replot();
}

void KChart ::setupIndexDemo(QCustomPlot *customPlot, QVector<Stock> &S, bool isPrediction)
{
    rawTimes.clear();
    rawDatas.clear();
    hash_t.clear();
    QColor BrushPositive("#ec0000");
    QColor PenPositive("#8a0000");
    QColor BrushNegative("#00da3c");
    QColor PenNegative("#008f28");

    int cnt = 0;
    for(auto item : S)
    {
        string str = to_string(item.datetime.year) + "\\" +
                to_string(item.datetime.month) + "\\" + to_string(item.datetime.day) ;
        hash_t[cnt ++ ] =  item.datetime.day;
        QString s = str.c_str();
        rawTimes.append(s);
        QVector<double>tmp;
        tmp.push_back(item.opening_price);
        tmp.push_back(item.closing_price);
        tmp.push_back(item.min_price);
        tmp.push_back(item.max_price);
        rawDatas.push_back(tmp);
    }

    if(isPrediction)
    {
        preDatas = rawDatas;
        preTimes = rawTimes;
        preHash_t = hash_t;
    }

    QSharedPointer<QCPAxisTickerText> textTicker(new MyAxisTickerText);
    textTicker->setTickCount(10);
    QCPDataContainer<QCPFinancialData> datas;
    QVector<double> timeDatas, MA5Datas, MA10Datas, MA20Datas, MA30Datas;

    MA5Datas = calculateMA(rawDatas, 5);
    MA10Datas = calculateMA(rawDatas, 10);
    MA20Datas = calculateMA(rawDatas, 20);
    MA30Datas = calculateMA(rawDatas, 30);

    for (int i = 0; i < rawTimes.size(); i ++ )
    {
        timeDatas.append(i);

        QCPFinancialData data;
        data.key = i;
        data.open = rawDatas.at(i).at(0);
        data.close = rawDatas.at(i).at(1);
        data.low = rawDatas.at(i).at(2);
        data.high = rawDatas.at(i).at(3);
        datas.add(data);

        textTicker->addTick(i, rawTimes.at(i));
    }

    QCPFinancial *financial = new QCPFinancial(customPlot->xAxis, customPlot->yAxis);
    financial->setName("日K");
    financial->setBrushPositive(BrushPositive);
    financial->setPenPositive(PenPositive);
    financial->setBrushNegative(BrushNegative);
    financial->setPenNegative(PenNegative);
    financial->data()->set(datas);

    const QVector<QColor> ColorOptions = {
        "#c23531", "#2f4554", "#61a0a8", "#d48265"
    };

    QCPGraph *graph = customPlot->addGraph();
    graph->setName("MA5");
    graph->setData(timeDatas, MA5Datas);
    graph->setPen(ColorOptions.at(0));
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(ColorOptions.at(0), 2), QBrush(Qt::white), 8));

    graph = customPlot->addGraph();
    graph->setName("MA10");
    graph->setData(timeDatas, MA10Datas);
    graph->setPen(ColorOptions.at(1));
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(ColorOptions.at(1), 2), QBrush(Qt::white), 8));

    graph = customPlot->addGraph();
    graph->setName("MA20");
    graph->setData(timeDatas, MA20Datas);
    graph->setPen(ColorOptions.at(2));
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(ColorOptions.at(2), 2), QBrush(Qt::white), 8));

    graph = customPlot->addGraph();
    graph->setName("MA30");
    graph->setData(timeDatas, MA30Datas);
    graph->setPen(ColorOptions.at(3));
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(ColorOptions.at(3), 2), QBrush(Qt::white), 8));

    customPlot->xAxis->setTicker(textTicker);
    customPlot->rescaleAxes();
    customPlot->xAxis->scaleRange(1.05, customPlot->xAxis->range().center());
    customPlot->yAxis->scaleRange(1.05, customPlot->yAxis->range().center());
    customPlot->legend->setVisible(true);
}

QVector<double> KChart :: calculateMA(const QVector<QVector<double> > &v, int dayCount)
{
    auto func = [](double result, const QVector<double> &v2){
      return result + v2[1];
    };

    QVector<double> result;
    for (int i = 0; i < v.size(); ++i)
    {
        if (i < dayCount) result.append(qQNaN());
        else
        {
            double sum = std::accumulate(v.begin() + i - dayCount + 1, v.begin() + i + 1, 0.0, func);
            result.append(sum / dayCount);
        }
    }
    return result;
}

KChart::~KChart()
{
    delete ui;
}
