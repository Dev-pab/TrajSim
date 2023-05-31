#include "rocket.h"
#include "Tabs.h"

#include <QtMath>
#include <QDebug>

namespace {
constexpr double ANGLE_SPEED_P = 100;

constexpr double ANGLE_DIR_P = 5;
}

double errAngleDir = 0;

int cpt_propu = 0;
double max_speed = 0;
double max_altitude = 0;
int timeSupersonic = 0;
int timeTransonic = 0;

Rocket::Rocket(QObject *parent) :
    QObject{parent}
{
    Force defaultForce;

    defaultForce.angle = 0;
    defaultForce.strengh = 0;

    for (int i=0; i < forceNB; i++)
    {
        _forces.append(defaultForce);
    }
}

void Rocket::setForce(ForceType id, double strengh, double angle)
{
    if (id < forceNB)
    {
        _forces[id].angle = angle;
        _forces[id].strengh = strengh;
    }
    qDebug() << _forces[id].angle << _forces[id].strengh;
}

Force Rocket::force(ForceType id)
{
    if (id < forceNB)
    {
        return _forces[id];
    }

    return _forces.at(0);
}

void Rocket::simulateNexStep(int ms)
{
    /* Calculation of forces sum */

    double xSum = 0;
    double ySum = 0;

    if (_velocity > 343)
    {
        timeSupersonic += 10;
    }
    if (_velocity > 257.25)
    {
        timeTransonic += 10;
    }

    for (int i=0; i < _forces.size(); i++)
    {
        if (i == ForceType::propulsor && false)
        {
            double angle = (qCos( qDegreesToRadians(_forces[i].angle) ) * (_forces[i].strengh * (ms / 1000.00)) );

            _rotationSpeed -= angle * 0.01;

            if (_rotationSpeed < 0)
            {
                //_rotationSpeed += _rotationFriction * (ms / 1000.00);
            }
            else
            {
                //_rotationSpeed -= _rotationFriction * (ms / 1000.00);
            }

            _angle += _rotationSpeed;

            if (_rotationSpeed < 0)
            {
                _accelerationAngle += 180;
            }

            xSum += (qCos( qDegreesToRadians(_angle) ) * (_forces[i].strengh * (ms / 1000.00)) );
            ySum += (qSin( qDegreesToRadians(_angle) ) * (_forces[i].strengh * (ms / 1000.00)) );

        }
        else if (i == ForceType::friction)
        {
            double acc = 0;

            if (!_isApogee)
            {
                acc = ( 0.0014 * qPow(_velocity, 2) - 0.152 * _velocity + 4.958 );
            }
            else
            {
                acc = (1.3 * 0.66 * 2.2 * qPow(_velocity, 2) ) / 2;
            }

            _currentFriction = acc;
            acc = acc / (_mass/1000);

            xSum += (qCos( qDegreesToRadians(_velocityAngle-180) ) * (acc * (ms / 1000.00)) );
            ySum += (qSin( qDegreesToRadians(_velocityAngle-180) ) * (acc * (ms / 1000.00)) );
        }
        else if (i == ForceType::thrust)
        {
            if (cpt_propu < 170)
            {
                _forces[ForceType::thrust].strengh = tab_propulsor[cpt_propu] / (_mass/1000);
                cpt_propu++;

                _mass -= 4.28;
            }
            else
            {
                _forces[ForceType::thrust].strengh = 0;
            }
            qDebug() << _mass << max_altitude << max_speed << _x << timeSupersonic << timeTransonic;
            //qDebug() << _forces[thrust].strengh;

            xSum += (qCos( qDegreesToRadians(_forces[i].angle) ) * (_forces[i].strengh * (ms / 1000.00)) );
            ySum += (qSin( qDegreesToRadians(_forces[i].angle) ) * (_forces[i].strengh * (ms / 1000.00)) );
        }
        else
        {
            xSum += (qCos( qDegreesToRadians(_forces[i].angle) ) * (_forces[i].strengh * (ms / 1000.00)) );
            ySum += (qSin( qDegreesToRadians(_forces[i].angle) ) * (_forces[i].strengh * (ms / 1000.00)) );
        }
    }

    //////////// UPDATE ACCELERATION ///////////////

    double xVelocity = xSum + (qCos( qDegreesToRadians(_velocityAngle) ) * _velocity);
    double yVelocity = ySum + (qSin( qDegreesToRadians(_velocityAngle) ) * _velocity);

    _acceleration = qSqrt( xSum*xSum + ySum*ySum) * (1000.00 / ms);
    _accelerationAngle = qRadiansToDegrees( qAtan(ySum / xSum) );


    if (xSum < 0)
    {
        _accelerationAngle += 180;
    }

    //////////// UPDATE VELOCITY ///////////////

    _velocity = qSqrt( xVelocity*xVelocity + yVelocity*yVelocity );
    _velocityAngle = qRadiansToDegrees( qAtan(yVelocity / xVelocity) );

    if (xVelocity < 0)
    {
        _velocityAngle += 180;
    }

    //////////// UPDATE POSITION ///////////////

    _x += xVelocity * (ms / 1000.00);
    _y += yVelocity * (ms / 1000.00);

    _angle = _velocityAngle;

    if (_y <= 0)
    {
        _y = 0;
    }

    if (max_speed < _velocity)
    {
        max_speed = _velocity;
    }

    if (max_altitude < _y)
    {
        //max_altitude = _y;
    }
    else
    {
        _isApogee = true;
    }

    //qDebug() << max_speed << max_altitude << _x << _isApogee;

    //runAsservStep(ms);
}

void Rocket::runAsservStep(int ms)
{
    if (_enableAutoDir || _enableAutoStab)
    {
        double angle = 0, angleOrigin = _angle - 90;
/*
        while (angleOrigin > 360) {
            angleOrigin -= 360;
        }
        while (angleOrigin < 360) {
            angleOrigin += 360;
        }*/

        if (_enableAutoStab || _enableAutoDir)
        {
            angle = -_rotationSpeed * ANGLE_SPEED_P;
        }
        if (_enableAutoDir)
        {
            angle += (-angleOrigin - _angleTarget) * ANGLE_DIR_P;
        }

        if (_y <= 4000)
        {

        }
        else
        {
            _forces[propulsor].strengh = 100;
        }

        if (angle < -90) angle = -90;
        if (angle > 90) angle = 90;

        _forces[propulsor].angle = angle + 90;
        //qDebug() << angle << _angle;
    }
}
