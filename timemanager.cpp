#include "timemanager.h"

TimeManager::TimeManager(QObject *parent) :
    QObject{parent},
    _mainTimer(new QTimer)
{
    connect(_mainTimer, &QTimer::timeout, this, &TimeManager::newMainStep);
}

void TimeManager::run()
{
    _lastDuration_ms = _step_ms;
    _mainTimer->start(_step_ms * (_speedfactor / 50.00));
}

void TimeManager::pause()
{
    _mainTimer->stop();
}

void TimeManager::setSpeedFactor(double f)
{
    _speedfactor = f;
    _mainTimer->setInterval(_step_ms * (_speedfactor / 50.00));
}

void TimeManager::newMainStep()
{
    _counter++;
    emit newStep(_lastDuration_ms);
}
