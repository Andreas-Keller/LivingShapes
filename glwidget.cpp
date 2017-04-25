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


    /* TEST CODE ----------------------------------------------------------------------------- */
    _shader = new QOpenGLShaderProgram();
    _shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shaders/simple.vert.glsl");
    _shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shaders/simple.frag.glsl");
    _shader->link();

    //Play around with this to see what we have so far:
    _entities.push_back(
        new GameEntity{ new Rectangle(_shader, QVector2D{0.2, 0.8}, QColor(200, 0, 100)) });
    _entities.back()->transform()->setPos(QVector3D{ -1.0, -0.5, 0.0 });

    _entities.push_back(
        new GameEntity{ new TriangleEqualSided(_shader, QVector2D{0.3, 0.3}, QColor(100, 0, 200)) });

    _entities.push_back(
        new GameEntity{ new TriangleEqualSided(_shader, QVector2D{0.3, 0.5}, QColor(50, 50, 200)) });
    _entities.back()->transform()->setPos(QVector3D{ 1.0, 1.0, 0.0 });

    _entities.push_back(
        new GameEntity{ new Rectangle(_shader, QVector2D{0.4, 0.4}, QColor(50, 200, 50)) });
    _entities.back()->transform()->setPos(QVector3D{ 0.5, -0.5, 0.0 });

     /* END OF TEST CODE --------------------------------------------------------------------- */
}


void GLWidget::paintGL()
{
    _gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _cam.update();

    //bind shader and set camera matrices:
    _shader->bind();
    _gl->glUniformMatrix4fv(_shader->uniformLocation("O"), 1, GL_FALSE, _cam.orthoMatrix().data());
    _gl->glUniformMatrix4fv(_shader->uniformLocation("V"), 1, GL_FALSE, _cam.viewMatrix().data());

    /* TEST CODE ----------------------------------------------------------------------------- */
    //update our gameEntities (THIS SHOULD HAPPEN IN A LOOP SEPERATED FROM DRAWING)
    int flipDir = 1;
    for (size_t i = 0; i <_entities.size(); i++) {
        _entities[i]->update((i+1) * flipDir);
        flipDir *= -1;
    }

    //draw our gameEntities:
    for (auto& E : _entities) E->draw(_shader);

    _shader->release();
    /* END OF TEST CODE --------------------------------------------------------------------- */
}


void GLWidget::resizeGL(int width, int height)
{
    _gl->glViewport(0, 0, width, height);
    _cam.resize(width, height);
}

