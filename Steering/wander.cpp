#include "wander.h"

#include "Entities/movingentity.h"
#include "utils/randomizer.h"

Wander::Wander(float distance, float radius, float jitter)
    : 	_distance	{ distance },
		_radius		{ radius },
        _jitter 	{ jitter }
{
    _target.setY(_distance + _radius);
}


Wander::~Wander() {}


QVector3D Wander::update(MovingEntity* owner) {
	//add a random displacment to the target:
    _target += QVector2D{ Randomizer::randFloat(0, 1) * _jitter,
                          Randomizer::randFloat(0, 1) * _jitter };

	//reproject the target onto the wander circle:
	_target.normalize();
	_target *= _radius;

	//translate target by the wander distance in front of our entity:
    QVector3D localPos = QVector3D(_target.x(), _target.y() + _distance, 0.0f);

    //transform target into world space:
    QVector3D worldPos = owner->transform()->matrix().inverted() * localPos;
    return worldPos - owner->transform()->pos();
}
