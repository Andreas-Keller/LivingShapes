#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include <QOpenGLShaderProgram>

#include "Shapes/shape.h"
#include "Shapes/concreteshapes.h"
#include "transform.h"
#include "aabb.h"

class GameEntity {
public:
    GameEntity(Shape* shape);
    virtual ~GameEntity();

    //@param deltaTime: the time that has passed since the last update in miliseconds
    virtual void update(int deltaTime);

    virtual void draw(QOpenGLShaderProgram* shader);

    //getters and setters:
    Shape* shape()                      { return _shape; }
    Transform* transform()              { return &_transform; }

protected:
    Shape* _shape;                      //the graphical representation of this entity
    Transform _transform;               //holds position, rotation and scale
    AABB _aabb;                         //axis-aligned-bounding-box for collision checks

    AABBShape _aabbShape;               //purely for debugging
};

#endif // GAMEENTITY_H
