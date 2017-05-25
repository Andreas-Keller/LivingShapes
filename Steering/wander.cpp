#include "wander.h"

Wander::Wander(MovingEntity* owner, float distance, float radius, float jitter) 
	: 	_distance	{ distance },
		_radius		{ radius },
		_jitter 	{ jitter },
		_owner 		{ owner }
{
	Q_ASSERT(_owner);
    _target.setX(_distance + _radius);
}


Wander::~Wander() {}


QVector2D Wander::update() {
	
	//add a random displacment to the target:

	
	//reproject the target onto the wander circle:
	_target.normalize();
	_target *= _radius;

	//translate target by the wander distance in front of our entity:
    QVector3D localPos = QVector3D(_target.x() + _distance, _target.y(), 0.0f);

	//transform target into world space:
    QVector3D worldPos = _owner->transform()->matrix().inverted() * localPos;
    QVector3D direction = worldPos - _owner->transform()->pos();
}
