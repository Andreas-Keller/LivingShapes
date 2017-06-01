#include "objpicker.h"

#include "Scene.h"
#include "camera.h"
#include "Entities/gameentity.h"

#include <QMatrix4x4>

ObjPicker::ObjPicker(Scene *scene, Camera* cam, QWidget* window)
    :   _scene  { scene },
        _cam    { cam },
        _window { window }
{
    Q_ASSERT(_scene && _cam && _window);
}

ObjPicker::~ObjPicker() { }

GameEntity *ObjPicker::pick(const QPoint& mousePos) {

    //get the openGL-Coordinates of the mouse:
	QVector3D pos = mouseWorldPos(mousePos);

	//TO DO: Perform test against every objects AABB and return the first hit
    for (auto i = _scene->shapesBegin(); i != _scene->shapesEnd(); i++) {
		if ((*i)->aabb()->isInside(pos)) {
			return (*i);
		}
    }

    //no luck:
    return nullptr;
}

QVector3D ObjPicker::mouseWorldPos(const QPoint& mousePos) const {
    float glX = float(mousePos.x()) / (0.5f*float(_window->width())) - 1.f;
    float glY = float(mousePos.y()) / (0.5f*float(_window->height())) - 1.f;

    //get the inverse matrix of the camera-model-matrix:
    QMatrix4x4 M = (_cam->orthoMatrix() * _cam->viewMatrix()).inverted();

    QVector3D glMouse = { glX, -glY, 0.f };
    return (M * glMouse);
}
