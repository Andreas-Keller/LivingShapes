#include "shape.h"

#include <QDebug>

Shape::Shape(QOpenGLShaderProgram *shader, const QColor& color) :
    _color      { QVector3D{ color.redF(), color.greenF(), color.blueF() } },
    _shader     { shader },
    _vbo        { QOpenGLBuffer::VertexBuffer },
    _ebo        { QOpenGLBuffer::IndexBuffer },
    _gl         { QOpenGLContext::currentContext()->functions() }
{
    _M.setToIdentity();
    Q_ASSERT(_shader);
    Q_ASSERT(_gl);
}


Shape::~Shape()
{}


void Shape::draw(QOpenGLShaderProgram *shader)
{
    //set model matrix uniform:
    int location = shader->uniformLocation("M");
    //if (location == -1) qDebug() << "<Shape::draw> Could not find uniform named \"M\".";
    _gl->glUniformMatrix4fv(location, 1, GL_FALSE, _M.data());

    //set color uniform:
    location = shader->uniformLocation("color");
    _gl->glUniform3f(location, _color.x(), _color.y(), _color.z());

    //enable attribute arrays:
    shader->enableAttributeArray(0);
    shader->enableAttributeArray(1);
    shader->enableAttributeArray(2);

    _vao.bind();

    //_gl->glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); //-->Not supported by OpenGL ES
    _gl->glDrawElements(GL_TRIANGLES, _numVertices, GL_UNSIGNED_INT, 0);

    _vao.release();
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
                Vertex::uvOffset(),
                2,
                Vertex::stride());

    //release all buffers:
    _vao.release();
    _ebo.release();
    _vbo.release();
    _shader->release();
}
