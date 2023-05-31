#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include <QObject>
#include <QVector>
#include <QPointF>

#include "rocket.h"
#include "timemanager.h"
#include "configurationModel.h"

class Rocket;
class TimeManager;

struct TargetModel
{
    double x;
    double y;
    double angle;

    bool enableAutoStab = false;
    bool enableAutoDir = false;
    bool enableAutoThrust = false;
};

struct PositionModel
{
   double rocketX;
   double rocketY;

   double angle;
   double rotationSpeed;

   double propulsorAngle;
   double propulsorThrust;

   QVector<QPointF> rocketHistoric;
   QVector<QPointF> velocityHistoric;
   QVector<QPointF> frictionHistoric;

   double rocketVelocity;
   double rocketVelocityAngle;

   double rocketAccelerationSum;
   double rocketAccelerationSumAngle;

   int elapsedTime;
};

class WorldManager : public QObject
{
    Q_OBJECT
public:
    explicit WorldManager(QObject *parent);
    ~WorldManager();

    void run();
    void pause();
    void setSimulationSpeed(double s) { _timeManager->setSpeedFactor(s); };

    void setInitialParameters(InitialParameters_t * const);

    void setRocketForce(ForceType id, double strengh, double angle);

    void setTarget(TargetModel m);
    TargetModel target() { return _target; };

    PositionModel model() { return _positionModel; };

signals:
    void newStep(int ms);

private:

    void newMainStep(int ms);
    void simulateRocket(int ms);

    Rocket * _rocket;
    TimeManager * _timeManager;

    double _gravity = 9.81;

    PositionModel _positionModel;

    TargetModel _target;

};

#endif // WORLDMANAGER_H
