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
#include <QColor>

/* helper struct: */
struct Vertex {
    Vertex() {}
    Vertex(const QVector3D& position) : pos{ position } {}

    //extract position vectors from an array of vertices:
    static std::vector<QVector3D> extractPositions(const std::vector<Vertex>& vertices) {
        std::vector<QVector3D> positions;
        for (const auto& vertex : vertices) {
            positions.push_back(vertex.pos);
        }
        return positions;
    }

    static int posOffset() { return offsetof(Vertex, pos); }
    static int uvOffset()  { return offsetof(Vertex, uv); }
    static int stride()    { return sizeof(Vertex); }

    QVector3D pos;
    QVector2D uv;
};


class Shape {

public:
    Shape(QOpenGLShaderProgram* shader, const QColor& color = QColor(150, 150, 150));
    virtual ~Shape();

    virtual void draw(QOpenGLShaderProgram* shader);

    //set the model matrix:
    void setMatrix(const QMatrix4x4& matrix)    { _M = matrix; }

    //get the vertices:
    const std::vector<Vertex>& vertices() const { return _vertices; }

    //getter and setter for color:
    QVector3D color() const                     { return _color; }
    void      setColor(const QVector3D& color)  { _color = color; }

    //ugly ad hoc solution for debugging the aabbs:
    QOpenGLShaderProgram* shader() { return _shader; }

protected:
    //MUST be defined in subclasses:
    virtual void initVertices(std::vector<Vertex>& vertices,
                              std::vector<int>& indices) = 0;

    virtual void initBuffers(std::vector<Vertex>& vertices,
                             std::vector<int>& indices);

    QVector3D               _color;

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
