#include "arrive.h"

#include "Entities/movingentity.h"

Arrive::Arrive(SpeedType speedType)
    :   _speedType      { speedType },
        _speedTweaker   { 0.3f }
{}

Arrive::~Arrive() {}

QVector3D Arrive::update(MovingEntity *owner) {

    QVector3D toTarget = _target - owner->transform()->pos();
    float dist = toTarget.length();

    if (dist < 0.01f) return QVector3D{ 0.f, 0.f, 0.f };

    //calculate speed:
    float speed = dist / (static_cast<float>(_speedType) * _speedTweaker);
    //speed = std::fmin(owner->maxSpeed(), speed);

    //calculate desired velocity:
    QVector3D desiredVelocity = toTarget * speed / dist;

    return (desiredVelocity - 10.f * owner->velocity());
}
