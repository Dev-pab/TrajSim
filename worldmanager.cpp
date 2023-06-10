#include "worldmanager.h"

#include <QDebug>

WorldManager::WorldManager(QObject *parent) :
    QObject{parent},
    _rocket(new Rocket(parent)),
    _timeManager(new TimeManager(parent))
{
    connect(_timeManager, &TimeManager::newStep, this, &WorldManager::newMainStep);
}

WorldManager::~WorldManager()
{

}

void WorldManager::run()
{
    _timeManager->setStepDuration(10);
    _timeManager->setSpeedFactor(50);
    _timeManager->run();
}

void WorldManager::pause()
{
    _timeManager->pause();
}

void WorldManager::setInitialParameters(InitialParameters_t * i)
{
    _rocket->setForce(ForceType::Gravity, i->gravity, -90);
    _rocket->setForce(ForceType::thrust, 0, i->angle);
    _rocket->setMass(i->mass);

    _rocket->setPosition(i->x, i->y);
}

void WorldManager::setRocketForce(ForceType id, double strengh, double angle)
{
    _rocket->setForce(id, strengh, angle);
}

void WorldManager::setTarget(TargetModel m)
{
    _target = m;

    _rocket->enableAutoStab(_target.enableAutoStab);
    _rocket->enableAutoDir(_target.enableAutoDir);
}

void WorldManager::newMainStep(int ms)
{
    simulateRocket(ms);

    _positionModel.elapsedTime = _timeManager->counter() * _timeManager->stepDuration();

    _positionModel.rocketX = _rocket->x();
    _positionModel.rocketY = -_rocket->y();

    _positionModel.angle = -_rocket->angle();
    _positionModel.rotationSpeed = _rocket->rotationSpeed();

    _positionModel.propulsorAngle = _rocket->force(ForceType::propulsor).angle;
    _positionModel.propulsorThrust = _rocket->force(ForceType::propulsor).strengh;

    QPointF point(_rocket->x(), -_rocket->y());
    _positionModel.rocketHistoric.append(point);

    QPointF velocityPoint(_timeManager->counter(), _rocket->velocity());
    _positionModel.velocityHistoric.append(velocityPoint);

    QPointF frictionPoint(_timeManager->counter(), _rocket->friction());
    _positionModel.frictionHistoric.append(frictionPoint);

    _positionModel.rocketVelocity = _rocket->velocity();
    _positionModel.rocketVelocityAngle = _rocket->velocityAngle();

    _positionModel.energy = 0.5 * _rocket->mass() * _rocket->velocity() * _rocket->velocity();

    _positionModel.openingShock = (1.3 * 0.66 * 2.2 * qPow(_rocket->velocity(), 2) ) / 2 / 10;
    qDebug() << _positionModel.openingShock;

    _positionModel.rocketAccelerationSum = _rocket->acceleration();
    _positionModel.rocketAccelerationSumAngle = _rocket->accelerationAngle();

    if (_rocket->y() == 0)
    {
        _timeManager->pause();
    }

    emit newStep(ms);
}

void WorldManager::simulateRocket(int ms)
{
    _rocket->simulateNexStep(ms);
}
