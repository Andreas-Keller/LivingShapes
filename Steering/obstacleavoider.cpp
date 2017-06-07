#include "obstacleavoider.h"

#include "Entities/movingentity.h"
#include "Grafics/debugrenderer.h"
#include "Scene.h"

#include "math.h"

ObstacleAvoider::ObstacleAvoider()
{}

ObstacleAvoider::~ObstacleAvoider()
{}


/*  see Buckland, page 99pp for a detailed explanation of what we do here
    note that in our case, a shape in its origin points along the y-Axis,
    while in Bucklands case, it points along the x-Axis
*/
QVector3D ObstacleAvoider::update(MovingEntity *owner, Scene *scene) {

    /* STEP A: FIND THE CLOSEST OBSTACLE THE OWNER WILL COLLIDE WITH: */

    //first we calculate the braking distance of this entity:
    float maxAcceleration = owner->maxForce() / owner->mass();
    float speedSquared = owner->velocity().lengthSquared();
    float brakingDistance = speedSquared / (2.f * maxAcceleration);

    //increase braking distance (the entity will never turn if we dont):
    brakingDistance += 1.f;

    //now we need a list of all entities within the braking distance:
    std::vector<GameEntity*> objsInRange = scene->tagObstacles(owner, brakingDistance);

    //keep track of the distance and position of the clostest object:
    GameEntity* clostesObj = nullptr;
    float closestDistance = std::numeric_limits<float>::max();
    QVector3D localPos{ };

    //loop through all possible obstacles:
    QMatrix4x4 M = owner->transform()->matrix().inverted();
    owner->aabb()->update(M);

    for (auto current = objsInRange.begin(); current != objsInRange.end(); current++)
    {
        //get obstacles local Pos:
        QVector3D local = M * (*current)->transform()->pos();

        //discard object if it is behind the owner:
        if (local.y() < 0) continue;

        //else we need a more detailed test:
        (*current)->aabb()->update(M);
        float radius = (*current)->aabb()->radius() + owner->aabb()->radius();

        //if we have a defintive collision, we need to find its distance from owner:
        if (std::fabs(local.x()) < radius) {
            float square = sqrt(radius*radius - local.x() * local.x());
            float distance = local.y() - square;
            if (distance < 0) distance = local.y() + square;

            //check if this is the clostest intersection found so far:
            if (distance < closestDistance) {
                closestDistance = distance;
                clostesObj = (*current);
                localPos = local;
            }
        }
    }


    /* STEP B: CALCULATE A STEERING FORCE AWAY FROM THE OBSTACLE: */
    QVector3D force;

    if (!clostesObj) return force;
    //calculate a multiplier which increases the closer the two objs are:
    float multiplier = 1.f + (brakingDistance - localPos.y()) / brakingDistance;

    force.setX((clostesObj->aabb()->radius() - localPos.x()) * multiplier);
    force.setY((clostesObj->aabb()->radius() - localPos.y()) * 10 * multiplier);

    QVector3D ret = owner->transform()->matrix() * force;

    return ret;
}
