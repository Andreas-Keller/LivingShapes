/*	Class Steering 
	Base class for calculating Steering Forces for MovingEntities
*/

#pragma once

#include <QVector3D>

#include "Scene.h"
#include "wander.h"
#include "obstacleavoider.h"
#include "arrive.h"
#include "formation.h"

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
    void    arriveOn()              { _arriveOn = true; }
    void    arriveOff()             { _arriveOn = false; }
    void    formationOn()           { _formationOn = true; }
    void    formationOff()          { _formationOn = false; }

    //set update time:
    void    setUpdateTime(size_t val) { _updateTime = val; }

    //getters to set values of single steering behaviours:
    Arrive* arrive()                { return &_arrive; }
    Wander* wander()                { return &_wander; }
    Formation* formation()          { return &_formation; }

    //getters for the behaviour switches:
    bool    isWanderOn()              { return _wanderOn; }
    bool    isObstacleAvoidanceOn()   { return _avoiderOn; }
    bool    isArriveOn()              { return _arriveOn; }
    bool    isFormationOn()           { return _formationOn; }

private:
    //steering behaviours:
    Wander          _wander;
    ObstacleAvoider _avoider;
    Arrive          _arrive;
    Formation       _formation;

    //bools for turning behaviours on and off:
    bool   _wanderOn;
    bool   _avoiderOn;
    bool   _arriveOn;
    bool   _formationOn;

    MovingEntity*   _owner;
    Scene*          _scene;

    QVector3D       _force;

    size_t          _updateTime; //we need an independent update time
    size_t          _accumTime;
};
