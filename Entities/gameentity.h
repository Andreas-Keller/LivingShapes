#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include <QOpenGLShaderProgram>

#include "Shapes/shape.h"
#include "Shapes/concreteshapes.h"
#include "transform.h"
#include "aabb.h"

//enum class to identify different types of entities
enum class EntType {
    base,
    moving,
    light
};

class GameEntity {
public:
    GameEntity(Shape* shape, EntType type = EntType::base);
    virtual ~GameEntity();

    //@param deltaTime: the time that has passed since the last update in miliseconds
    virtual void update(int deltaTime);

    virtual void draw(QOpenGLShaderProgram* shader);

    //getters and setters:
    Shape* shape()                      { return _shape; }
    Transform* transform()              { return &_transform; }

    EntType type() const                { return _type; }

protected:
    Shape* _shape;                      //the graphical representation of this entity
    Transform _transform;               //holds position, rotation and scale
    AABB _aabb;                         //axis-aligned-bounding-box for collision checks

    EntType _type;

    AABBShape _aabbShape;               //purely for debugging
};

#endif // GAMEENTITY_H
