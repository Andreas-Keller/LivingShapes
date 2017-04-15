/*  Class Shape

    Abstract Base class for drawing our shapes onto an OpenGL-Widget
    openGL-Context MUST be active before any instance of this class is created
*/

#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>

/* helper struct: */
struct Vertex {
    Vertex() {}
    Vertex(const QVector3D& position, const QVector3D& color) : pos{ position }, rgb{ color } {}

    static int posOffset() { return offsetof(Vertex, pos); }
    static int rgbOffset() { return offsetof(Vertex, rgb); }
    static int stride()    { return sizeof(Vertex); }

    QVector3D pos;
    QVector3D rgb;
};

class Shape {

public:
    Shape(QOpenGLShaderProgram* shader);
    virtual ~Shape();

    void draw();
    void draw(QOpenGLShaderProgram* shader);

    //set the model matrix:
    void setMatrix(const QMatrix4x4& matrix)    { _M = matrix; }

protected:
    //MUST be defined in subclasses:
    virtual void initVertices(std::vector<Vertex>& vertices,
                              std::vector<int>& indices,
                              const QColor& color) = 0;

    virtual void initBuffers(std::vector<Vertex>& vertices,
                             std::vector<int>& indices);

    //openGF-Stuff:
    QOpenGLShaderProgram*   _shader;
    QOpenGLBuffer           _vbo;   //vertex buffer object
    QOpenGLBuffer           _ebo;   //index buffer
    QOpenGLVertexArrayObject _vao;  //vertex array object

    QOpenGLFunctions*       _gl;    //pointer to openGL-Functions

    std::vector<Vertex>     _vertices;
    int                     _numVertices;

    //Model Matrix:
    QMatrix4x4              _M;
};


#endif // SHAPE_H
