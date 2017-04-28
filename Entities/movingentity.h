#ifndef MOVINGENTITY_H
#define MOVINGENTITY_H

#include "gameentity.h"

class MovingEntity : public GameEntity {
public:
    MovingEntity(Shape* shape, float mass, const QVector3D& initialVelocity = QVector3D());
    virtual ~MovingEntity();

    virtual void update(int deltaTime) override;

    //getters:
    QVector3D velocity()    { return _v; }
    QVector3D acceleration(){ return _a; }
    float mass()            { return _mass; }

protected:
    float _mass;
    QVector3D _v;   //velocity
    QVector3D _a;   //acceleration


};

#endif // MOVINGENTITY_H
