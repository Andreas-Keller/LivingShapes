﻿#include "glwidget.h"
#include "Shapes/concreteshapes.h"
#include "Shapes/shapemaker.h"
#include "Entities/movingentity.h"
#include "Shapes/shapemaker.h"

#include <QDebug>
#include <QWheelEvent>
#include <sstream>

//test
#include "utils/randomizer.h"

GLWidget::GLWidget(QWidget* parent)
    :   QOpenGLWidget{ parent },
        _cam        { this->width(), this->height() },
        _debugWin   { nullptr },
        _picker     { &_scene, &_cam, this }
{}

GLWidget::~GLWidget() {
    DEL(_debugWin)
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
    MovingEntity* obj = static_cast<MovingEntity*>(_scene["tri"]);

    if (event->text() == "w")
        obj->setAcceleration(QVector3D{ 0.f, 0.4f, 0.f});
    if (event->text() == "s")
        obj->setAcceleration(QVector3D{ 0.f, -0.4f, 0.f});
    if (event->text() == "a")
        obj->setAcceleration(QVector3D{ -0.4f, 0.f, 0.f});
    if (event->text() == "d")
        obj->setAcceleration(QVector3D{ 0.4f, 0.f, 0.f});
    if (event->text() == " ") {
        obj->setVelocity(QVector3D{ 0.f, 0.f, 0.f });
        obj->setAcceleration(QVector3D{0.f, 0.f, 0.f});
    }
}

void GLWidget::initializeGL()
{
	//initialize debug window:
    initDebugWin();

	//let window get input if its has mouse focus
    setFocusPolicy(Qt::StrongFocus);

	//initialize the renderer:
	_renderer = new Renderer{
					&_scene, 
                    this->width(), this->height() };

    /* TEST CODE ----------------------------------------------------------------------------- */
    //generate several lights

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
    _scene.add(new MovingEntity{ &_scene, ShapeMaker::instance()->get(ShapeType::triangle), 100.f }, "tri");
    _scene["tri"]->transform()->scale(QVector3D{ 2, 4, 1.f });
    static_cast<MovingEntity*>(_scene["tri"])->steering()->wanderOff();

    /* END OF TEST CODE --------------------------------------------------------------------- */
}

void GLWidget::paintGL()
{
    static int counter{ 0 };
    std::string name = "Light" + std::to_string(counter++);

    _cam.update();
    _scene.update(16);  //because paintGL gets called every 16ms

    //let the renderer do the drawing:
    _renderer->draw(_cam);

    /*add new lights <entities>:
    for (auto& pos : _newEntPos) {
        _scene.add(
            new MovingEntity{ &_scene, ShapeMaker::instance()->get(ShapeType::triangle), 100.f }, name);
        _scene[name]->transform()->setPos(pos);
        _scene[name]->transform()->scale(QVector3D{ 2, 4, 1.f });
    }*/
    _newEntPos.clear();

    //debug: retrieve messages from WinMsg and send them to our debug window:
    _debugWin->addText(WinMsg::get());
}

void GLWidget::resizeGL(int width, int height)
{
    _cam.resize(width, height);
	_renderer->resize(width, height);

   //resize debug Window:
   _debugWin->onResize(width, height);
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
