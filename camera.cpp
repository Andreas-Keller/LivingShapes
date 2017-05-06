#include "camera.h"

Camera::Camera(int canvasWidth, int canvasHeight, QVector3D position, float zoom) :
    _pos    { position },
    _rot    { 0.f },
    _zoom   { zoom },
    _zoomMin{ 10.f },
    _sensitivity{ 0.5f }
{
    _zoom = 200.0;
    _O.setToIdentity();
    _O.ortho(
        -(float)canvasWidth / 2.0, (float)canvasWidth / 2.0,
        -(float)canvasHeight / 2.0, (float)canvasHeight / 2.0, -1.0, 1.0);
    _V.setToIdentity();
    _V.scale(_zoom);
    _V.translate(_pos);

}


Camera::~Camera()
{}


void Camera::update()
{
   _V.setToIdentity();
   _V.translate(_pos);
   _V.rotate(_rot, QVector3D{ 0.f, 0.f, 1.f });
   _V.scale(_zoom);
}


void Camera::resize(int width, int height)
{
    _O.setToIdentity();
    _O.ortho(-float(width) / 2.0, (float)width / 2.0, -float(height) / 2.0, (float)height / 2.0, -1.0, 1.0);
}
