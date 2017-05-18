/*  Class ConcreteShapes
    Here we define shapes like Rectangles and Triangles, which all inherit from
    class Shape
*/

#ifndef CONCRETESHAPES_H
#define CONCRETESHAPES_H

#include "shape.h"

#include <vector>
#include <QVector2D>
#include <QColor>

class Rectangle : public Shape {
public:
    Rectangle(
        QOpenGLShaderProgram* shader,
        const QVector2D& widthAndHeight,
        const QColor& color = QColor(150, 150, 150));

    ~Rectangle();

private:
    virtual void initVertices(std::vector<Vertex> &vertices,
                              std::vector<int> &indices) override;

    QVector2D _widthAndHeight;
};



class TriangleEqualSided : public Shape {
public:
    TriangleEqualSided(
        QOpenGLShaderProgram* shader,
        const QVector2D& widthAndHeight,
        const QColor& color = QColor(150, 150, 150));

    ~TriangleEqualSided();

private:
    virtual void initVertices(std::vector<Vertex> &vertices,
                              std::vector<int> &indices) override;

    QVector2D _widthAndHeight;
};


class Circle : public Shape {
public:
    Circle(
        QOpenGLShaderProgram* shader,
        float radius, int segments,
        const QColor& color = QColor(150, 150, 150));

    ~Circle();

private:
    virtual void initVertices(std::vector<Vertex> &vertices,
                              std::vector<int> &indices) override;

   void segmentBreakup(std::vector<Vertex>& vertices);

    float _r;
    int   _segments;
};


class Line : public Shape {
public:
    Line(QOpenGLShaderProgram* shader, const QVector3D& start, const QVector3D &end,
         const QColor& color = QColor(150, 150, 150));
    ~Line();

    //need to override the draw function here:
    virtual void draw(QOpenGLShaderProgram *shader) override;

private:
    virtual void initVertices(std::vector<Vertex> &vertices,
                              std::vector<int> &indices) override;

    //the line is represented in the form:
    //points = start + k * direction, with a k(max) for the end point:
    QVector3D   _start;
    QVector3D   _dir;
    float       _kMax;
};

#endif // CONCRETESHAPES_H
