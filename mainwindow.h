#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPointF>
#include <QVector>
#include <QGraphicsTextItem>

#include "worldmanager.h"
#include "graphicviewsurcharged.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class WorldManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void fitIn();

    void on_pushButton_pause_clicked();

    void on_horizontalSlider_simSpeed_valueChanged(int value);

    void on_pushButton_resetSpeed_clicked();

    void on_horizontalSlider_accelAngle_valueChanged(int value);

    void on_horizontalSlider_accelForce_valueChanged(int value);

    void on_actionIncrease_Rocket_Force_triggered();

    void on_actionDecrease_Rocket_Force_triggered();

    void on_actionRun_triggered();

    void on_checkBox_autoStab_stateChanged(int arg1);

    void on_checkBox_autoDir_stateChanged(int arg1);

    void on_pushButton_resetSimu_clicked();

    void on_pushButton_zoom_clicked();

    void on_pushButton_dezoom_clicked();

private:
    Ui::MainWindow *_ui;
    QGraphicsScene* _scene;
    QGraphicsScene* _forceScene;

    GraphicViewSurcharged * _graphicView;

    QPen _penPoint;
    QPen _penMousePoint;
    QPen _bluePen;
    QPen _greenPen;
    QPen _redPen;
    QPen _redPenHeavier;
    QPen _bluePenHeavier;
    QBrush _brushPoint;

    QVector<QGraphicsRectItem*> _rocketItem;
    QVector<QGraphicsEllipseItem*> _rocketHistoricItem;
    QVector<QGraphicsEllipseItem*> _energyItem;
    QVector<QGraphicsLineItem*> _ForceItemMain;
    QVector<QGraphicsLineItem*> _stepLine;
    QVector<QGraphicsTextItem*> _stepTitle;

    QVector<QGraphicsLineItem*> _mouseLine;
    QVector<QGraphicsTextItem*> _mouseTitle;

    QVector<QGraphicsLineItem*> _ForceItem;

    void refresh();
    void refreshMouseDisplay(QPoint p);
    void refreshViewDisplay(QPoint p);

    WorldManager * _worldManager;

};
#endif // MAINWINDOW_H
