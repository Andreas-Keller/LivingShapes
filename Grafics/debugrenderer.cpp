#include "debugrenderer.h"

#include "camera.h"

DebugRenderer *DebugRenderer::instance() {
    static DebugRenderer instance;
    return &instance;
}

void DebugRenderer::addLine(const QVector3D &start,
                            const QVector3D &end,
                            const QVector3D &color)
{
    _lines.push_back(start);
    _lines.push_back(color);
    _lines.push_back(end);
    _lines.push_back(color);
}

void DebugRenderer::draw(Camera *cam) {
    //get the size of our data:
    size_t size = _lines.size() * 3 * sizeof(float);
    size_t stride = 2 * 3 * sizeof(float);
    size_t posOffset = 0;
    size_t colorOffset = 3 * sizeof(float);

    //bind buffers:
    _shader->bind();
    _vao.bind();
    _vbo.bind();

    //buffer data with every single draw call:
    _vbo.allocate(_lines.data(), size);
    _shader->enableAttributeArray(0);
    _shader->setAttributeBuffer(
                0,
                GL_FLOAT,
                posOffset,
                3,
                stride);
    _shader->enableAttributeArray(1);
    _shader->setAttributeBuffer(
                1,
                GL_FLOAT,
                colorOffset,
                3,
                stride);

    //draw:
    _gl->glEnable(GL_BLEND);

    if (!_lines.empty()) {
        _gl->glUniformMatrix4fv(
                    _shader->uniformLocation("O"),
                    1,
                    GL_FALSE,
                    cam->orthoMatrix().data());
        _gl->glUniformMatrix4fv(
                    _shader->uniformLocation("V"),
                    1,
                    GL_FALSE,
                    cam->viewMatrix().data());
        _gl->glDrawArrays(GL_LINES, 0, _lines.size() >> 1);
    }

    _vbo.release();
    _vao.release();
    _shader->release();

    //delete the data:
    _lines.clear();
}


DebugRenderer::DebugRenderer()
    :   _vbo        { QOpenGLBuffer::VertexBuffer },
        _gl         { QOpenGLContext::currentContext()->functions() }
{
    if (!_vbo.create()) qDebug() << "DebugRenderer: unabel to create vertex buffer!";
    if (!_vao.create()) qDebug() << "DebugRenderer: unable to create vertex array!";

    _vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    _shader = new QOpenGLShaderProgram();
    _shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shaders/debug.vert.glsl");
    _shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shaders/debug.frag.glsl");
    _shader->link();
}

