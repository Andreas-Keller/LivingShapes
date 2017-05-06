#include "gameentity.h"

GameEntity::GameEntity(Shape *shape)
    : _shape    { shape },
      _aabb     { Vertex::extractPositions(_shape->vertices()) },
      _aabbShape{ _shape->shader(), _aabb }
{
    Q_ASSERT(_shape);
}

GameEntity::~GameEntity()
{
    if (_shape) {
        delete _shape;
        _shape = nullptr;
    }
}

void GameEntity::update(int deltaTime)
{
    //here we can start applying ai-stuff
    //for now just a test:
    _transform.setRotationZ(_transform.rotationZ() + float(deltaTime / 10.0));

    _aabb.update(_transform.matrix());
    _aabbShape.update();
}

void GameEntity::draw(QOpenGLShaderProgram* shader)
{
    if (_shape) {
        _shape->setMatrix(_transform.matrix());
        _shape->draw(shader);
    }
    _aabbShape.draw(shader);
}
