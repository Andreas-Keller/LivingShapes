#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include <QTransform>
#include <QOpenGLShaderProgram>

#include "Shapes/shape.h"
#include "Shapes/concreteshapes.h"

class GameEntity {
public:
    GameEntity(Shape* shape);
    virtual ~GameEntity();

    //@param deltaTime: the time that has passed since the last update in miliseconds
    virtual void update(int deltaTime);

    virtual void draw(QOpenGLShaderProgram* shader);

    //getters and setters:
    Shape* shape()                     { return _shape; }
    Qt3DCore::QTransform* transform()  { return &_transform; }

protected:
    Shape* _shape;                      //the graphical representation of this entity
    Qt3DCore::QTransform _transform;    //holds position, rotation and scale
};

#endif // GAMEENTITY_H
