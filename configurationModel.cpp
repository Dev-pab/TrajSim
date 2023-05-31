#include "configurationModel.h"

const QString EXISTING_str = "EXISTING";

ConfigurationModel::ConfigurationModel(QObject *parent) :
    QObject{parent},
    _setting{new QSettings("MinesSpace", "TrajSim")},
    _initialParam{new InitialParameters_t},
    _rocketParam{new RocketlParameters_t},
    _systemParam{new SystemParameters_t}
{    
    loadConfiguration();
}

void ConfigurationModel::setModelValue(QString key, QString s)
{
    _model[key]->s = s;

    _setting->setValue(key, s);

    updateStruct();
}

void ConfigurationModel::setModelValue(QString key, double d)
{
    _model[key]->d = d;

    _setting->setValue(key, d);

    updateStruct();
}

void ConfigurationModel::setModelValue(QString key, bool b)
{
    _model[key]->b = b;

    _setting->setValue(key, b);

    updateStruct();
}

void ConfigurationModel::loadConfiguration()
{
    bool loadFile = _setting->contains(EXISTING_str);

    QString str;
    foreach (str, ConfigMap.keys())
    {
        _model[str] = ConfigMap[str];

        if (loadFile && _setting->contains(str))
        {
            if (ConfigMap[str]->type == Type_t::STRING) _model[str]->s = _setting->value(str).toString();
            if (ConfigMap[str]->type == Type_t::DOUBLE) _model[str]->d = _setting->value(str).toDouble();
            if (ConfigMap[str]->type == Type_t::BOOL) _model[str]->b = _setting->value(str).toBool();

        }
        else
        {
            if (ConfigMap[str]->type == Type_t::STRING) _setting->setValue(str, _model[str]->s);
            if (ConfigMap[str]->type == Type_t::DOUBLE) _setting->setValue(str, _model[str]->d);
            if (ConfigMap[str]->type == Type_t::BOOL) _setting->setValue(str, _model[str]->b);
        }

        qDebug() << str << ConfigMap[str]->type << ConfigMap[str]->s << ConfigMap[str]->d << ConfigMap[str]->b;
    }

    _setting->setValue(EXISTING_str, true);

    updateStruct();

}

void ConfigurationModel::updateStruct()
{
    _initialParam->angle = _model[INIT_ANGLE]->d;
    _initialParam->x = _model[INIT_X]->d;
    _initialParam->y = _model[INIT_Y]->d;
    _initialParam->gravity = _model[INIT_GRAVITY]->d;
    _initialParam->mass =  _model[INIT_MASS]->d;

    _systemParam->propulsorPath = _model[PROPULSION_PATH]->s;
    _systemParam->frictionPath = _model[FRICTION_PATH]->s;

    emit valueUpdated();
}
