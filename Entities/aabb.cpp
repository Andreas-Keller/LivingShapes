#include "aabb.h"
#include <math.h>


AABB::AABB() {

    float min = std::numeric_limits<float>::min();
    float max = std::numeric_limits<float>::max();
    _minO = QVector2D{ max, max };
    _maxO = QVector2D{ min, min };
    _min = _minO;
    _max = _maxO;
}

AABB::AABB(const std::vector<QVector3D>& vertices) {
    construct(vertices);
}

AABB::~AABB()
{}


void AABB::construct(const std::vector<QVector3D> vertices) {

    float min = std::numeric_limits<float>::min();
    float max = std::numeric_limits<float>::max();

    _minO = QVector2D{ max, max };
    _maxO = QVector2D{ min, min };

    for (const auto& vertex : vertices) {
        if (vertex.x() < _minO.x()) _minO.setX(vertex.x());
        if (vertex.x() > _maxO.x()) _maxO.setX(vertex.x());
        if (vertex.y() < _minO.y()) _minO.setY(vertex.y());
        if (vertex.y() > _maxO.y()) _maxO.setY(vertex.y());
    }

    _min = _minO;
    _max = _maxO;
}

/* this is a straight adaption from a game math book: */
void AABB::update(QMatrix4x4 M) {

    _min = _max = getTranslation(M);

    /* matrix first column (QMatrix has column-row reversed !) */
    if (M(0,0) > 0.0f) {
        _min.setX(_min.x() + M(0,0) * _minO.x());
        _max.setX(_max.x() + M(0,0) * _maxO.x());
    }
    else {
        _min.setX(_min.x() + M(0,0) * _maxO.x());
        _max.setX(_max.x() + M(0,0) * _minO.x());
    }
    if (M(0,1) > 0.0f) {
        _min.setY(_min.y() + M(0,1) * _minO.x());
        _max.setY(_max.y() + M(0,1) * _maxO.x());
    }
    else {
        _min.setY(_min.y() + M(0,1) * _maxO.x());
        _max.setY(_max.y() + M(0,1) * _minO.x());
    }

    //Second column:
    if (M(1,0) > 0.0f) {
        _min.setX(_min.x() + M(1,0) * _minO.y());
        _max.setX(_max.x() + M(1,0) * _maxO.y());
    }
    else {
        _min.setX(_min.x() + M(1,0) * _maxO.y());
        _max.setX(_max.x() + M(1,0) * _minO.y());
    }
    if (M(1,1) > 0.0f) {
        _min.setY(_min.y() + M(1,1) * _minO.y());
        _max.setY(_max.y() + M(1,1) * _maxO.y());
    }
    else {
        _min.setY(_min.y() + M(1,1) * _maxO.y());
        _max.setY(_max.y() + M(1,1) * _minO.y());
    }
}


bool AABB::intersection(const AABB &box1, const AABB &box2, AABB *intersectionBox) {

    if (box1.max().x() < box2.min().x()) return false;
    if (box1.max().y() < box2.min().y()) return false;
    if (box1.min().x() > box2.max().x()) return false;
    if (box1.min().y() > box2.max().y()) return false;

    //we have an verlap. Calculate intersection if caller gave an intersection box:
    if (!intersectionBox) return true;

    QVector3D min{ };
    QVector3D max{ };

    min.setX(fmax(box1.min().x(), box2.min().x()));
    min.setY(fmax(box1.min().y(), box2.min().y()));
    max.setX(fmin(box1.max().x(), box2.max().x()));
    max.setY(fmin(box1.max().y(), box2.max().y()));

    std::vector<QVector3D> points(2);
    points[0] = min;
    points[1] = max;

    intersectionBox->construct(points);

    return true;
}


/* Purely for debugging: */
AABBShape::AABBShape(QOpenGLShaderProgram *shader, const AABB &aabb)
    : Shape{ shader },
      _aabb{ aabb }
{
    std::vector<Vertex> vertices;
    std::vector<int> indices;

    QColor color{ 250, 0, 0 };

    initVertices(vertices, indices, color);
    initBuffers(vertices, indices);

    //we may need the edge points:
    _vertices = vertices;

    //and we certainly need the number of indices:
    _numVertices = indices.size();
}

void AABBShape::initVertices(std::vector<Vertex> &vertices, std::vector<int> &indices, const QColor &color) {
    vertices.resize(4);
    vertices[0].pos = QVector3D{ _aabb.min() };                             //bottom left
    vertices[1].pos = QVector3D{ _aabb.max().x(), _aabb.min().y(), 0.0f };  //bottom right
    vertices[2].pos = QVector3D{ _aabb.max() };                             //top right
    vertices[3].pos = QVector3D{ _aabb.min().x(), _aabb.max().y(), 0.0f };  //top left

    indices.resize(6);
    indices = { 0, 1, 3, 1, 2, 3 };
}
