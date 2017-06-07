#include "Steering.h"

#include "Entities/movingentity.h"

Steering::Steering(MovingEntity *owner, Scene* scene)
    : _wanderOn { true },
      _avoiderOn{ true },
      _owner    { owner },
      _scene	{ scene },
      _updateTime { 500 },
      _accumTime  { 0 }
{
    Q_ASSERT(_scene && _owner);
}

Steering::~Steering() {

}

QVector3D Steering::update(size_t timestep, bool tracker) {

    //if the timestep is smaller than the steering behaviour update time,
    //just return the force calculated in an earlier step:
    _accumTime += timestep;
    if (_accumTime < _updateTime) return _force;

    _accumTime -= _updateTime;

    //update all steering behaviours:
    _force = QVector3D{ 0.f, 0.f, 0.f };

    if (_avoiderOn) _force += _avoider.update(_owner, _scene);
    if (_wanderOn && _force.length() < _owner->maxForce()) _force += _wander.update(_owner);

    return _force;
}
