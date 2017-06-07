#ifndef MOVINGENTITY_H
#define MOVINGENTITY_H

#include "gameentity.h"

#include "Steering/Steering.h"

class Scene;

class MovingEntity : public GameEntity {
public:
    MovingEntity(
            Scene* scene,
            Shape* shape,
            float mass,
            const QVector3D& initialVelocity = QVector3D());

    virtual ~MovingEntity();

    virtual void update(int deltaTime) override;

    //setters:
    void setVelocity(const QVector3D& val) { _v = val; }
    void setAcceleration(const QVector3D& val) { _a = val; }

    void setMaxForce(float val)            { _maxF = val; }

    //getters:
    QVector3D velocity() const      { return _v; }
    QVector3D acceleration() const  { return _a; }
    float mass() const              { return _mass; }
    float maxForce() const          { return _maxF; }

    Steering* steering()            { return &_steering; }

    //debugging. Tracker will be switched off automaticly after one frame
    void track()                        { _tracker = true; }

protected:
    float _mass;
    QVector3D _v;       //velocity
    QVector3D _a;       //acceleration

    float     _maxF;    //the maximal force this entity can use on itself

    Steering  _steering;

    //for debugging:
    bool      _tracker;
};

#endif // MOVINGENTITY_H
