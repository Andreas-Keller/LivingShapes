#include "formation.h"

#include "Entities/movingentity.h"

Formation::Formation()
    :   _leader{ nullptr }
{}


Formation::Formation(MovingEntity *leader, QVector3D &offset)
    :   _leader{ leader },
        _offset{ offset }
{
    _arrive.setSpeed(SpeedType::fast);
}

Formation::~Formation() {}

QVector3D Formation::update(MovingEntity *owner) {

    Q_ASSERT(_leader);

    //calculate offset coordinates in world space:
    QMatrix4x4 M = _leader->transform()->matrix().inverted();
    QVector3D offsetWorld = M * _offset;

    //vector from the entity to the offset:
    QVector3D toOffset = offsetWorld - owner->transform()->pos();

    //calculate a look-ahead time:
    float lookAhead = toOffset.length() / (owner->maxSpeed() + _leader->velocity().length());

    _arrive.setTarget(offsetWorld + _leader->velocity() * lookAhead);
    return _arrive.update(owner);
}
