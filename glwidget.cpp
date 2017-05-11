#include "glwidget.h"
#include "Shapes/concreteshapes.h"
#include "Shapes/shapemaker.h"
#include "Entities/movingentity.h"
#include "Shapes/shapemaker.h"

#include <QDebug>
#include <QWheelEvent>

GLWidget::GLWidget(QWidget* parent, QColor clearColor /* = QColor::black */)
    :   QOpenGLWidget{ parent },
        _gl         { nullptr },
        _shader     { nullptr },
        _lightShader{ nullptr },
        _color      { clearColor },
        _cam        { this->width(), this->height() },
        _framebuffer{ nullptr }
{}


void GLWidget::mousePressEvent(QMouseEvent *event) {
    _mousePos = event->pos();
    setCursor(QCursor{ Qt::CursorShape::OpenHandCursor });
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    setCursor(QCursor{ Qt::CursorShape::ArrowCursor });
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {

    if (event->buttons() & Qt::LeftButton){
        QPoint delta = event->pos() - _mousePos;
        float dx = float(delta.x());
        float dy = float(delta.y());
        _cam.move(QVector3D{ dx, dy, 0.f });
    }
    else if (event->buttons() & Qt::RightButton) {
        QPoint delta = event->pos() - _mousePos;
        float dx =  float(delta.x());
        float dy = float(delta.y());
        _cam.rotate(dx + dy);
    }

    _mousePos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent *event) {

    QPoint degrees = event->angleDelta();

    //we ignore the size of the movment:
    if (degrees.y() == 0) return;
    else if (degrees.y() > 0) _cam.setZoom(_cam.zoom() + 20.f);
    else if (degrees.y() < 0) _cam.setZoom(_cam.zoom() - 20.f);
}

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

    //Create the shapes shader:
    _shader = new QOpenGLShaderProgram();
    _shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shaders/simple.vert.glsl");
    _shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shaders/simple.frag.glsl");
    _shader->link();

    //Create the light shader:
    _lightShader = new QOpenGLShaderProgram();
    _lightShader->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shaders/lights.vert.glsl");
    _lightShader->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shaders/lights.frag.glsl");
    _lightShader->link();

    //Give the shape shader to our ShapeMaker:
    ShapeMaker::instance()->setShader(_shader);

    //initialize the framebuffer with a const size:
    _framebuffer = new QOpenGLFramebufferObject{ 1024, 1024 };
    Q_ASSERT(_framebuffer->isValid());

    /* TEST CODE ----------------------------------------------------------------------------- */
    //generate a light:
    ShapeMaker::instance()->setShader(_lightShader);
    _lights.push_back(new Light{ "light", 1.f });

    //generate some pseudo-randomized game entities:*/
    ShapeMaker::instance()->setShader(_shader);
    for (size_t i = 0; i < 20; i++) {
        int type = rand() % 3;
        float dx = float(rand() % 13 - 6) / 2.f;
        float dy = float(rand() % 11 - 4) / 2.f;
        float w = float(rand() % 4 + 1) / 5.f;
        float h = float(rand() % 4 + 1) / 5.f;

        switch(type) {
        case 0:
            _entities.push_back(
                new GameEntity{ ShapeMaker::instance()->get(ShapeType::rectangle) });
            break;
        case 1:
            _entities.push_back(
                new GameEntity{ ShapeMaker::instance()->get(ShapeType::triangle) });
            break;
        case 2:
            _entities.push_back(
                new GameEntity{ ShapeMaker::instance()->get(ShapeType::circle) });
            break;
        }
        _entities.back()->transform()->setPos(QVector3D{ dx, -dy, 0.0 });
        _entities.back()->transform()->scale(QVector3D(w, h, 1.f));

    }

     /* END OF TEST CODE --------------------------------------------------------------------- */
}

void GLWidget::paintGL()
{
     /* TEST CODE ----------------------------------------------------------------------------- */
    //update our gameEntities (THIS SHOULD HAPPEN IN A LOOP SEPERATED FROM DRAWING)
    int flipDir = 1;
    for (size_t i = 0; i <_entities.size(); i++) {
        _entities[i]->update((i+1) * flipDir);
        flipDir *= -1;
    }
    /* END OF TEST CODE --------------------------------------------------------------------- */

    //DRAWING, PASS 1: Draw the lights:
    //_framebuffer->bind();
    _gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _lightShader->bind();

    setCameraUniforms(_lightShader);

    for (auto& L : _lights) L->draw(_lightShader);

    _lightShader->release();
    //_framebuffer->release();

/*
    //DRAWING, PASS 2: Draw the shapes:
    _gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _cam.update();

    //bind shader and set camera matrices:
    _shader->bind();
    setCameraUniforms(_shader);

    //draw our gameEntities:
    for (auto& E : _entities) E->draw(_shader);

    _shader->release();*/
}


void GLWidget::resizeGL(int width, int height)
{
    _gl->glViewport(0, 0, width, height);
    _cam.resize(width, height);
}


void GLWidget::setCameraUniforms(QOpenGLShaderProgram* shader) {
    _gl->glUniformMatrix4fv(shader->uniformLocation("O"), 1, GL_FALSE, _cam.orthoMatrix().data());
    _gl->glUniformMatrix4fv(shader->uniformLocation("V"), 1, GL_FALSE, _cam.viewMatrix().data());
}
