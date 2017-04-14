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
                              std::vector<int> &indices,
                              const QColor& color) override;

    QVector2D _widthAndHeight;
};



#endif // CONCRETESHAPES_H
