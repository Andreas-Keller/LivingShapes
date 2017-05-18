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
        _color      { clearColor },
        _cam        { this->width(), this->height() },
        _fbLight    { nullptr },
        _fbScene    { nullptr },
        _screenQuad { nullptr },
        _shader     { nullptr },
        _lightShader{ nullptr },
        _sceneShader{ nullptr },
        _gauss      { nullptr },
        _lightmapScale { 0.5f }
{}


void GLWidget::mousePressEvent(QMouseEvent *event) {
    _mousePos = event->pos();
    setCursor(QCursor{ Qt::CursorShape::OpenHandCursor });
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    setCursor(QCursor{ Qt::CursorShape::ArrowCursor });

    /* test: Get the world coordinate of the mouse: */
    float x = float(_mousePos.x()) / (0.5f*float(this->width())) - 1.f;
    float y = float(_mousePos.y()) / (0.5f*float(this->height())) - 1.f;

    qDebug() << "Mouse in openGL-Coordinates: " << QVector3D(x, y, 0.f);

    QMatrix4x4 I = (_cam.orthoMatrix() * _cam.viewMatrix()).inverted();

    QVector3D mouse = QVector3D{x, -y, 0.f };

    QVector3D mouseWorld = I * mouse;
    qDebug() << "Mouse in world coordiantes: " << mouseWorld;

    //visualize:
    _newEntPos.push_back(mouseWorld);
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
    _gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initShaders();

    //Give the shape shader to our ShapeMaker:
    ShapeMaker::instance()->setShader(_shader);

    //initialize the framebuffer with a const size:
    _fbLight = new QOpenGLFramebufferObject{
            this->width() * _lightmapScale,
            this->height()* _lightmapScale};
    _fbScene = new QOpenGLFramebufferObject{ this->width(), this->height() };

    Q_ASSERT(_fbLight->isValid());
    Q_ASSERT(_fbScene->isValid());

    //initialize the screen-filling quad:
    _screenQuad = new Rectangle{ _shader, QVector2D{ 2.f, 2.f} };

    //init gauss:
    _gauss = new GaussBlur{ width() * _lightmapScale, height() * _lightmapScale };

    /* TEST CODE ----------------------------------------------------------------------------- */
    //generate several lights

    ShapeMaker::instance()->setShader(_sceneShader);
    _lights.push_back(new Light{ "light2", QVector3D{ 0.f, 0.f, 0.f }, 4.f });
    /*
    for (size_t i = 0; i < 5; i++) {
        QVector3D pos =QVector3D{ -5.f + 2.f*float(i), 0.f, 0.f };
        _lights.push_back(new Light{ "light2", pos, 2.f });
    }*/

    //generate some pseudo-randomized game entities:*/
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
    for (auto& L : _lights) {
        L->update(16);
    }
    /* END OF TEST CODE --------------------------------------------------------------------- */

    _cam.update();

    //DRAW THE SCENE IN 3 STEPS:
    drawLights();
    drawScene();
    drawFinal();

    //add new lights <entities>:
    for (auto& pos : _newEntPos) {
        _lights.push_back(
            new Light{ "light2", QVector3D{ 0.f, 0.f, 0.f }, 4.f });
        _lights.back()->transform()->setPos(pos);
    }
    _newEntPos.clear();
}


void GLWidget::resizeGL(int width, int height)
{
    _gl->glViewport(0, 0, width, height);
    _cam.resize(width, height);

    //we also need to resize our framebuffers:
    delete _fbScene;
    _fbScene = new QOpenGLFramebufferObject{ width, height };
    delete _fbLight;
    _fbLight = new QOpenGLFramebufferObject{
            width * _lightmapScale,
            height * _lightmapScale };
    Q_ASSERT(_fbScene->isValid());
    Q_ASSERT(_fbLight->isValid());

    //resize gauss framebuffers:
    delete _gauss;
   _gauss = new GaussBlur{ width * _lightmapScale, height * _lightmapScale };
}

void GLWidget::initShaders() {
    //Create the final shader:
    _shader = new QOpenGLShaderProgram();
    _shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shaders/final.vert.glsl");
    _shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shaders/final.frag.glsl");
    _shader->link();

    //Create the light shader:
    _lightShader = new QOpenGLShaderProgram();
    _lightShader->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shaders/lights.vert.glsl");
    _lightShader->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shaders/lights.frag.glsl");
    _lightShader->link();

    //Create the scene shader:
    _sceneShader = new QOpenGLShaderProgram();
    _sceneShader->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shaders/scene.vert.glsl");
    _sceneShader->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shaders/scene.frag.glsl");
    _sceneShader->link();
}

void GLWidget::drawLights() {
    _fbLight->bind();
    _gl->glViewport(0, 0, _fbLight->width(), _fbLight->height());
    _gl->glEnable(GL_BLEND);
    _gl->glBlendFunc(GL_ONE, GL_ONE); //additive blending for the lights
    _gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _lightShader->bind();

    setCameraUniforms(_lightShader);

    for (auto& L : _lights) L->draw(_lightShader);

    _lightShader->release();
    _fbLight->release();
    _gl->glViewport(0, 0, this->width(), this->height());
}

void GLWidget::drawScene() {
    _gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    _fbScene->bind();
    _gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*
    //draw lights again with 1/3 of size so we can see them in empty areas:
    _lightShader->bind();
    for (auto& L : _lights) {
        L->transform()->scale(2.f);
        L->draw(_lightShader);
        L->transform()->scale(4.f);
    }
    _lightShader->release();*/

    //bind shader and set camera matrices:
    _sceneShader->bind();
    setCameraUniforms(_sceneShader);


    //draw our gameEntities:
    for (auto& E : _entities) E->draw(_sceneShader);

    _sceneShader->release();
    _fbScene->release();
}

void GLWidget::drawFinal() {
    _gl->glDisable(GL_BLEND);
    _gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //test: gauss blur
    //int text = _gauss->blurDS(_fbScene, 1);
    //_gl->glBindFramebuffer(GL_FRAMEBUFFER, 0); //why necessary?
    //_gl->glViewport(0, 0, this->width(), this->height());

    _shader->bind();

    //bind the textures from our two framebuffers:
    int sceneTex = _shader->uniformLocation("sceneTex");
    int lightTex = _shader->uniformLocation("lightTex");

    _gl->glActiveTexture(GL_TEXTURE0);
    _gl->glBindTexture(GL_TEXTURE_2D, _fbScene->texture());
    _gl->glUniform1i(sceneTex, 0);

    _gl->glActiveTexture(GL_TEXTURE1);
    _gl->glBindTexture(GL_TEXTURE_2D, _fbLight->texture());
    _gl->glUniform1i(lightTex, 1);

    _screenQuad->draw(_shader);

    _shader->release();
}

void GLWidget::setCameraUniforms(QOpenGLShaderProgram* shader) {
    _gl->glUniformMatrix4fv(shader->uniformLocation("O"), 1, GL_FALSE, _cam.orthoMatrix().data());
    _gl->glUniformMatrix4fv(shader->uniformLocation("V"), 1, GL_FALSE, _cam.viewMatrix().data());
}
