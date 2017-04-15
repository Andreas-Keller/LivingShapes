/*  Orthographic Camera - needed in case the window is resized or in case we want to have
    a scene bigger than the screen and we want to scroll around */

#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>

class Camera {
public:
    Camera(
        int canvasWidth, int canvasHeight,
        QVector3D position = { 0.0, 0.0, 0.0 },
        float zoom = 1.0);

    ~Camera();

    void update();
    void resize(int width, int height);

    //get the cams matrices:
    const QMatrix4x4& viewMatrix() const      { return _V; }
    const QMatrix4x4& orthoMatrix() const     { return _O; }

    //getters and setters for zoom and position:
    float       zoom() const            { return _zoom; }
    void        setZoom(float value)    { _zoom = (value >= _zoomMin)? value : _zoom; }

    QVector3D   pos() const             { return _pos; }
    void        setPos(QVector3D pos)   { _pos = pos; }
    void        move(QVector3D value)   { _pos += value; }

private:
    QVector3D   _pos;
    float       _zoom;
    float       _zoomMin;

    QMatrix4x4  _O; //orthographic Matrix
    QMatrix4x4  _V; //view Matrix
};

#endif // CAMERA_H
