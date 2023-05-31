#include "Plot.h"
#include "ui_Plot.h"

Plot::Plot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Plot),
    _chartView(nullptr),
    _chart(new QChart)
{
    ui->setupUi(this);

    _chart->setAnimationOptions(QChart::NoAnimation);

    _chart->setBackgroundVisible(false);

    _chartView = new QChartView(_chart);
    _chartView->setRenderHint(QPainter::Antialiasing);
    _chartView->setUpdatesEnabled(true);

    ui->Plot_Layout->addWidget(_chartView);

    QValueAxis *axisX = new QValueAxis;
    axisX->setTickCount(10);
    _chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = new QValueAxis;
    axisY->setTickCount(10);
    _chart->addAxis(axisY, Qt::AlignLeft);

    QStringList title;
    title << SPEED_TTTLE->toUtf8() <<
        ALTITUDE_TITLE->toUtf8() <<
        ACCELERO_X_TITLE->toUtf8() <<
        ACCELERO_Y_TITLE->toUtf8() <<
        ACCELERO_Z_TITLE->toUtf8() <<
        FRICTION_TITLE->toUtf8();

    for (int i=0; i<CURVE_NB; i++)
    {
        PlotLine plotLine;
        plotLine.name = title[i];
        plotLine.serie = new QLineSeries;

        plotLine.serie->attachAxis(axisX);
        plotLine.serie->attachAxis(axisY);
        plotLine.serie->setName(plotLine.name);

        _chart->addSeries(plotLine.serie);
        _lines.append(plotLine);
        _lines[i].serie->setVisible(false);

        _maxAbs.append(0);
        _maxOrd.append(0);
    }
    _chart->createDefaultAxes();
}

Plot::~Plot()
{
    delete ui;
}

void Plot::appendData(int index, QPointF point)
{
    _lines[index].serie->append(point);

    if (point.x() > _maxAbs[index])
    {
        _chart->axes(Qt::Horizontal).at(0)->setMax(point.x()*1.2);
        _maxAbs[index] = point.x();
    }
    if (point.y() > _maxOrd[index])
    {
        _chart->axes(Qt::Vertical).at(0)->setMax(point.y()*1.2);
        _maxOrd[index] = point.y();
    }
}

void Plot::enableCurve(Curve curve)
{
    _lines[curve].serie->append(0, 0);
    _lines[curve].serie->setVisible(true);
}
