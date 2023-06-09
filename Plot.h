#ifndef PLOT_H
#define PLOT_H

#include <QWidget>

#include <QVBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include <QVector>
#include <QPointF>

namespace
{
    Q_GLOBAL_STATIC_WITH_ARGS(const QString, SPEED_TTTLE, ("Speed"));
    Q_GLOBAL_STATIC_WITH_ARGS(const QString, ALTITUDE_TITLE, ("Altitude"));
    Q_GLOBAL_STATIC_WITH_ARGS(const QString, ACCELERO_X_TITLE, ("Accelero_X"));
    Q_GLOBAL_STATIC_WITH_ARGS(const QString, ACCELERO_Y_TITLE, ("Accelero_Y"));
    Q_GLOBAL_STATIC_WITH_ARGS(const QString, ACCELERO_Z_TITLE, ("Accelero_Z"));
    Q_GLOBAL_STATIC_WITH_ARGS(const QString, FRICTION_TITLE, ("Trainée"));
}

enum Curve
{
    SPEED,
    ALTITUDE,
    ACCELERO_X,
    ACCELERO_Y,
    ACCELERO_Z,
    FRICTION,
    CURVE_NB
};

struct PlotLine
{
    QLineSeries *serie;

    QString name;
};

namespace Ui {
class Plot;
}

class Plot : public QWidget
{
    Q_OBJECT

public:
    explicit Plot(QWidget *parent = nullptr);
    ~Plot();

    void appendData(int index, QPointF point);

    void enableCurve(Curve curve);

private:
    Ui::Plot *ui;
    QChartView *_chartView;
    QChart *_chart;
    QVector<PlotLine> _lines;

    QVector<int> _maxAbs;
    QVector<int> _maxOrd;
};

#endif // PLOT_H
