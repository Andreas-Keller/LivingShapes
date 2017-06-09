#ifndef FORMATION_H
#define FORMATION_H

#include <QVector3D>
#include "Steering/arrive.h"

class MovingEntity;

class Formation {
public:
    Formation();
    Formation(MovingEntity* leader, QVector3D& offset);
    ~Formation();

    QVector3D update(MovingEntity* owner);
    void setLeader(MovingEntity* leader)    { _leader = leader; }
    void setOffset(const QVector3D& offset) { _offset = offset; }

private:
    MovingEntity* _leader;
    QVector3D     _offset;

    Arrive        _arrive;
};

#endif // FORMATION_H
