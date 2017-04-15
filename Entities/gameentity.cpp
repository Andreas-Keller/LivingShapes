#include "gameentity.h"

GameEntity::GameEntity(Shape *shape)
    : _shape{ shape }
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
    _shape->setMatrix(_transform.matrix());
}

void GameEntity::draw(QOpenGLShaderProgram* shader)
{
    if (_shape) _shape->draw(shader);
}
