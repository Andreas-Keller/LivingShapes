#include "glwidget.h"
#include "Shapes/concreteshapes.h"

#include <QDebug>

GLWidget::GLWidget(QWidget* parent, QColor clearColor /* = QColor::black */)
    :   QOpenGLWidget{ parent },
        _gl     { nullptr },
        _color  { clearColor },
        _cam    { this->width(), this->height() }
{}

void GLWidget::initializeGL()
{
    if (!_gl) {
        _gl = QOpenGLContext::currentContext()->functions();
    }
    _gl->glClearColor(_color.redF(), _color.greenF(), _color.blueF(), _color.alphaF());
    _gl->glViewport(0, 0, this->width(), this->height());


    //TEST CODE -----------------------------------------
    _shader = new QOpenGLShaderProgram();
    _shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shaders/simple.vert.glsl");
    _shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shaders/simple.frag.glsl");
    _shader->link();

    //Play around with this to see what we have so far:
    _shapes.push_back(new Rectangle(_shader, QVector2D{0.2, 0.8}, QColor(200, 0, 100)));
    _shapes.push_back(new TriangleEqualSided(_shader, QVector2D{0.3, 0.3}, QColor(100, 0, 200)));
}


void GLWidget::paintGL()
{
    _gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _cam.update();

    //bind shader and set camera matrices:
    _shader->bind();
    _gl->glUniformMatrix4fv(_shader->uniformLocation("O"), 1, GL_FALSE, _cam.orthoMatrix().data());
    _gl->glUniformMatrix4fv(_shader->uniformLocation("V"), 1, GL_FALSE, _cam.viewMatrix().data());

    qDebug() << _cam.orthoMatrix();
    qDebug() << _cam.viewMatrix();

    //draw our shapes:
    for (auto& S : _shapes) S->draw(_shader);

    _shader->release();

    //force another update (NOT sure if this is the right way to do it):
    update();
}


void GLWidget::resizeGL(int width, int height)
{
    _gl->glViewport(0, 0, width, height);
    _cam.resize(width, height);
}

