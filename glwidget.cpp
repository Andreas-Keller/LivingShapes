#include "glwidget.h"
#include "Shapes/concreteshapes.h"
#include "Entities/movingentity.h"

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
    _gl->glEnable(GL_BLEND);
    //_gl->glBlendFunc(GL_ONE, GL_ONE);
    _gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* TEST CODE ----------------------------------------------------------------------------- */
    _shader = new QOpenGLShaderProgram();
    _shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shaders/simple.vert.glsl");
    _shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shaders/simple.frag.glsl");
    _shader->link();


    //generate some pseudo-randomized game entities:*/
    for (size_t i = 0; i < 12; i++) {
        int type = rand() % 3;
        float dx = float(rand() % 9 - 4) / 2.f;
        float dy = float(rand() % 5 - 2) / 2.f;
        float w = float(rand() % 4 + 1) / 5.f;
        float h = float(rand() % 4 + 1) / 5.f;
        float scale = float(rand() % 4 + 1) / 2.f;
        int r = rand() % 256;
        int g = rand() % 256;
        int b = rand() % 256;

        switch(type) {
        case 0:
            _entities.push_back(
                new GameEntity{ new Rectangle(_shader, QVector2D{w, h}, QColor(r, g, b)) });
            break;
        case 1:
            _entities.push_back(
                new GameEntity{ new TriangleEqualSided(_shader, QVector2D{w, h}, QColor(r, g, b)) });
            break;
        case 2:
            _entities.push_back(
                new GameEntity{ new Circle(_shader, 1.f, 6, QColor(r, g, b))});
            break;
        }
        _entities.back()->transform()->setPos(QVector3D{ dx, -dy, 0.0 });
        _entities.back()->transform()->scale(scale);

    }

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

