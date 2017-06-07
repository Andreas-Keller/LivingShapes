/*  singleton class to draw debug information like force vectors
    heavy on performance, purely for debugging
*/

#ifndef DEBUGRENDERER_H
#define DEBUGRENDERER_H

#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include <vector>

class Camera;

class DebugRenderer {
public:
    static DebugRenderer* instance();

    void addLine(const QVector3D& start,
                 const QVector3D& end,
                 const QVector3D& color);

    void draw(Camera* cam);

private:
    DebugRenderer();

    std::vector<QVector3D> _lines;

    QOpenGLFunctions*           _gl;
    QOpenGLBuffer               _vbo;
    QOpenGLVertexArrayObject    _vao;
    QOpenGLShaderProgram*       _shader;
};

#endif // DEBUGRENDERER_H
