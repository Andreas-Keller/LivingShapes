/*	Class Steering 
	Base class for calculating Steering Forces for MovingEntities
*/

#pragma once

#include <QVector3D>

#include "Scene.h"
#include "wander.h"

class MovingEntity;

class Steering {
public:
    Steering(MovingEntity* owner, Scene* scene);
	~Steering();

    QVector3D update();

    //Switches for every steering behaviour we have:
    void    wanderOn()  { _wanderOn = true; }
    void    wanderOff() { _wanderOn = false; }

private:
    Wander _wander;
    bool   _wanderOn;

    MovingEntity*   _owner;
    Scene*          _scene;
};
