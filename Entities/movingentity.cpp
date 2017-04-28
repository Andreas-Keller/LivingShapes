#include "movingentity.h"
#include "Shapes/shape.h"

MovingEntity::MovingEntity(Shape *shape, float mass, const QVector3D& initialVelocity)
    : GameEntity{ shape },
      _mass     { mass },
      _v        { initialVelocity }

{}

MovingEntity::~MovingEntity()
{
    if (_shape) {
        delete _shape;
        _shape = nullptr;
    }
}

void MovingEntity::update(int deltaTime) {
    //TODO: Calculate steering force

    //basic Newton:
    _v += _a*deltaTime;
    _transform.move(_v*deltaTime / 1000.f);

    //TODO: align objects orientation with its velocity

    _shape->setMatrix(_transform.matrix());
}
