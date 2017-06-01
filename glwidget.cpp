#include "glwidget.h"
#include "Shapes/concreteshapes.h"
#include "Shapes/shapemaker.h"
#include "Entities/movingentity.h"
#include "Shapes/shapemaker.h"

#include <QDebug>
#include <QWheelEvent>
#include <sstream>

//test
#include "utils/randomizer.h"

GLWidget::GLWidget(QWidget* parent, QColor clearColor /* = QColor::black */)
    :   QOpenGLWidget{ parent },
        _gl         { nullptr },
        _color      { clearColor },
        _cam        { this->width(), this->height() },
        _fbLight    { nullptr },
        _fbScene    { nullptr },
        _screenQuad { nullptr },
        _debugWin   { nullptr },
        _picker     { &_scene, &_cam, this },
        _shader     { nullptr },
        _lightShader{ nullptr },
        _sceneShader{ nullptr },
        _gauss      { nullptr },
        _lightmapScale { 0.5f }
{

}

GLWidget::~GLWidget() {
    DEL(_gauss)         //DEL macro defined in glwidget.h
    DEL(_sceneShader)
    DEL(_lightShader)
    DEL(_shader)
    DEL(_debugWin)
    DEL(_screenQuad)
    DEL(_fbScene)
}


void GLWidget::mousePressEvent(QMouseEvent *event) {
    _mousePos = event->pos();
    setCursor(QCursor{ Qt::CursorShape::OpenHandCursor });

    //display debug info if we hit a shape with the cursor:
    auto entity = _picker.pick(_mousePos);
    if (entity) sendShapeInfo(entity);
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

void GLWidget::keyPressEvent(QKeyEvent *event) {
    //debugging transfrom class...
    if (event->text() == "w") _scene["tri"]->transform()->addRotation(-10.f, QVector3D{0.f, 0.f, 1.f});
    if (event->text() == "a") {
        _scene["tri"]->transform()->move(QVector3D{0.0f, 0.5f, 0.0f});
        _scene["tri"]->transform()->setRotationZ(182.f);
    }
    if (event->text() == "d") _scene["tri"]->transform()->setRotationZ(-45.f);
    if (event->text() == "s") _scene["tri"]->transform()->setRotationZ(180.f);
    if (event->text() == "q") _scene["tri"]->transform()->move(QVector3D{ 0.f, 1.f, 0.f});
}

void GLWidget::initializeGL()
{
    initDebugWin();
    setFocusPolicy(Qt::StrongFocus);

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
            int(this->width() * _lightmapScale),
            int(this->height()* _lightmapScale) };
    _fbScene = new QOpenGLFramebufferObject{ this->width(), this->height() };

    Q_ASSERT(_fbLight->isValid());
    Q_ASSERT(_fbScene->isValid());

    //initialize the screen-filling quad:
    _screenQuad = new Rectangle{ _shader, QVector2D{ 2.f, 2.f} };

    //init gauss:
    _gauss = new GaussBlur{ int(width() * _lightmapScale), int(height() * _lightmapScale) };

    /* TEST CODE ----------------------------------------------------------------------------- */
    //generate several lights

    ShapeMaker::instance()->setShader(_sceneShader);
    _scene.add(new Light{ "light2", QVector3D{ 0.f, 0.f, 0.f }, 8.f });

    /*
    for (size_t i = 0; i < 5; i++) {
        QVector3D pos =QVector3D{ -5.f + 2.f*float(i), 0.f, 0.f };
        _lights.push_back(new Light{ "light2", pos, 2.f });
    }*/

    //generate some pseudo-randomized game entities:*/
    int counter { 0 };
    for (size_t i = 0; i < 20; i++) {
        std::string name = std::to_string(counter);

        int type = Randomizer::randInt(0, 2);
        float dx = Randomizer::randFloat(0.f, 12.f) - 6.f;
        float dy = Randomizer::randFloat(0.f, 12.f) - 6.f;
        float w = Randomizer::randFloat(0.5f, 2.f);
        float h = Randomizer::randFloat(0.5f, 2.f);

        switch(type) {
        case 0:
            _scene.add(
                new GameEntity{ ShapeMaker::instance()->get(ShapeType::rectangle) },
                name);
            break;
        case 1:
            //for the triangle, we crate a moving entity to test the wander behaviour:
            _scene.add(
                new MovingEntity{ &_scene, ShapeMaker::instance()->get(ShapeType::triangle), 100.f },
                name);
            break;
        case 2:
            _scene.add(
                new GameEntity{ ShapeMaker::instance()->get(ShapeType::circle) },
                name);
            break;
        }
        if (type != 1) {
            _scene[name]->transform()->scale(QVector3D(w, h, 1.f));
        }
        else {
            _scene[name]->transform()->scale(QVector3D(1, 2, 1.f));
        }
        _scene[name]->transform()->setPos(QVector3D{ dx, -dy, 0.0 });

        counter++;
    }
    //debug: set a user-controlable triangle:
    _scene.add(new GameEntity{ ShapeMaker::instance()->get(ShapeType::triangle) }, "tri");
    _scene["tri"]->transform()->scale(QVector3D{ 2, 4, 1.f });

    /* END OF TEST CODE --------------------------------------------------------------------- */
}

void GLWidget::paintGL()
{
    static int counter{ 0 };
    std::string name = "Light" + std::to_string(counter++);

    _cam.update();
    _scene.update(1);

    //DRAW THE SCENE IN 3 STEPS:
    drawLights();
    drawScene();
    drawFinal();

    //add new lights <entities>:
    for (auto& pos : _newEntPos) {
        _scene.add(
            new Light{ "light2", QVector3D{ 0.f, 0.f, 0.f }, 4.f }, name);
        _scene[name]->transform()->setPos(pos);
    }
    _newEntPos.clear();

    //debug: retrieve messages from WinMsg and send them to our debug window:
    _debugWin->addText(WinMsg::get());
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

   //resize debug Window:
   _debugWin->onResize(width, height);
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

    _scene.drawLights(_lightShader);

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
    _scene.drawShapes(_sceneShader);

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

void GLWidget::initDebugWin() {
    //test: add a second window with debug information (later we can use this
    //window for user options)
    _debugWin = new controlWindow{ this };
    _debugWin->setWindowFlags(Qt::WindowStaysOnTopHint);
    _debugWin->resize(QSize{ this->width(), this->height() });
    _debugWin->setWindowTitle("Debug Win");
    _debugWin->show();
}

void GLWidget::sendShapeInfo(GameEntity* entity) {

    _debugWin->addText("<b>Properties of the object you clicked on:</b>");
    _debugWin->addText("Type id:\t" + std::to_string((int)entity->type()));
    _debugWin->addText("Position:\t" +
                       std::to_string(entity->transform()->pos().x()) + " / " +
                       std::to_string(entity->transform()->pos().y()) + " / " +
                       std::to_string(entity->transform()->pos().z()));
    _debugWin->addText("Angle:\t" + std::to_string(entity->transform()->rotationZ()));

    //for moving Entities, we give more infos:
    if (entity->type() == EntType::moving) {
        auto movingEnt = static_cast<MovingEntity*>(entity);
        _debugWin->addText("Velocity:\t" +
                        std::to_string(movingEnt->velocity().x()) + " / " +
                        std::to_string(movingEnt->velocity().y()) + " / " +
                        std::to_string(movingEnt->velocity().y()));

        //track the object for one frame:
        movingEnt->track();
    }
}
