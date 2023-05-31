#ifndef CONFIGURATIONMODEL_H
#define CONFIGURATIONMODEL_H

#include <QObject>
#include <QSettings>
#include <QMap>
#include <QStringList>

#include "Tabs.h"

struct InitialParameters_t
{
    double angle;
    double x;
    double y;

    double gravity;
    double mass;
};

struct RocketlParameters_t
{

};

struct SystemParameters_t
{
    QString propulsorPath;
    QString frictionPath;
};

class ConfigurationModel : public QObject
{
    Q_OBJECT
public:
    explicit ConfigurationModel(QObject *parent);

    InitialParameters_t *  initialParam() const { return _initialParam; }
    RocketlParameters_t * rocketParam() const { return _rocketParam; }
    SystemParameters_t * systemParam() const { return _systemParam; }

    QMap<QString, Value_t*> model() const { return _model; }

    void setModelValue(QString key, QString s);
    void setModelValue(QString key, double d);
    void setModelValue(QString key, bool b);

    void loadConfiguration();

signals:
    void valueUpdated();

private:

    void updateStruct();

    QSettings * _setting;

    QMap<QString, Value_t*> _model;

    InitialParameters_t * _initialParam;
    RocketlParameters_t * _rocketParam;
    SystemParameters_t * _systemParam;

};

#endif // CONFIGURATIONMODEL_H
