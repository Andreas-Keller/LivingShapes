#include "gameentity.h"

GameEntity::GameEntity(Shape *shape, EntType type)
    : _shape    { shape },
      _aabb     { Vertex::extractPositions(_shape->vertices()) },
      _type     { type }//,
      //_aabbShape{ _shape->shader(), _aabb }
{
    Q_ASSERT(_shape);
}

GameEntity::~GameEntity() {
    //no need to delte, since scene destructor already does
}

void GameEntity::update(int deltaTime)
{
    _aabb.update(_transform.matrix());
    //_aabbShape.update();
}

void GameEntity::draw(QOpenGLShaderProgram* shader)
{
    if (_shape) {
        _shape->setMatrix(_transform.matrix());
        _shape->draw(shader);
    }
    //_aabbShape.draw(shader);
}
