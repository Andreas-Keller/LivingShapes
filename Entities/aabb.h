/*  Class for an axis-aligned-boundingbox, used to check for possible collisions
    between our objects */

#ifndef AABB_H
#define AABB_H

#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <vector>

#include "transform.h"

class AABB {
public:
    AABB();
    AABB(const std::vector<QVector3D>& vertices);
    ~AABB();

    void update(QMatrix4x4 transform);

    void construct(const std::vector<QVector3D> vertices);

    /*  Tests if two aabbs intersect or not.
        @param intersectionBox: contains the aabb formed by the intersection
        if delivered
    */
    static bool intersection(
            const AABB& box1, const AABB& box2, AABB* intersectionBox = nullptr);

    bool      isEmpty() const { return (_min.x() > _max.x() || _min.y() > _max.y()); }
    QVector2D min() const   { return _min; }
    QVector2D max() const   { return _max; }

private:
    QVector2D _maxO;    //original max and min
    QVector2D _minO;
    QVector2D _max;     //max and min after transformations
    QVector2D _min;
};


/* a class for debugging only: draw the AABBs: */

#include "Shapes/shape.h"
#include <QOpenGLShaderProgram>
#include <QColor>
class AABBShape : public Shape {
public:
    AABBShape(QOpenGLShaderProgram* shader, const AABB& aabb);

    virtual void draw(QOpenGLShaderProgram *shader) override;

    //reupload all vertices:
    void update();

private:
    void initVertices(std::vector<Vertex>& vertices,
                      std::vector<int>& indices) override;

    const AABB& _aabb;
};


#endif // AABB_H
