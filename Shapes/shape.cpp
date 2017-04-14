#include "shape.h"

#include <QDebug>

Shape::Shape(QOpenGLShaderProgram *shader) :
    _shader     { shader },
    _vbo        { QOpenGLBuffer::VertexBuffer },
    _ebo        { QOpenGLBuffer::IndexBuffer },
    _gl         { QOpenGLContext::currentContext()->functions() }
{
    Q_ASSERT(_shader);
    Q_ASSERT(_gl);
}


Shape::~Shape()
{}


void Shape::draw()
{
    draw(_shader);
}

void Shape::draw(QOpenGLShaderProgram *shader)
{
    shader->bind();
    _shader->enableAttributeArray(0);
    _shader->enableAttributeArray(1);
    _vao.bind();

    glDrawElements(GL_TRIANGLES, _numVertices, GL_UNSIGNED_INT, 0);

    _vao.release();
    shader->release();
}


void Shape::initBuffers(std::vector<Vertex> &vertices, std::vector<int> &indices)
{
    //create openGL Buffers:
    if (!_vao.create()) qDebug() << "<Shape::initBuffers> Unable to create vertex array object.";
    if (!_ebo.create()) qDebug() << "<Shape::initBuffers> Unable to create index buffer.";
    if (!_vbo.create()) qDebug() << "<Shape::initBuffers> Unable to create vertex buffer object.";

    //bind buffers and upload data:
    _shader->bind();
    _vao.bind();

    _vbo.bind();
    _vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _vbo.allocate(vertices.data(), sizeof(Vertex) * vertices.size());
    _ebo.bind();
    _ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _ebo.allocate(indices.data(), sizeof(int) * indices.size());

    //set attribute pointer:
    _shader->enableAttributeArray(0);
    _shader->setAttributeBuffer(
                0,
                GL_FLOAT,
                Vertex::posOffset(),
                3,
                Vertex::stride());
    _shader->enableAttributeArray(1);
    _shader->setAttributeBuffer(
                1,
                GL_FLOAT,
                Vertex::rgbOffset(),
                3,
                Vertex::stride());

    //release all buffers:
    _vao.release();
    _ebo.release();
    _vbo.release();
    _shader->release();
}
