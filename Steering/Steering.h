/*	Class Steering 
	Base class for calculating Steering Forces for MovingEntities
*/

#pragma once

#include <QVector2D>

#include "Entities/movingentity.h"
#include "Scene.h"


class Steering {
public:
	Steering(Scene* scene);
	~Steering();

    QVector2D update();

    //Switches for every steering behaviour we have:
    void    wanderOn()  { _wanderOn = true; }
    void    wanderOff() { _wanderOn = false; }

private:
    bool   _wanderOn;


	Scene* _scene;
};
