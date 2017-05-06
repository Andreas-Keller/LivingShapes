/*  Singleton Class Shapemaker

    Purpose: Store standard shapes only once in memory and give each Instance of GameEntity a Pointer to
    this standard shape
    We can still make the standard shapes more individual by applying a transform

*/

#ifndef SHAPEMAKER_H
#define SHAPEMAKER_H

#include <QOpenGLShaderProgram>

#include "concreteshapes.h"

/* enum class to define all valid shapes: */
enum class ShapeType {
    rectangle,
    triangle,
    circle
};

class ShapeMaker {
public:
    static ShapeMaker* instance(QOpenGLShaderProgram* shader);

    ~ShapeMaker();

    Shape* get(ShapeType type);

private:
    ShapeMaker(QOpenGLShaderProgram* shader);

    Shape* _rectangle;
    Shape* _triangle;
    Shape* _circle;

    QOpenGLShaderProgram* _shader;
};

#endif // SHAPEMAKER_H