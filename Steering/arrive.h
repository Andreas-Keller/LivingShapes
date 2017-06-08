#ifndef ARRIVE_H
#define ARRIVE_H

#include <QVector3D>

class MovingEntity;

enum class SpeedType {
    fast,
    medium,
    slow
};

class Arrive {
public:
    Arrive(SpeedType speedType = SpeedType::medium);
    ~Arrive();

    QVector3D update(MovingEntity* owner);

    void setSpeed(SpeedType speedType) { _speedType = speedType; }
    void setTarget(const QVector3D& target) { _target = target; }
    void setSpeedTweaker(float value)       { _speedTweaker = value; }

private:
    SpeedType   _speedType;
    float       _speedTweaker;

    QVector3D   _target;
};

#endif // ARRIVE_H
