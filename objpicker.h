/*  Class to pick shapes by mouse click - checks the position of the mouse in world space and return the first
    object which intersects with this position */

#ifndef OBJPICKER_H
#define OBJPICKER_H

#include <QVector3D>
#include <QPoint>
#include <QWidget>

class Scene;
class GameEntity;
class Camera;

class ObjPicker {
public:
    ObjPicker(Scene* scene, Camera *cam, QWidget *window);
    ~ObjPicker();

    GameEntity* pick(const QPoint& mousePos);

    //converts the position of the mouse on-screen to its position in the game-world
    QVector3D mouseWorldPos(const QPoint&) const;

private:
    Scene*      _scene;
    Camera*     _cam;
    QWidget*    _window;
};

#endif // OBJPICKER_H
