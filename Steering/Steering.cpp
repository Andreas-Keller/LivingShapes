#include "Steering.h"

#include "Entities/movingentity.h"

Steering::Steering(MovingEntity *owner, Scene* scene)
    : _wanderOn { false },
      _owner    { owner },
      _scene	{ scene }
{
    Q_ASSERT(_scene && _owner);
}

Steering::~Steering() {

}

QVector3D Steering::update() {

    QVector3D force;

    //update all steering behaviours:
    if (_wanderOn) force += _wander.update(_owner);

    return force;
}
