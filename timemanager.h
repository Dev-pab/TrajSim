#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <QObject>
#include <QTimer>

class TimeManager : public QObject
{
    Q_OBJECT
public:
    explicit TimeManager(QObject *parent);

    void run();
    void pause();

    int stepDuration() { return _step_ms; };

    void setSpeedFactor(double f);
    void setStepDuration(int ms) { _step_ms = ms; };

    int counter() { return _counter; };

signals:
    void newStep(int ms);

private:

    void newMainStep();

    QTimer * _mainTimer;

    int _step_ms = 1000;
    int _speedfactor = 1;

    int _lastDuration_ms;
    int _counter = 0;
};

#endif // TIMEMANAGER_H
