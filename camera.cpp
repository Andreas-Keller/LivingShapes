#include "camera.h"

Camera::Camera(int canvasWidth, int canvasHeight, QVector3D position, float zoom) :
    _pos    { position },
    _zoom   { zoom }
{
    _zoom = 200.0;
    _O.setToIdentity();
    _O.ortho(0.0, (float)canvasWidth, 0.0, (float)canvasHeight, -1.0, 1.0);
    _V.setToIdentity();
    _V.scale(_zoom);
    _V.translate(_pos);

}


Camera::~Camera()
{}


void Camera::update()
{
   _V.setToIdentity();
   _V.scale(_zoom);
   _V.translate(_pos);
}


void Camera::resize(int width, int height)
{
    _O.setToIdentity();
    _O.ortho(-float(width) / 2.0, (float)width / 2.0, -float(height) / 2.0, (float)height / 2.0, -1.0, 1.0);
}
