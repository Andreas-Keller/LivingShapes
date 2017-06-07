/*	Class Steering 
	Base class for calculating Steering Forces for MovingEntities
*/

#pragma once

#include <QVector3D>

#include "Scene.h"
#include "wander.h"
#include "obstacleavoider.h"

class MovingEntity;

class Steering {
public:
    Steering(MovingEntity* owner, Scene* scene);
	~Steering();

    QVector3D update(size_t timestep, bool tracker = false);

    //Switches for every steering behaviour we have:
    void    wanderOn()              { _wanderOn = true; }
    void    wanderOff()             { _wanderOn = false; }
    void    obstacleAvoidanceOn()   { _avoiderOn = true; }
    void    obstacleAvoidanceOff()  { _avoiderOn = false; }

    //set update time:
    void    setUpdateTime(size_t val) { _updateTime = val; }

private:
    //steering behaviours:
    Wander          _wander;
    ObstacleAvoider _avoider;

    //bools for turning behaviours on and off:
    bool   _wanderOn;
    bool   _avoiderOn;

    MovingEntity*   _owner;
    Scene*          _scene;

    QVector3D       _force;

    size_t          _updateTime; //we need an independent update time
    size_t          _accumTime;
};
