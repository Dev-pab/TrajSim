#ifndef ROCKET_H
#define ROCKET_H

#include <QObject>
#include <QVector>

enum ForceType
{
    Gravity,
    thrust,
    friction,
    propulsor,

    forceNB
};

struct Force
{
    double strengh;
    double angle;
};

class Rocket : public QObject
{
    Q_OBJECT
public:
    explicit Rocket(QObject *parent);

    void setPosition(double x, double y) { _x = x; _y = y; };
    void setMass(double g) { _mass = g; };
    void setVelocity(double v) { _velocity = v; };
    void setVelocityAngle(double a) { _velocityAngle = a; };
    void setAcceleration(double a) { _acceleration = a; };
    void setAccelerationAngle(double a) { _accelerationAngle = a; };
    void setForce(ForceType id, double strengh, double angle);

    double x() { return _x; };
    double y() { return _y; };
    double angle() { return _angle; };
    double rotationSpeed() { return _rotationSpeed; };
    double mass() { return _mass; };
    double velocity() { return _velocity; };
    double velocityAngle() { return _velocityAngle; };
    double acceleration() { return _acceleration; };
    double accelerationAngle() { return _accelerationAngle; };
    double friction() { return _currentFriction; };
    Force force(ForceType id);

    void simulateNexStep(int ms);

    void enableAutoStab(bool e) { _enableAutoStab = e; };
    void enableAutoDir(bool e) { _enableAutoDir = e; };

private:

       void runAsservStep(int ms);

       double _x = 0;
       double _y = 20;

       double _angle = 60;
       double _rotationSpeed = 0.2;

       double _mass = 5500;

       double _velocity = 0;
       double _velocityAngle = -90;

       double _acceleration = 0;
       double _accelerationAngle = 0;

       QVector<Force> _forces;

       double _rotationFriction = 0.1; // °/s²
       double _currentFriction = 0;

       double _angleTarget = 0;

       bool _enableAutoStab = false;
       bool _enableAutoDir = false;

       bool _isApogee = false;

};

#endif // ROCKET_H
