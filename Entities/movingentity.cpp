#include "movingentity.h"
#include "Shapes/shape.h"
#include "Scene.h"

MovingEntity::MovingEntity(Scene* scene, Shape *shape, float mass, const QVector3D& initialVelocity)
    : GameEntity{ shape, EntType::moving },
      _mass     { mass },
      _v        { initialVelocity },
      _steering { this, scene }
{
    Q_ASSERT(scene);
    _steering.wanderOn();
}

MovingEntity::~MovingEntity()
{
    //deletion happens in class scene
}

void MovingEntity::update(int deltaTime) {
    //Calculate steering force
    QVector3D force = _steering.update();
    _a = force / _mass;

    //basic Newton:
    _v += _a*deltaTime / 100.f;
    _transform.move(_v*deltaTime / 1000.f);

    //align objects orientation with its velocity(this is the easy way...):
    if (_v.lengthSquared() > 0.001){
        _transform.lookAt(QVector2D{ _v.x(), _v.y() });
    }

    //give the world-matrix to the shape:
    _shape->setMatrix(_transform.matrix());

    //update the aabb:
    _aabb.update(_transform.matrix());
    _aabbShape.update();
}
