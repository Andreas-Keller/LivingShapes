#include "movingentity.h"
#include "Shapes/shape.h"
#include "Scene.h"

MovingEntity::MovingEntity(Scene* scene, Shape *shape, float mass, const QVector3D& initialVelocity)
    : GameEntity{ shape, EntType::moving },
      _mass     { mass },
      _v        { initialVelocity },
      _maxF     { 2.f },
      _steering { this, scene },
      _tracker  { false }
{
    Q_ASSERT(scene);
    _steering.wanderOn();
}

MovingEntity::~MovingEntity()
{
    //deletion happens in class scene
}

void MovingEntity::update(int deltaTime) {
    //get the steering force:
    QVector3D force = _steering.update(deltaTime, _tracker);

    if (force.lengthSquared() > 0.01f)
        _a = force / _mass;

    //basic Newton:
    _v += _a*deltaTime / 1000.f;
    _transform.move(_v*deltaTime / 1000.f);

    //align objects orientation with its velocity(this is the easy way...):
    if (_v.lengthSquared() > 0.001){
        _transform.lookAt(QVector2D{ _v.x(), _v.y() }, _tracker);
    }

    //give the world-matrix to the shape:
    _shape->setMatrix(_transform.matrix());

    //update the aabb:
    _aabb.update(_transform.matrix());
    _aabbShape.update();

    _tracker = false;
}
