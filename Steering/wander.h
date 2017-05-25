#ifndef WANDER_H
#define WANDER_H

#include <QVector2D>

#include "Entities/movingentity.h"

class Wander {
public:
    Wander	(MovingEntity* owner, 
			float distance = 2.f, 
			float radius = 1.f, 
			float jitter = 0.2f);
    ~Wander();

    QVector2D update();

	//setters:
	void setDistance(float distance)	{ _distance = distance; }
	void setRadius(float radius) 		{ _radius = radius; }
	void setJitter(float jitter) 		{ _jitter = jitter; }	

private:
	float 	_distance;
	float 	_radius;
	float 	_jitter;
	
	QVector2D 		_target;

	MovingEntity* 	_owner;
};

#endif // WANDER_H
