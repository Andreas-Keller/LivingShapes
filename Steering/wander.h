#ifndef WANDER_H
#define WANDER_H

#include <QVector3D>

//forward declaration:
class MovingEntity;

class Wander {
public:
    Wander  (float distance = 4.f,
            float radius = 0.5f,
            float jitter = 0.5f);
    ~Wander();

    QVector3D update(MovingEntity* owner);

	//setters:
	void setDistance(float distance)	{ _distance = distance; }
	void setRadius(float radius) 		{ _radius = radius; }
	void setJitter(float jitter) 		{ _jitter = jitter; }	

private:
	float 	_distance;
	float 	_radius;
    float 	_jitter;
	
    QVector3D 		_target;
};

#endif // WANDER_H
