#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsItem>
#include <QDebug>
#include <QtMath>
#include <QTransform>

int pointCpt = 0;
double paraCpt = 0;

double maxAlt = 0;
bool maxAltDone = 0;
double maxSpeed = 0;
bool maxSpeedDone = 0;

enum MouseLine
{
    HORIZONTAL,
    VERTICAL,

    LINE_NB
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _scene(new QGraphicsScene),
    _forceScene(new QGraphicsScene),
    _graphicView(new GraphicViewSurcharged(this)),
    _worldManager(new WorldManager(parent))
{
    _ui->setupUi(this);

    _ui->widgetView->setLayout(new QVBoxLayout(this));
    _ui->widgetView->layout()->addWidget(_graphicView);
    _graphicView->setScene(_scene);

    QList<QWidget*> widgets = this->findChildren<QWidget*>();

    QPalette pal = this->palette();
    pal.setColor(QPalette::ButtonText, QColor(Qt::blue));
    pal.setColor(QPalette::Text, QColor(Qt::green));
    pal.setColor(QPalette::BrightText, QColor(Qt::red));

    foreach (QWidget* w, widgets)
    {
        w->setPalette(pal);
        w->setAutoFillBackground(true);
        w->update();
        w->repaint();
        //w->show();

        //_ui->pushButton_dezoom->setPalette(this->palette());

    }

    _graphicView->setScene(_scene);
    _ui->graphicsView_forceView->setScene(_forceScene);

    _graphicView->setRenderHint(QPainter::Antialiasing);
    _ui->graphicsView_forceView->setRenderHint(QPainter::Antialiasing);
    _graphicView->setStyleSheet("background: transparent; border: 0px");
    _graphicView->setCursor(QCursor(Qt::BlankCursor));

    _penPoint.setBrush(QColorConstants::White);
    _penPoint.setWidth(20);

    _greenPen.setBrush(QColorConstants::Green);
    _greenPen.setWidth(40);

    _penMousePoint.setBrush(QColorConstants::White);
    _penMousePoint.setWidth(2);

    _bluePen.setBrush(QColorConstants::Blue);
    _bluePen.setWidth(6);

    _redPen.setBrush(QColorConstants::Red);
    _redPen.setWidth(4);

    _redPenHeavier.setBrush(QColorConstants::Red);
    _redPenHeavier.setWidth(40);

    _bluePenHeavier.setBrush(QColorConstants::Blue);
    _bluePenHeavier.setWidth(40);

    _brushPoint.setColor(QColorConstants::Red);

    QPen redFill;
    redFill.setColor(QColorConstants::Red);
    redFill.setWidth(60);

    //////////////////////////////////////////////////////////
    _scene->setSceneRect(-4000, 0, 12000, -12000);

    _rocketItem.append(_scene->addRect(0, 0, 600, 250, redFill));
    _ForceItemMain.append(_scene->addLine(0,0,0,0, redFill));

    _stepLine.append(_scene->addLine(0,0,0,0, _penPoint));
    _stepLine.append(_scene->addLine(0,0,0,0, _penPoint));
    _stepTitle.append(_scene->addText("", QFont("Arial",300)));
    _stepTitle.append(_scene->addText("", QFont("Arial",300)));

    for (int i=0; i<MouseLine::LINE_NB; i++)
    {
        _mouseLine.append(_scene->addLine(0,0,0,0, _penMousePoint));
        _mouseTitle.append(_scene->addText("", QFont("Arial",100)));
    }

    _scene->addLine(0,0 , 10000,0, _penPoint);
    //_scene->addLine(0,10000 , 10000,10000, _penPoint);
    //_scene->addLine(0,0 , 0,10000, _penPoint);
    //_scene->addLine(10000,10000 , 10000,0, _penPoint);

    //////////////////////////////////////////////////////////
    _ui->graphicsView_forceView->scale(1, -1);
    _forceScene->setSceneRect(0, 0, 300, 300);
    _forceScene->addEllipse(150-3, 150-3, 6, 6, _penPoint);

    _ForceItem.append(_forceScene->addLine(0,0,0,0, _bluePen));
    _ForceItem.append(_forceScene->addLine(0,0,0,0, _redPen));

    _ui->graphicsView_forceView->centerOn(150, 150);
    _ui->graphicsView_forceView->setStyleSheet("background: transparent; border: 0px");

    _ui->plotZone->plot1()->enableCurve(SPEED);
    _ui->plotZone->plot2()->enableCurve(FRICTION);

    connect(_worldManager, &WorldManager::newStep, this, &MainWindow::refresh);

    connect(_ui->configuration_Widget, &configuration::valueUpdated, this, [this] () {
        _worldManager->setInitialParameters(_ui->configuration_Widget->getModel()->initialParam());
    });

    connect(_graphicView, &GraphicViewSurcharged::MouseMooved, this, &MainWindow::refreshMouseDisplay);
    connect(_graphicView, &GraphicViewSurcharged::WheelMooved, this, &MainWindow::refreshViewDisplay);

    _ui->configuration_Widget->syncModel();
    _worldManager->setInitialParameters(_ui->configuration_Widget->getModel()->initialParam());

    QTimer::singleShot(100, this, SLOT(fitIn()));
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::refresh()
{
    PositionModel model = _worldManager->model();

    _rocketItem[0]->setPos(model.rocketX-300, model.rocketY-125);
    _rocketItem[0]->setTransformOriginPoint( 300, 125);
    _rocketItem[0]->setRotation(model.angle);

    QPointF propuOrigin(model.rocketX -(qCos( qDegreesToRadians(model.angle) )*300), model.rocketY - (qSin( qDegreesToRadians(model.angle) )*300));

    _ForceItemMain[0]->setLine(propuOrigin.x(), propuOrigin.y(), propuOrigin.x()+(qCos( qDegreesToRadians(model.propulsorAngle-model.angle+90) ) * model.propulsorThrust*5), propuOrigin.y()-(qSin( qDegreesToRadians(model.propulsorAngle-model.angle+90) ) * model.propulsorThrust*5));

    if (model.openingShock <= 200 && model.elapsedTime > 2000)
    {
        _energyItem.append(_scene->addEllipse(model.rocketHistoric.last().x()-20, model.rocketHistoric.last().y()-20, 40, 40, _greenPen, _brushPoint));
        _energyItem.last()->setZValue(10);

        paraCpt += 10;

        //qDebug() << paraCpt / 1000;
    }

    if (model.rocketHistoric.size() > _rocketHistoricItem.size() && pointCpt > 7)
    {
        if (_worldManager->model().rocketVelocity >= 343)
        {
            _rocketHistoricItem.append(_scene->addEllipse(model.rocketHistoric.last().x()-10, model.rocketHistoric.last().y()-10, 20, 20, _redPenHeavier, _brushPoint));
        }
        else if (_worldManager->model().rocketVelocity >= 257.25)
        {
            _rocketHistoricItem.append(_scene->addEllipse(model.rocketHistoric.last().x()-10, model.rocketHistoric.last().y()-10, 20, 20, _bluePenHeavier, _brushPoint));
        }
        else
        {
            _rocketHistoricItem.append(_scene->addEllipse(model.rocketHistoric.last().x()-10, model.rocketHistoric.last().y()-10, 20, 20, _penPoint, _brushPoint));
        }
        _rocketHistoricItem.last()->setZValue(20);


        for (int i=0; i<100; i++)
        {
            if (_rocketHistoricItem.size() > i+1)
            {
                if (_rocketHistoricItem[_rocketHistoricItem.size()-1-i]->opacity() > 0.3)
                {
                    _rocketHistoricItem[_rocketHistoricItem.size()-1-i]->setOpacity(_rocketHistoricItem[_rocketHistoricItem.size()-1-i]->opacity()-0.01);
                }
            }
        }

        pointCpt = 0;
    }
    pointCpt++;

    _ForceItem[0]->setLine(150,150, 150+(qCos( qDegreesToRadians(model.rocketVelocityAngle) ) * model.rocketVelocity/3), 150+(qSin( qDegreesToRadians(model.rocketVelocityAngle) ) * model.rocketVelocity)/3);
    _ForceItem[1]->setLine(150,150, 150+(qCos( qDegreesToRadians(model.rocketAccelerationSumAngle) ) * model.rocketAccelerationSum*2), 150+(qSin( qDegreesToRadians(model.rocketAccelerationSumAngle) ) * model.rocketAccelerationSum*2));

    if (_worldManager->model().rocketVelocity > maxSpeed)
    {
        maxSpeed = _worldManager->model().rocketVelocity;
    }
    else if (!maxSpeedDone)
    {
        _stepLine[0]->setLine(model.rocketHistoric.last().x()-25, model.rocketHistoric.last().y()-25, model.rocketHistoric.last().x()-25 - 1000, model.rocketHistoric.last().y()-25);
        _stepTitle[0]->setPos(model.rocketHistoric.last().x()-25 - 3500, model.rocketHistoric.last().y()-25 - 250);
        _stepTitle[0]->setPlainText(QString::number(maxSpeed) + "m/s");
        maxSpeedDone = true;
    }

    if (_worldManager->model().rocketY < maxAlt)
    {
        maxAlt = _worldManager->model().rocketY;
    }
    else if (!maxAltDone)
    {
        _stepLine[1]->setLine(model.rocketHistoric.last().x()-25, model.rocketHistoric.last().y()-25, model.rocketHistoric.last().x()-25 - 1000, model.rocketHistoric.last().y()-25);
        _stepTitle[1]->setPos(model.rocketHistoric.last().x()-25 - 3000, model.rocketHistoric.last().y()-25 - 250);
        _stepTitle[1]->setPlainText(QString::number(-maxAlt) + "m\n" + QString::number(model.elapsedTime / 1000.00) + "s");
        maxAltDone = true;
    }

    _ui->label_elapsedTime->setText(QString::number(model.elapsedTime));
    _ui->label_altitude->setText(QString::number(-model.rocketY));
    _ui->label_speed->setText(QString::number(model.rocketVelocity));
    _ui->label_acceleration->setText(QString::number(model.rocketAccelerationSum));
    _ui->label_rotationSpeed->setText(QString::number(model.rotationSpeed * 100)); // 10ms / 1s

    if (_ui->checkBox_autoFocus->isChecked()) _graphicView->centerOn(model.rocketX, model.rocketY);

    _ui->plotZone->plot1()->appendData(Curve::SPEED, _worldManager->model().velocityHistoric.last());
    _ui->plotZone->plot2()->appendData(Curve::FRICTION, _worldManager->model().frictionHistoric.last());
}

void MainWindow::refreshMouseDisplay(QPoint p)
{
    QPointF scenePos = _graphicView->mapToScene(p);
    _mouseLine[MouseLine::VERTICAL]->setLine(scenePos.x(), 0, scenePos.x(), -12000);
    _mouseLine[MouseLine::HORIZONTAL]->setLine(0, scenePos.y(), 12000, scenePos.y());

    _mouseTitle[VERTICAL]->setPos(scenePos.x() + 100, scenePos.y() - 600);
    _mouseTitle[VERTICAL]->setPlainText(QString::number(int(scenePos.x())) + " m");

    _mouseTitle[HORIZONTAL]->setPos(scenePos.x() + 600, scenePos.y() - 200);
    _mouseTitle[HORIZONTAL]->setPlainText(QString::number(int(-scenePos.y())) + " m");
}

void MainWindow::refreshViewDisplay(QPoint p)
{
    QPointF scenePos = _graphicView->mapToScene(_graphicView->lastMouseCoord());

    _graphicView->centerOn(scenePos.x(), scenePos.y());

    if (p.y() > 0)
    {
        _graphicView->scale(1.01, 1.01);
    }
    else
    {
        _graphicView->scale(0.99, 0.99);
    }
}

void MainWindow::on_pushButton_clicked()
{
    _worldManager->run();
}

void MainWindow::fitIn()
{
    _graphicView->centerOn(0, 0);
    _graphicView->scale(0.08, 0.08);
}

void MainWindow::on_pushButton_pause_clicked()
{
    _worldManager->pause();
}


void MainWindow::on_horizontalSlider_simSpeed_valueChanged(int value)
{
    _worldManager->setSimulationSpeed(100-value);
}


void MainWindow::on_pushButton_resetSpeed_clicked()
{
    _ui->horizontalSlider_simSpeed->setValue(50);
}


void MainWindow::on_horizontalSlider_accelAngle_valueChanged(int value)
{
    _worldManager->setRocketForce(ForceType::propulsor, _ui->horizontalSlider_accelForce->value(), value);
    _ui->doubleSpinBox_accelAngle->setValue(value);
}


void MainWindow::on_horizontalSlider_accelForce_valueChanged(int value)
{
    _worldManager->setRocketForce(ForceType::propulsor, value, _ui->horizontalSlider_accelAngle->value());
    _ui->doubleSpinBox_accelForce->setValue(value);
}


void MainWindow::on_actionIncrease_Rocket_Force_triggered()
{
    _ui->horizontalSlider_accelForce->setSliderPosition(_ui->horizontalSlider_accelForce->value()+5);
}


void MainWindow::on_actionDecrease_Rocket_Force_triggered()
{
    _ui->horizontalSlider_accelForce->setSliderPosition(_ui->horizontalSlider_accelForce->value()-5);
}


void MainWindow::on_actionRun_triggered()
{
    _worldManager->run();
}


void MainWindow::on_checkBox_autoStab_stateChanged(int arg1)
{
    TargetModel target = _worldManager->target();

    target.enableAutoStab = arg1;

    _worldManager->setTarget(target);
}


void MainWindow::on_checkBox_autoDir_stateChanged(int arg1)
{
    TargetModel target = _worldManager->target();

    target.enableAutoDir = arg1;

    _worldManager->setTarget(target);
}


void MainWindow::on_pushButton_resetSimu_clicked()
{
    delete _worldManager;

    _worldManager = new WorldManager(this);

    _worldManager->setInitialParameters(_ui->configuration_Widget->getModel()->initialParam());
}

void MainWindow::on_pushButton_zoom_clicked()
{
    _graphicView->scale(1.1, 1.1);
    _graphicView->centerOn(0, 0);
}


void MainWindow::on_pushButton_dezoom_clicked()
{
    _graphicView->scale(0.9, 0.9);
    _graphicView->centerOn(0, 0);
}

