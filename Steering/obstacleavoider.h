#ifndef OBSTACLEAVOIDER_H
#define OBSTACLEAVOIDER_H

#include <QVector3D>

class MovingEntity;
class Scene;

class ObstacleAvoider {
public:
    ObstacleAvoider();
    ~ObstacleAvoider();

    QVector3D update(MovingEntity* owner, Scene* scene);

private:

};

#endif // OBSTACLEAVOIDER_H
