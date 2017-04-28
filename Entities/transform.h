/*  Class Transform
    holds position, scale and rotation and let us extract a matrix containing these three elements

    all functions are inlined in the header file
*/


#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>

class Transform {
public:
    Transform() : _scale{ QVector3D(1.f, 1.f, 1.f) } {}
    ~Transform() {}


    /* setters: */
    void setPos (const QVector3D& pos)   { _pos = pos; }
    void move   (const QVector3D& val)   { _pos += val; }

    void scale  (float val)              { _scale = QVector3D{ val, val, val }; }
    void scale  (const QVector3D& val)   { _scale = val; }

    void setRotation(float angle, const QVector3D& axis) {
        _rot = QQuaternion::fromAxisAndAngle(axis, angle);
    }
    void addRotation(float angle, const QVector3D& axis) {
        QQuaternion q = QQuaternion::fromAxisAndAngle(axis, angle);
        _rot = _rot * q;
        _rot.normalize();
    }

    //convenience functions because we work in 2D and therefore will always rotate around the Z-Axis:
    void setRotationZ(float angle) {
        setRotation(angle, QVector3D{ 0.f, 0.f, 1.f });
    }
    float rotationZ() {
        float pitch = 0.f; float yaw = 0.f; float roll = 0.f;
        _rot.getEulerAngles(&pitch, &yaw, &roll);
        return roll;
    }

    /* getters: */
    QVector3D pos() const       { return _pos; }
    QVector3D scale() const     { return _scale; }
    QQuaternion rotation() const { return _rot; }
    QVector4D rotationAngleAxis() const {
        float angle{ 0.f };
        float x = 0.f; float y = 0.f; float z = 0.f;
        _rot.getAxisAndAngle(&x, &y, &z, &angle);
        return QVector4D{ angle, x, y, z };
    }

    /*  this is the matrix we need to give our shapes before rendering them. */
    QMatrix4x4 matrix() {
        QMatrix4x4 M;
        M.setToIdentity();
        M.scale(_scale);
        M.translate(_pos);
        M.rotate(_rot);
        return M;
    }


private:
    QVector3D       _pos;
    QVector3D       _scale;
    QQuaternion     _rot;

};

//Math helper function: get the translation part from a matrix:
inline QVector2D getTranslation(const QMatrix4x4& M) {
    return QVector2D{ M.column(3) };
}

#endif // TRANSFORM_H
